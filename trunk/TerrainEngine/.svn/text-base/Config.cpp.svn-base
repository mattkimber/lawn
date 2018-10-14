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

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <cstring>

char Config::tileLocation[256] = "E:\\Projects\\Data\\Tiles";

int Config::quadtreeLevels = 7;
int Config::tileSize = 16;

int Config::scale = 10;

int Config::tileBBoxHeight = 500;

int Config::lodStack;
int Config::minLOD;

// LOD metrics
// I think this will get replaced by a precomputed LOD sphere eventually.
int Config::virtualScreenSize = 768;
int Config::errorBias = 8;

// Loading the config file
void Config::load()
{

	// Create the property tree object.
	using boost::property_tree::ptree;
	ptree config;

	// Load XML file
	read_xml("config.xml", config);

	// Read the values
	quadtreeLevels = config.get<int>("terrain.QuadtreeLevels", 2);
	scale = config.get<int>("terrain.Scale", 10);

	tileSize = config.get<int>("lod.TileSize", 16);
	errorBias = config.get<int>("lod.Error", 8);
	virtualScreenSize = config.get<int>("lod.VirtualSize", 768);

	lodStack = config.get<int>("lod.MaxStack", 10);
	minLOD = config.get<int>("lod.MinLOD", 0);

	strcpy_s(tileLocation, 256, config.get<std::string>("terrain.Location", "E:\\Projects\\Data\\Tiles").c_str());
}