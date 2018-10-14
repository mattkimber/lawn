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

#include "Config.h"
#include "SRTM.h"
#include "Perlin.h"

#ifndef H_SRTM_MANAGER

#define H_SRTM_MANAGER

class SRTMManager {

public:

	double getLngFromMtrs(double);
	double getLatFromMtrs(double);

	// Height getter
	float getHeight(double, double);

	// Constructor/destruct.
	SRTMManager(float, float);
	~SRTMManager();

protected:

	// Origin of centre point for projection
	double lng;
	double lat;

	// Tiles
	SRTM*** tiles;

	static const double radius;

	// "Roughness" functions (used for perlinating)
	float getRoughnessHorizontal(int, int, int, int);
	float getRoughnessVertical(int, int, int, int);
	float getRoughness(int,int,int,int);


	// Height getters
	float getCubicValue(double, float*);
	float getHeightCubic(double, double);
	float getHeightNaive(double, double);

	// Perlin stuff
	float getPerlin(int, int);
	float perlin(int, int, float, float);
	float getHeightPerlinAugmented(int, int);



};


#endif