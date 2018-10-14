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

#include "NoiseMaker.h"

#ifndef H_HEIGHTMAP_TREE

#define H_HEIGHTMAP_TREE

// Quadtree used for storage of heightmap tiles.  They
// are kept independently so heightmap tiles of the lowest
// FILESYSTEM level (which may be 10m scale or more) can
// be re-used when creating the refinement levels lower
// down.
//
// (Or just for caching purposes...)

struct HeightmapData {
	bool isSea;
	float minY;
	float maxY;
	unsigned char* data;
};

class HeightmapTree
{
public:

	static NoiseMaker* noise;

	HeightmapTree(int, int, int);
	~HeightmapTree();

	// Get the data at x, z, lod
	HeightmapData* getData(int, int, int);

	// Clean up - maybe this will
	// need to take a parameter detailing
	// how many tiles to clean up?
	void clean();

private:
	// Children
	HeightmapTree* child[4];

	// Location of this tile
	int x, z;

	// Start and end points within parent tile
	float parentX1, parentX2;
	float parentZ1, parentZ2;

	// Pointer to the parent data
	unsigned char* parentData;

	// Level of detail
	int lod;

	// The heightmap data
	HeightmapData data;

	// Variables used to track when a tile should be uncached.
	int lastAccessed;
	static int access;

	// Data loading function
	void loadData();
};

#endif