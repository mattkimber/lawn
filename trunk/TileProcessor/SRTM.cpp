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
#include <string>
#include <fstream>
#include "windows.h"

#include "SRTM.h"


int SRTM::getTileX(double lng)
{
	return (int)((lng+180.0) / 5.0) + 1;
}

int SRTM::getTileZ(double lat)
{
	return 24 - (int)((lat+60.0) / 5.0);
}

void SRTM::getFileName(double lat, double lng, char* ca)
{		
	// Nobbly.
	sprintf_s(ca, 256, "srtm_%02d_%02d.raw", getTileX(lng), getTileZ(lat));

}

void SRTM::getFileName(int x, int z, char* ca)
{		
	// Also nobbly.
	sprintf_s(ca, 256, "srtm_%02d_%02d.raw", x, z);
}


void SRTM::load(char* fileName)
{
	std::cout << "Loading heightmap " << fileName << "...";
	
	long start = GetTickCount();
	

	data = new short[tileSize * tileSize];
	int val;

	// AAARGH - doubles memory requirement!
	// TODO - do we need this now we're outside Java?
	fileArray = new unsigned char[2 * SRTM::tileSize * SRTM::tileSize];
	
	char qualifiedFileName[256] = "";
	strcat_s(qualifiedFileName, "E:\\Projects\\Data\\");
	strcat_s(qualifiedFileName, fileName);


	//std::fstream infile(qualifiedFileName, std::ios::binary|std::ios::in);
	std::ifstream infile(qualifiedFileName, std::ios::binary|std::ios::in);
	
	infile.read((char *)fileArray, 2 * SRTM::tileSize * SRTM::tileSize);
	
	for(int i = 0; i < (tileSize*tileSize*2); i+=2)
	{
			
		val = (( fileArray[i+1]<<8 ) | ( fileArray[i]&0x00FF ));
		
		// Cope with null value - sea level
		if (val == 32767) val = -5;

		
		data[i/2] = (short)val;
		
	}
	
	
	infile.close();

	delete [] fileArray;

	long duration = (GetTickCount() - start) / 1000;

	std::cout << " (took " << duration << "s)" << std::endl;
}


SRTM::SRTM(float lat, float lng)
{	
	// Set initial values.
	x = getTileX(lng);
	z = getTileZ(lat);
	
	char fileName[256];
	getFileName(x,z, fileName);

	// Load the file
	load(fileName);
}
	
	SRTM::SRTM(int x_in, int z_in)
	{	
		x = x_in;
		z = z_in;
	
		char fileName[256];
		getFileName(x,z, fileName);

		// Load the file
		load(fileName);
	}

	int SRTM::getX(double lng)
	{

		double lngWithinTile = lng - ((x - 37) * 5.0);
		
		// Add 0.5f to compensate for sampled values corresponding to the centre of a tile
		return (int)(((lngWithinTile * tileSize) / 5.0)+0.5);
	}
	



	double SRTM::getXError(double lng)
	{
		int i = getX(lng);
		double lngWithinTile = lng - ((x - 37) * 5.0);
		double f = (((lngWithinTile * tileSize) / 5.0)+0.5);
		
		return f - (double)i;
	}
	
	int SRTM::getZ(double lat)
	{
		double latWithinTile = 5.0 - (lat + ((z - 12) * 5.0));
		
		// Index from top
		return (int)(((latWithinTile * tileSize) / 5.0)+0.5);
	}
	
	double SRTM::getZError(double lat)
	{
		int i = getZ(lat);
		double latWithinTile = 5.0f - (lat + ((z - 12) * 5.0f));
		double f = (((latWithinTile * tileSize) / 5.0f)+0.5f);
		
		// Remember we index in the opposite direction.
		return  f - (float)i;
	}


	int SRTM::getDataAtPoint(int x, int z)
	{
		if (NULL == data) return 0; 
		else return data[x + (z * tileSize)];
	}

