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
#include <iostream>

#ifndef H_SRTM

#define H_SRTM

class SRTM {
public:
	static const int tileSize = 6001;

	static int getTileX(double);
	static int getTileZ(double);

	// Overloaded options for getting file names
	static void getFileName(double,double, char*);
	static void getFileName(int,int, char*);

	// Constructor/destructor
	SRTM(float, float);
	SRTM(int, int);
	~SRTM();

	// Tile co-ordinate stuff, part II
	int getX(double);
	double getXError(double);
	int getZ(double);
	double getZError(double);

	// Data get function
	int getDataAtPoint(int,int);

private:
	// Tile co-ordinates
	int x;
	int z;

	// Tile data
	short *data;

	void load(char* fileName);

	unsigned char* fileArray;


};

#endif