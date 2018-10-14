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
#include "HeightmapProcessor.h"
#include "QuadtreeProcessor.h"
#include "math.h"
#include <fstream>

const char HeightmapProcessor::tileLocation[] = "E:\\Projects\\Data\\Tiles\\";


float HeightmapProcessor::getHeightAtPoint(double i, double j)
{
	//return getHeightAtPointLinear(i,j);
	return srtm->getHeight(i, j);
	///return 23;
}




	// Save the heightmap level
	void HeightmapProcessor::Save()
	{
				
		int offset = size / 2;
		int file_x = ((x - offset) / points);
		int file_z = ((z - offset) / points);
		
		char fileName[256];
		sprintf_s(fileName, 256, "%s%d_%d_%d.tile", tileLocation, lod, file_x, file_z);

		std::ofstream outfile(fileName, std::ios::binary|std::ios::out);
	


		//ObjectOutputStream outfile = new ObjectOutputStream(new FileOutputStream(fileName));
		// outfile.write((char *)&isSea, sizeof(isSea));
		outfile.write((char *)&minY, sizeof(minY));
		outfile.write((char *)&maxY, sizeof(maxY));
		outfile.write((char *)hmap, 4*Config::tileSize*Config::tileSize);
				
		
		
		outfile.flush();
		outfile.close();

		

		
	}
	
	// 1. Get a list of vertices
	void HeightmapProcessor::getVertices()
	{
		int n = 0;
		float h;
		// Used to create the heightmap at first
		float** hmapValues;
			
		hmapValues = new float*[sizeMap+1];
		for(int i = 0; i < sizeMap + 1; ++i) { hmapValues[i] = new float[sizeMap+1]; }
		

		int lod = (size/points);
		double lodBias = (double)size / (double)(points - 1);
		int offset = size / 2;
		
		for (int i = -1; i < sizeMap; i ++) {
			for (int j = -1; j < sizeMap; j ++){
				
				// Insert the data from the paged height map
				h = (getHeightAtPoint(
						((double)x - offset) + ((double)i * lodBias),
						((double)z - offset) + ((double)j * lodBias)
						//((double)x - offset) + ((double)i * lodScale),
						//((double)z - offset) + ((double)j * lodScale)
						//(double)((x-offset) + ((double)i*lodBias)) * (double)size / (double)Config::tileSize,
						//(double)((z-offset) + ((double)j*lodBias)) * (double)size / (double)Config::tileSize
						));
				

				
				if (h < minY) minY = h;
				if (h > maxY) maxY = h;
				
				if (h != -5.0f) isSea = false;
				
				hmapValues[i+1][j+1] = h;
			}
			
		}
		
		// Copy to the final byte array
		for (int i = 0; i < points; i ++) {
			for (int j = 0; j < points; j ++){
				h = hmapValues[i+1][j+1];
				//hmap[0][i][j] = (byte)((h / 1024) * 256);

				/*
				if (!((i > 1) && (i < 31) && (j > 1) && (j < 31)))
				{
					h = 30;
					maxY = 30;
				}
				else
				{
					h = 0;
					minY = 0;
				}
				*/
				
				float r = 32 * (abs(hmapValues[i+1][j+1] - hmapValues[i][j]) + abs(hmapValues[i+1][j+1] - hmapValues[i][j])) / (size / points);
				if(r > 255) r = 255;

				// Choose the material
				unsigned char mat;
				if ( h < 1.0 ) { mat = 0; }
				else if ( r > 128 ) { mat = 3; }
				else if ( h < 5.0 ) { mat = 1; }
				else { mat = 2; }

				// Store a preliminary roughness value in the heightmap
				hmap[(i*4)+(j*points*4)] = mat;
				hmap[1+(i*4)+(j*points*4)] = (unsigned char)(((h - minY) / (maxY - minY)) * 255.0);
				hmap[2+(i*4)+(j*points*4)] = (unsigned char)((h - ((float)hmap[1+(i*4)+(j*points*4)] / 255.0f)) * 255.0);
				hmap[3+(i*4)+(j*points*4)] = 0;

				
			}

			//if(lod == 1) { std::cout << (int)(((h - minY) / (maxY - minY)) * 255.0) << std::endl; }
		}

		//if(lod == 1) {  std::cout << "Tile finished" << std::endl; }
		

		// Free up the memory
		for(int i = 0; i < sizeMap + 1; ++i) { delete [] hmapValues[i]; }
		delete [] hmapValues;

	}
		

HeightmapProcessor::HeightmapProcessor(int x_in, int z_in, int size_in, int lod_in, SRTMManager* srtm_in)
{
	x = x_in;
	z = z_in;
	size = size_in;
	lod = lod_in;
	srtm = srtm_in;

	isSea = true;
	points = Config::tileSize;
	sizeMap = points + 1;

	minY = 32767.0f;
	maxY = -32767.0f;

	// Create the new byte map
	// Has four channels.
	// Channel 1: coarse height ( 1/256th of max )
	// Channel 2: fine height (1/256th fractions )
	hmap = new unsigned char[4*points*points];
	
	// Get the vertices
	getVertices();

}

HeightmapProcessor::~HeightmapProcessor()
{
	delete [] hmap; 
}