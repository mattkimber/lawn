/*
 * Copyright (C) 2011 by Matt Kimber
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "stdafx.h"

#include "SRTMManager.h"
#include "math.h"
#include "windows.h"

#define PI 3.1415926535

const double SRTMManager::radius = 6371000.0;


	double SRTMManager::getLngFromMtrs(double x)
	{
		x = x * (double)Config::scale;

		double lon1 = lng * (PI / 180.0);
		double lat1 = lat * (PI / 180.0);
		double brng = PI / 2.0;
		
		double lat2 = asin( sin(lat1)*cos((double)x/radius) + 
                cos(lat1)*sin((double)x/radius)*cos(brng) );
		
		double lon2 = atan2(sin(brng)*sin((double)x/radius)*cos(lat1), 
                       cos((double)x/radius)-sin(lat1)*sin(lat2));
		
		return (lon1 + lon2) * (180.0 / PI);
		
	}


	double SRTMManager::getLatFromMtrs(double z)
	{
		z = z * (double)Config::scale;

		// Assume a constant radius
		return lat + (z / 111193.5);
	}


	float SRTMManager::getRoughnessHorizontal(int x, int z, int tx2, int tz)
	{
		// Set up the variables
		int tx1 = tx2; int tx3 = tx2;
		int u1 = x - 1; int u2 = x; int u3 = x + 1;
		
		// Check for no overflow
		if (u1 < 0) { u1 += SRTM::tileSize; tx1--; } 
		if (u3 > SRTM::tileSize) { u3 -= SRTM::tileSize; tx3++; }
		
		// Ensure we have tiles
		if(tiles[tx1][tz] == NULL) { tiles[tx1][tz] = new SRTM(tx1, tz); }
		if(tiles[tx3][tz] == NULL) { tiles[tx3][tz] = new SRTM(tx3, tz); }
		
		// Get the values
		float h1 = tiles[tx1][tz]->getDataAtPoint(u1,z);
		float h2 = tiles[tx2][tz]->getDataAtPoint(u2,z);
		float h3 = tiles[tx3][tz]->getDataAtPoint(u3,z);
		
		// Return the "roughness" (error value)
		return min(abs(h2 - ((h1 - h3) / 2)), abs(h1-h3));
	}

	float SRTMManager::getRoughnessVertical(int x, int z, int tx, int tz2)
	{
		// Set up the variables
		int tz1 = tz2; int tz3 = tz2;
		int v1 = z - 1; int v2 = z; int v3 = z + 1;
		
		// Check for no overflow
		if (v1 < 0) { v1 += SRTM::tileSize; tz1--; } 
		if (v3 > SRTM::tileSize) { v3 -= SRTM::tileSize; tz3++; }
		
		// Ensure we have tiles
		if(NULL == tiles[tx][tz1]) tiles[tx][tz1] = new SRTM(tx, tz1);
		if(NULL == tiles[tx][tz3]) tiles[tx][tz3] = new SRTM(tx, tz3);
		
		// Get the values
		float h1 = tiles[tx][tz1]->getDataAtPoint(x,v1);
		float h2 = tiles[tx][tz2]->getDataAtPoint(x,v2);
		float h3 = tiles[tx][tz3]->getDataAtPoint(x,v3);
		
		// Return the "roughness" (error value)
		// Clamping to the height difference reduces epic uber-spikiness
		// on mountains.
		return min(abs(h2 - ((h1 - h3) / 2)), abs(h1-h3));
	}

	float SRTMManager::getRoughness(int x, int z, int tx, int tz)
	{
		// gets the roughness of point (x, z) within a heightmap
		return (getRoughnessHorizontal(x,z,tx,tz) + getRoughnessVertical(x,z,tx,tz)) / 2;
	}


	float SRTMManager::getCubicValue(double x, float *f)
	{
		double a0, a1, a2, a3, x2;
		
		x2 = x * x;
		
		a0 = f[3] - f[2] - f[0] + f[1];
		a1 = f[0] - f[1] - a0;
		a2 = f[2] - f[0];
		a3 = f[1];
		
		return (float)(a0 * x * x2 + a1 * x2 + a2 * x + a3);
		//return  (f[1] * (1 - x)) + (f[2] * x);
	}

	float SRTMManager::getHeightNaive(double x, double z)
	{
		double lng = getLngFromMtrs(x);
		double lat = getLatFromMtrs(-z);
		
		int tx = SRTM::getTileX(lng);
		int tz = SRTM::getTileZ(lat);

		if((tx < 0) || (tx > 72) || (tz < 0) || (tz > 24)) return 0;

		if(NULL == tiles[tx][tz]) {
			// This tile doesn't exist (yet) so
			// we should load it.
			tiles[tx][tz] = new SRTM(tx, tz);
		}
		
		int cx = tiles[tx][tz]->getX(lng);
		int cz = tiles[tx][tz]->getZ(lat);
		
		return tiles[tx][tz]->getDataAtPoint(cx, cz);
	}

	float SRTMManager::getHeightCubic(double x, double z)
	{
		double lng = getLngFromMtrs(x);
		double lat = getLatFromMtrs(-z);

		int tx = SRTM::getTileX(lng);
		int tz = SRTM::getTileZ(lat);
		
		if(NULL == tiles[tx][tz]) {
			// This tile doesn't exist (yet) so
			// we should load it.
			tiles[tx][tz] = new SRTM(tx, tz);
		}
		
		int u[4];
		u[1] = tiles[tx][tz]->getX(lng);
		u[0] = u[1] - 1;
		u[2] = u[1] + 1;
		u[3] = u[1] + 2;
		
		int v[4];
		v[1] = tiles[tx][tz]->getZ(lat);
		v[0] = v[1] - 1;
		v[2] = v[1] + 1;
		v[3] = v[1] + 2;

		int txa[4];
		int tza[4];
		
		txa[0] = tx; tza[0] = tz;
		txa[1] = tx; tza[1] = tz;
		txa[2] = tx; tza[2] = tz;
		txa[3] = tx; tza[3] = tz;
		
		if (u[0] < 0) { u[0] += SRTM::tileSize; txa[0]--; }
		if (u[2] > SRTM::tileSize) { u[2] -= SRTM::tileSize; txa[2]++; }
		if (u[3] > SRTM::tileSize) { u[3] -= SRTM::tileSize; txa[3]++; }

		if (v[0] < 0) { v[0] += SRTM::tileSize; tza[0]--; }
		if (v[2] > SRTM::tileSize) { v[2] -= SRTM::tileSize; tza[3]++; }
		if (v[3] > SRTM::tileSize) { v[3] -= SRTM::tileSize; tza[3]++; }

		
		// Ensure the surrounding tiles if necessary
		for(int i = 0; i< 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if(NULL == tiles[txa[i]][tza[j]]) { tiles[txa[i]][tza[j]] = new SRTM(txa[i], tza[j]); }
			}
		}

		double dx =  tiles[tx][tz]->getXError(lng);
		double dz =  tiles[tx][tz]->getZError(lat);
		
		//System.out.println(String.format("Z: %2f dz: %2f X: %2f dx %2f", lat, dz, lng, dx));
		
		float l[4];
		
		for(int i = 0; i< 4; i++)
		{
			float h[4];
			
			for (int j = 0; j < 4; j++)
			{
				h[j] = tiles[txa[i]][tza[j]]->getDataAtPoint(u[i], v[j]);
			}
			
			l[i] = getCubicValue(dz, h);
		}
		                        
		return getCubicValue(dx, l);
	}

	float SRTMManager::getPerlin(int x, int z)
	{		
		return perlin(x,z,1,8) + perlin(x,z,.5f,16) + perlin(x,z,.25f,32) + perlin(x,z,.125f,64) + perlin(x,z,.0625f,128) + perlin(x,z,.03125f,256);
	}

	float SRTMManager::perlin(int x, int z, float amplitude, float frequency)
	{
		float u = (float)x / 1000.0f;
		float v = (float)z / 1000.0f;
		
		return amplitude * (float)Perlin::noise(u * frequency, v * frequency, amplitude/100.0f);
	}

	float SRTMManager::getHeightPerlinAugmented(int x, int z)
	{
		double lng = getLngFromMtrs(x);
		double lat = getLatFromMtrs(-z);
		
		int tx = SRTM::getTileX(lng);
		int tz = SRTM::getTileZ(lat);
		
		if(NULL == tiles[tx][tz]) {
			// This tile doesn't exist (yet) so
			// we should load it.
			tiles[tx][tz] = new SRTM(tx, tz);
		}
		
		// std::cout << "TX: " << tx << " TZ: " << tz << std::endl;

		int u = tiles[tx][tz]->getX(lng);
		int v = tiles[tx][tz]->getZ(lat);

		
		double dx =  tiles[tx][tz]->getXError(lng);
		double dz =  tiles[tx][tz]->getZError(lat);
		
		double h1 = getRoughness(u, v, tx, tz) * (1 - dx) * (1 - dz);
		double h2 = getRoughness(u+1, v, tx, tz) * dx * (1 - dz);
		double h3 = getRoughness(u, v+1, tx, tz) * (1 - dx) * dz;
		double h4 = getRoughness(u+1, v+1, tx, tz) * dx * dz;
		
		float r = (float)(h1 + h2 + h3 + h4);
		
		float hp = getPerlin(x, z);
		float h = getHeightCubic(x,z);
		
		// Don't perlinate smooth coasts
		hp = hp * (min(10.0f, max(0, h)) / 10.0f);
	
		return (float) (h + (hp * r));
	}


	float SRTMManager::getHeight(double x, double z)
	{
		float f;

		/// f = getHeightPerlinAugmented(x,z);
		f = getHeightCubic(x,z);
		//f = getHeightNaive(x,z);
		
		//System.out.println(f);
		
		return f;
	}

	SRTMManager::SRTMManager(float lt, float lg)
	{
		lat = lt;
		lng = lg;

		tiles = new SRTM**[72];

		for(int i = 0; i < 72; ++i) {
			tiles[i] = new SRTM*[24];
			for(int j = 0; j < 24; ++j) {
				tiles[i][j] = NULL;
			}
		}
	}

	SRTMManager::~SRTMManager()
	{
	}