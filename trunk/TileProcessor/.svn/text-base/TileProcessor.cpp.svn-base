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
#include "windows.h"

// Classes
#include "QuadtreeProcessor.h"


int _tmain(int argc, _TCHAR* argv[])
{


	long start = GetTickCount();

	//for(int i=0; i < 10000; ++i)
	//{
	//double q = Perlin::noise(1.0, 27.0, 3.0);
	//}

	//QuadtreeProcessor q(51.662f, -1.576f);

	// Dover
	//QuadtreeProcessor q(51.1116f, 1.3275f);

	// Swanage Bay
	QuadtreeProcessor q(50.6127f, -1.9578f);
	int s = 10000;

	q.walk(Config::quadtreeLevels, 0, 0, -s, -s, s, s);
		

	long duration = GetTickCount() - start;

	std::cout << "Total elapsed time: " << duration / 1000 << " s" << std::endl;


	std::cin.get();

	return 0;
}

