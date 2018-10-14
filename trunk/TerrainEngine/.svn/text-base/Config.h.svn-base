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

#ifndef H_CONFIG

#define H_CONFIG

// Detail scale conversion to allow very small tiles
// at the lowest level.  With scale = 1 we can go down
// to 1 tile = 1m... with 32x32 tiles a maximum resolution
// of ~3cm.  Setting it to 4 would allow 0.5cm, at the cost
// of reducing the maximum universe size by that same factor.
#define DETAIL_SCALE 8

class Config {
public:
	
	static int quadtreeLevels;
	static int tileSize;

	static int scale;

	// First OGRE frustration!  Bounding box
	// is set at the MESH level, where it
	// will have to keep one generic value for
	// every tile we ever render (bum).
	//
	// To help with this, we set a "sensible"
	// maximum tile delta height (the difference
	// between minY and maxY per tile) for the geometry
	// we have.
	static int tileBBoxHeight;

	// Location of files
	static char tileLocation[256];

	// LOD metrics
	// I think this will get replaced by a precomputed LOD sphere eventually.
	static int virtualScreenSize;
	static int errorBias;

	static int lodStack;
	static int minLOD;

	static void load();
};

#endif