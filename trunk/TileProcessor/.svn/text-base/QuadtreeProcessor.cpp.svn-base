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
#include "QuadtreeProcessor.h"
#include "SRTMManager.h"
#include "math.h"
#include "windows.h"


QuadtreeProcessor::QuadtreeProcessor(float lt, float lg)
{
	lat = lt;
	lng = lg;

	scale = Config::scale;

	// Compute LODs even if bounding box says not to.
	highestPracticalLOD = 10;

	// Heightmap metrics - set to sensible values for SRTM
	hmap_size = 6001;
	hmap_centre = hmap_size / 2;
}

// Walking methods
void QuadtreeProcessor::walk(int lod, int x, int z, int minX, int minZ, int maxX, int maxZ)
{
	SRTMManager srtm(lat, lng);

		

	walk(lod,x,z,minX,minZ,maxX,maxZ,true, &srtm);
}
	
	
void QuadtreeProcessor::walk(int lod, int x, int z, int minX, int minZ, int maxX, int maxZ, bool recurse, SRTMManager* srtm_in)
{

	int tileSize = Config::tileSize;
	int size = (int)(tileSize * pow(2.0, lod));
	
	// Tree walking method...
	if (lod > Config::quadtreeLevels - 2) 
	{
		std::cout << "Processing LOD " << lod << " at {" << x << ", " << z << "}" << std::endl;
	}
	else if (lod == Config::quadtreeLevels - 2)
	{	
		std::cout << ".";
	}

	long start = GetTickCount();

	// Create a heightmap processor for the current tile
	HeightmapProcessor hp(x, z, size, lod, srtm_in);
	
	// Process the heightmap
	hp.Save();

	//long duration = GetTickCount() - start;

	//std::cout << "Tile generation time: " << duration << " ms" << std::endl;

	
	// If we're at the bottom LOD then return
	if((lod == 0) || (!recurse && lod <= highestPracticalLOD)) return;
	
	// For each sub-quadrant of the current LOD
	for(int i=0;i<2;i++)
	{
		for(int j=0;j<2;j++)
		{
			// Calculate the x and y of this quadrant
			int offset = tileSize * (int)pow(2.0, lod - 1);
			int x_me = (x - (offset/2) + (i * offset));
			int z_me = (z - (offset/2) + (j * offset));
			
			int s[2];
			int t[2];
			
			s[0] = x_me - offset; 
			s[1] = x_me + offset;
			t[0] = z_me - offset; 
			t[1] = z_me + offset;
			
			//System.out.println(String.format("%d %d %d %d", s[0], t[0], s[1], t[1]));
			//System.out.println(String.format("%d %d %d %d", minX/scale, minZ/scale, maxX/scale, maxZ/scale));

			bool inside = false;
			
			int u[2];
			int v[2];
			
			u[0] = minX /scale;
			u[1] = maxX /scale;
			v[0] = minZ / scale;
			v[1] = maxZ / scale;
			
			for (int ix =0;ix<2;ix++)
			{
				for(int jx = 0; jx < 2; jx++)
				{
					// If interest box is inside terrain
					if( (u[ix] >= s[0])
							&& (u[ix] <= s[1])
							&& (v[jx] >= t[0])
							&& (v[jx] <= t[1]) )
						inside = true;

					// If terrain is inside interest box
					if( (s[ix] >= u[0])
							&& (s[ix] <= u[1])
							&& (t[jx] >= v[0])
							&& (t[jx] <= v[1]) )
						inside = true;
				}
			}
			
			
			// Identify if the bounding box of the interest region
			// (as given by the method) falls within this quadrant,
			// either wholly or partially.
			if ( inside )
			{
				//System.out.println(" - Inside");
				// If it does, then recurse to the next level of detail
				QuadtreeProcessor::walk(lod - 1, x_me, z_me, minX, minZ, maxX, maxZ, true, srtm_in);
			}
			else
			{
				//System.out.println(" - Outside");
				QuadtreeProcessor::walk(lod - 1, x_me, z_me, minX, minZ, maxX, maxZ, false, srtm_in);
			}
		}
	}
	
		
}
