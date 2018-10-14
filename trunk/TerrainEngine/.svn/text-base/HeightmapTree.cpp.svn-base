#include "HeightmapTree.h"
#include "Config.h"
#include "math.h"
#include <stdlib.h>
#include <fstream>

int HeightmapTree::access = 0;

NoiseMaker* HeightmapTree::noise = new NoiseMaker();

HeightmapTree::HeightmapTree(int x, int z, int lod)
{
	// Set up the information about us
	this->x = x;
	this->z = z;
	this->lod = lod;

	// Set up some sensible initial values for
	// the parent-finder calculation
	this->parentX1 = 0.0f;
	this->parentX2 = 1.0f;
	this->parentZ1 = 0.0f;
	this->parentZ2 = 1.0f;

	data.data = NULL;

	for(int i = 0; i < 4; ++i)
	{
		child[i] = NULL;
	}
}

HeightmapTree::~HeightmapTree()
{
	// Free up the data
	if(data.data != NULL) delete[] data.data;
}

void HeightmapTree::loadData()
{
		// First get the filename
	char filename[256];
	int offset = (Config::tileSize * pow(2.0, lod)) / 2;
	int fileX = ((x / (Config::scale * DETAIL_SCALE)) - offset) / Config::tileSize;
	int fileZ = ((z / (Config::scale * DETAIL_SCALE)) - offset) / Config::tileSize;
	sprintf_s(filename, 256, "%s\\%d_%d_%d.tile", Config::tileLocation, lod, fileX, fileZ);

	data.data = new unsigned char[Config::tileSize * Config::tileSize * 4];

	// Then open the file
			
	std::ifstream infile(filename, std::ios::binary|std::ios::in);
	data.isSea = false;
	//infile.read((char *)&data.isSea, sizeof(data.isSea));
	infile.read((char *)&data.minY, sizeof(data.minY));
	infile.read((char *)&data.maxY, sizeof(data.maxY));

	infile.read((char *)data.data, Config::tileSize * Config::tileSize * 4);			
	infile.close();
}

