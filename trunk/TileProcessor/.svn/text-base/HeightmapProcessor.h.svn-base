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

#ifndef H_HEIGHTMAP_PROCESSOR

#define H_HEIGHTMAP_PROCESSOR

class HeightmapProcessor {
public:

	HeightmapProcessor(int, int, int, int, SRTMManager*);
	~HeightmapProcessor();


	void Save();
	void getVertices();


protected:
	
	static const char tileLocation[256];

	static const int scale = Config::scale;

	// Size of the tile
	int size;
	int lod;

	// Is this sea?
	bool isSea;

	// x,z co-ordinates of tile centre
	int x;
	int z;

	// Edge length (vertices) - usually constant
	int points;
	int sizeMap;

	// Buffer
	unsigned char* hmap;

	// Precalculated tile values
	float minY;
	float maxY;

	// Get height at co-ord
	float getHeightAtPoint(double, double);

	// SRTM Management
	SRTMManager* srtm;

};

#endif