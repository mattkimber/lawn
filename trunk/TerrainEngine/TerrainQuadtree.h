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

#ifndef H_TERRAIN_QUADTREE
#define H_TERRAIN_QUADTREE

#include <OgreVector3.h>
#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

#include "TerrainTile.h"
#include "HeightmapTree.h"

// Class for the terrain quadtree
class TerrainQuadtree {
public:

	// Used for the parent
	TerrainQuadtree();

	// Used for creating child nodes.
	TerrainQuadtree(TerrainQuadtree*);
	~TerrainQuadtree();

	void setSceneManager(Ogre::SceneManager*);
	void setTerrainQuadtree();

	// Walk the tree from this point
	void walk(Ogre::Vector3);



protected:

	// Scene manager
	static Ogre::SceneManager* scene;

	// Terrain node
	static Ogre::SceneNode* terrainNode;

	// Terrain heightmap quadtree
	static HeightmapTree* heightmapTree;

	Ogre::SceneNode* tileNode;

	// Children
	TerrainQuadtree* child[4];

	// Parent
	bool hasParent;

	// If we have geometry
	bool hasGeometry;
	bool hasGeometryBelow;
	bool isSplitting;
	bool isMerging;

	// Level of detail
	int lod;

	// Bounding box (used in visibility algos)
	Ogre::Vector3 aabbMin;
	Ogre::Vector3 aabbMax;

	// Size of the bounding sphere for the associated LOD
	float LODSphereSize;

	// Height-of-tile values used when creating a tile
	float maxY;
	float minY;

	// Tile size
	float scale;

	// Co-ordinates of this tile
	float x;
	float z;

	// Used to speed up walk operations
	bool childrenCannotSplit;

	// Keep track of our current lowest LOD - how high the "stack" is allowed to get is set in config.
	static int lowestLOD;

	// Sea tiles are treated specially
	bool isSea;

	// Can we split?
	bool canSplit;

	// Methods

	// Get distance of camera to edge of bounding box.
	float getDistance(Ogre::Vector3);

	// Check if the tile bbox and LOD sphere intersect
	bool isInLODSphere(float, Ogre::Vector3);

	// Calculate the desired level of detail for this tile.
	int desiredLOD(Ogre::Vector3);

	// Deal with splitting
	void requestSplit();
	bool isSplitReady();
	void doSplit();

	// Deal with merging
	void requestMerge(bool);


	// Tracks if children have been created
	bool hasChildren();

	// Recalculate the bounding box
	void recalculateBoundingBox();

	// The tile
	TerrainTile* tile;

};

#endif