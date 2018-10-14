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

#include "HeightmapProcessor.h"

#ifndef H_QUADTREE_PROCESSOR
#define  H_QUADTREE_PROCESSOR

class QuadtreeProcessor
{
public:
	// Compute LODs even if bounding box says not to.
	int highestPracticalLOD ;

	// Heightmap metrics - set to sensible values for SRTM
	int hmap_size;
	int hmap_centre;

	QuadtreeProcessor(float, float);
	void walk(int, int, int, int, int, int, int);
	void walk(int, int, int, int, int, int, int, bool, SRTMManager*);

protected:
	int scale;

	
	float lat;
	float lng;

};

#endif