HeightmapData* HeightmapTree::getData(int x, int z, int lod)
{

	if ( ( (this->x == x) && (this->z == z) && (this->lod == lod) ) )
	{

		if(this->lod >= 0) {
		// This is the correct piece of heightmap, so we now
		// need to handle loading (if necessary) and passing
		// through of the data we have in this piece of the
		// tree
			if(NULL == data.data)
			{
				// Need to load the data from file.
				this->loadData();
			}
		}
		// TODO: deal with the negative LODs
		else
		{
			// What will need to happen here.

			float xWithinParent, zWithinParent;
			float xF, zF;
			float xI, zI;
			int i1, i2, j1, j2;
			float h11, h12, h21, h22;
			float h, hi, hf;

			data.data = new unsigned char[Config::tileSize * Config::tileSize * 4];
			for(int i = 0; i < Config::tileSize; ++i) {
				for(int j = 0; j < Config::tileSize; ++j) {

					// Get which (floating-point) part of the parent texture we should sample.
					xWithinParent = ((float)i * (parentX2 - parentX1)) + (parentX1 * (float)(Config::tileSize-1));
					zWithinParent = ((float)j * (parentZ2 - parentZ1)) + (parentZ1 * (float)(Config::tileSize-1));

					// Find out the fractional and integer parts of those co-ordinates.
					xF = modf(xWithinParent, &xI);
					zF = modf(zWithinParent, &zI);

					// Instantiate and clamp the four tile co-ordinates.
					i1 = xI; j1 = zI; i2 = i1 + 1; j2 = j1 + 1;
					if (j2 >= Config::tileSize) { j2--; }
					if (i2 >= Config::tileSize) { i2--; }

					// Material should use nearest neighbour sampling (no interpolation)
					data.data[(i*4)+(j*4*Config::tileSize)] = parentData[(i1*4)+(j1*4*Config::tileSize)];

					// TODO: Linearly interpolate the heights
					h11 = (float)((parentData[1+(i1*4)+(j1*4*Config::tileSize)] * 255) + parentData[2+(i1*4)+(j1*4*Config::tileSize)]);
					h12 = (float)((parentData[1+(i1*4)+(j2*4*Config::tileSize)] * 255) + parentData[2+(i1*4)+(j2*4*Config::tileSize)]);
					h21 = (float)((parentData[1+(i2*4)+(j1*4*Config::tileSize)] * 255) + parentData[2+(i2*4)+(j1*4*Config::tileSize)]);
					h22 = (float)((parentData[1+(i2*4)+(j2*4*Config::tileSize)] * 255) + parentData[2+(i2*4)+(j2*4*Config::tileSize)]);

					h = (h11 * (1.0f - xF) * (1.0f - zF)) + (h21 * xF * (1.0f - zF)) 
							+ (h12 * (1.0f - xF) * zF) + (h22 * xF * zF);

					h += noise->getNoise((float)i, (float)j) * 200.0f;

					hf = modf((h / 255.0f), &hi);

					data.data[1+(i*4)+(j*4*Config::tileSize)] = (int)hi;
					data.data[2+(i*4)+(j*4*Config::tileSize)] = (int)(hf * 255.0f);

				}
			}
		}

		// Set the time we were last accessed, and increment
		// the total no. of accesses overall.
		lastAccessed = ++access;

		return &data;
	}

	// Identify which child we should be selecting
	int idx = 0;
	if(x > this->x) idx += 1;
	if(z > this->z) idx += 2;

	// Does the child exist... if not, create it.
	if(NULL == child[idx]) 
	{
		int offset = (int)(((float)Config::tileSize * pow(2.0, this->lod - 2)) * Config::scale * DETAIL_SCALE);
		int childx = this->x - offset;
		int childz = this->z - offset;

		// Set the location of the child, which will always be relative
		// to the parent.
		if(x > this->x) childx += (offset * 2);
		if(z > this->z) childz += (offset * 2);

		child[idx] = new HeightmapTree(childx, childz, this->lod - 1);

		// Negative LODs are special cases as the child will take from the parent tile
		// TODO: this needs more setup, such as a pointer to the parent.
		if(this->lod <= 0) 
		{

			// Set the relevant data from the parent, particularly the pointer
			// to the heightmap

			// If this is a LOD-0 tile, we need to load our data first.
			if((NULL == data.data) && (this->lod == 0))
			{
				this->loadData();
				child[idx]->parentData = data.data;
			}
			else if (this->lod == 0)
			{
				// Data is already loaded
				child[idx]->parentData = data.data;
			}
			else
			{
				child[idx]->parentData = parentData;
			}


			child[idx]->data.maxY = data.maxY;
			child[idx]->data.minY = data.minY;
			child[idx]->data.isSea = data.isSea;

			// Set up the texture window
			if(x > this->x)
			{
				// X start should move half way along the tile
				child[idx]->parentX1 = (parentX1 + parentX2) / 2.0f;
				child[idx]->parentX2 = parentX2;
			}
			else
			{
				// X end should move half way back along the tile
				child[idx]->parentX1 = parentX1;
				child[idx]->parentX2 = (parentX1 + parentX2) / 2.0f;
			}

			if(z > this->z)
			{
				// X start should move half way along the tile
				child[idx]->parentZ1 = (parentZ1 + parentZ2) / 2.0f;
				child[idx]->parentZ2 = parentZ2;
			}
			else
			{
				// X end should move half way back along the tile
				child[idx]->parentZ1 = parentZ1;
				child[idx]->parentZ2 = (parentZ1 + parentZ2) / 2.0f;
			}

		}
	}

	// Now that the child has been created, and we know we are still
	// looking for something at a lower level, we can recurse.
	return child[idx]->getData(x, z, lod);
}