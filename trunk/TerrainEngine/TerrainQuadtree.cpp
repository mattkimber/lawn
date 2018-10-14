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

#include "TerrainQuadtree.h"
#include "TerrainTile.h"

#include "Config.h"
#include "math.h"


// Constants
int TerrainQuadtree::lowestLOD = 1;

Ogre::SceneManager* TerrainQuadtree::scene = NULL;
Ogre::SceneNode* TerrainQuadtree::terrainNode = NULL;
HeightmapTree* TerrainQuadtree::heightmapTree = NULL;

TerrainQuadtree::TerrainQuadtree()
{
	maxY = 10.0;
	minY = 0.0;

	// Set the tile to nothing
	tile = NULL;

	// Set the node to nothing
	tileNode = NULL;

	// Details about what we're doing
	hasGeometry = false;
	hasGeometryBelow = false;
	isSplitting = false;
	isMerging = false;

	x = 0;
	z = 0;

	hasParent = false;



	childrenCannotSplit = false;

	// The number of levels in the quadtree gives
	// us the uppermost level of detail.
	lod = Config::quadtreeLevels + Config::minLOD;

	scale = pow(2.0, lod) * (float)Config::scale;

	isSea = false;
	canSplit = true;

	recalculateBoundingBox();

	// Zero out the children
	for(int i = 0; i < 4; ++i)
	{
		child[i] = NULL;
	}
}


TerrainQuadtree::~TerrainQuadtree()
{
	for(int i = 0; i < 4; ++i)
	{
		if(child[i] != NULL)
		{
			free(child[i]);	
		}
	}
}


// Walk the tree from this point
void TerrainQuadtree::walk(Ogre::Vector3 loc)
{
	// The lowest LOD encountered will gradually increase
	// if the terrain quadtree doesn't have any detailed
	// tiles.
	if(!hasParent) { TerrainQuadtree::lowestLOD++; }
	else if (lod < lowestLOD) { lowestLOD = lod; }

	// Check if we're outside the current lod sphere
	// (If we are, then this is the appropriate detail level to show)
	bool outsideLODSphere = !isInLODSphere(LODSphereSize, loc);

	// Check for any of the following conditions:
	// 1. We've reached the desired LOD with this tile.  (i.e. pixel error sufficiently small)
	// 2. This is the lowest allowable level.
	// 3. We can't split the children for whatever reason.  (e.g. supposed to be distance tiles)
	if( outsideLODSphere || (Config::minLOD == lod) || childrenCannotSplit )
	{
		// Check that we are low enough to display geometry.
		// The rationale here is that when we get in really close
		// (say we're 1cm above the ground) we don't want to still
		// be trying to display mountain ranges from half a country
		// away, even though we still walk the tree past them.

		if(lod <= lowestLOD + Config::lodStack)
		{
			if((!hasGeometry) && (!isSplitting))
			{
				requestSplit();
			}


			// We should request that the children merge, as they are no longer
			// required
			//
			// This needs to check that we have geometry at THIS level
			// (i.e. isSplitReady is TRUE so we can replace the child geom
			//  with the one at this level).
			if(isSplitReady())
			{
				// Deal with the children.
				if(hasChildren())
				{
					for(int i = 0; i < 4; ++i)
					{
						// This should tell the child to merge all its children too
						child[i]->requestMerge(true);
					}
				}

				// Handle our split
				doSplit();
			}
		}
	}
	else
	{
		// If we don't have any children we will need to (try to)
		// create them.  It's possible that the children won't be
		// walkable (in other words, don't have any tiles available
		// for their lower levels) in which case the canSplit
		// variable will be set to false (checked later).
		if(!hasChildren())
		{
			// Calculate an offset value which is the amount by which
			// the centre of each child tile must be offset from the parent
			//
			// Note the -2 in the pow() function - this is an optimisation of
			// the whole expression being divided by 2.
			float offset = (((float)Config::tileSize * pow(2.0, lod - 2.0)) * (float)Config::scale);

			for(int i = 0; i < 2; ++i) {
				for(int j = 0; j < 2; ++j) {
					// Calculate an index value
					int idx = i + (j * 2);

					// Create the child, with a link to the parent.
					child[idx] = new TerrainQuadtree();
					child[idx]->hasParent = true;
					child[idx]->lod = lod - 1;

					// Children are half the size of the parent
					child[idx]->scale = scale / 2.0f;

					// Set its location relative to us (the parent)
					// Note this means each level only needs to know its
					// own location in order to create its children in the
					// right place.
					child[idx]->x = (x - (float)offset + ((float)i * offset * 2.0f));
					child[idx]->z = (z - (float)offset + ((float)j * offset * 2.0f));

					// At this point children inherit the min/max Y of their parent.
					// This is to give some sensibleness to their LOD bounding box
					// calculations before they get their own values (think about
					// what happens when splitting tiles at the top of Everest!)
					child[idx]->maxY = maxY; child[idx]->minY = minY;

					// As the child's geometry has changed it needs to recalculate
					// its bounding box.
					child[idx]->recalculateBoundingBox();

				}
			}

		}

		bool allChildrenHaveGeometry = true;

		// Walk the children.
		for(int i = 0; i < 4; ++i)
		{
			child[i]->walk(loc);

			if(!child[i]->canSplit) 
			{
				// Set the variable that says children cannot
				// split, ever.
				childrenCannotSplit = true;

				// TODO: investigate whether we need to pre-emptively
				// call the request split/merge functions here to avoid
				// the valid-child split requests clogging up the queue
				// when it could be used for better things.
				break;
			}

			if(!child[i]->hasGeometryBelow) allChildrenHaveGeometry = false;
		}

		// If all children are populated, we can now merge.
		if (allChildrenHaveGeometry) 
		{
			hasGeometryBelow = true;
			requestMerge(false);
		}
	
	}

	// Now we have traversed all the children we know what
	// the lowest LOD encountered is.  In which case we can
	// check against it, and request our own merge operation
	// if this tile isn't needed.
	if(lod > (lowestLOD + Config::lodStack)) requestMerge(false);



}

// Algorithm from "On Faster Sphere-Box Overlap Testing"
// Thomas Larsson, Tomas Akenine-Moeller, Eric Lengyel
bool TerrainQuadtree::isInLODSphere(float radius, Ogre::Vector3 loc)
{
	float dist = 0;
	float e;

	// I unrolled the x,y,z cases
	e = std::max(aabbMin.x - loc.x, 0.0f) + std::max(loc.x - aabbMax.x, 0.0f);
	// Referenced paper has this the wrong way round in their QRI section...
	if(e > radius) return false;
	dist += (e * e);

	e = std::max(aabbMin.y - loc.y, 0.0f) + std::max(loc.y - aabbMax.y, 0.0f);
	if(e > radius) return false;
	dist += (e * e);

	e = std::max(aabbMin.z - loc.z, 0.0f) + std::max(loc.z - aabbMax.z, 0.0f);
	if(e > radius) return false;
	dist += (e * e);

	if(dist <= (radius * radius)) return true;

	return false;

}


// Fast log2 calculation
// Based on a suggestion by Michael Herf
// Requires an IEEE floating point architecture
inline int fastLog2(float val)
{
	Ogre::uint32 ival = (Ogre::uint32&)val;
	return (int)((ival >> 23) & 0xFF) - 127;
}


// Deal with splitting
void TerrainQuadtree::requestSplit()
{
	isSplitting = true;

	// Create the tile object if necessary
	if(NULL == tile) {
		tile = new TerrainTile();
	}

	// Load the tile.  One day this will be done
	// in a separate thread so we can go back to
	// rendering while our scenes are building.
	tile->Load(scene, x * DETAIL_SCALE, z * DETAIL_SCALE, lod, heightmapTree);

}

bool TerrainQuadtree::isSplitReady()
{
	if(NULL != tile) {
		return tile->isReady;
	}

	// If there is no tile then we are not ready to split.
	return false;
}

void TerrainQuadtree::doSplit()
{
	// get metadata
	minY = tile->minY;
	maxY = tile->maxY;
	isSea = tile->isSea;

	// Instantiate the geometry with all 
	// texture maps
	if(NULL == tileNode) {
		tileNode = terrainNode->createChildSceneNode();

		tileNode->setPosition(x, minY, z);

		tileNode->setScale(scale, (maxY-minY), scale);

		tileNode->detachAllObjects();
	} 
	else
	{
		// Remove everything currently attached
		tileNode->detachAllObjects();
	}

	tileNode->attachObject(tile->getTile());

	isSplitting = false;
	hasGeometry = true;
	hasGeometryBelow = true;
}

// Deal with merging
void TerrainQuadtree::requestMerge(bool mergeChildren)
{
	// TODO: will this always be fast enough to do in-thread?
	isMerging = true;

	// If we have children they need to merge too
	if(hasChildren())
	{
		hasGeometryBelow = false;

		for(int i = 0; i < 4; ++i)
		{
			if(mergeChildren) child[i]->requestMerge(true);
			else if ( child[i]->hasGeometryBelow ) hasGeometryBelow = true;
		}
	}


	// Remove all objects and detach the tile
	if(NULL != tileNode) tileNode->detachAllObjects();

	// If there's no tile, then no reason to merge
	if(NULL == tile) {
		isMerging = false;
		return;
	}

	tile->Unload(scene);

	// No longer merging or splitting
	isMerging = false;
	isSplitting = false;
	hasGeometry = false;

}


// Tracks if children have been created
bool TerrainQuadtree::hasChildren()
{
	// We either have all four children or none, so it's okay
	// to only test the first.
	return ( child[0] != NULL );
}

// Recalculate the bounding box
void TerrainQuadtree::recalculateBoundingBox()
{
	// -1 to the lod as we actually divide by 2.
	float thisLevelSize = Config::scale * (int)((float)Config::tileSize  * pow(2.0f, (float)lod - 1));
	LODSphereSize = (float)Config::scale * ((float)Config::tileSize  * pow(2.0f, (float)lod - Config::errorBias));

	// Store the origin in the origin vector
	Ogre::Vector3 origin, extents;
	origin.x = x;
	origin.y = (maxY - minY) / 2;
	origin.z = z;

	// Set the extents
	extents.x = thisLevelSize;
	extents.y = (maxY + minY) / 2;
	extents.z = thisLevelSize;

	// Set the AABB.
	aabbMin = origin - extents; aabbMax = origin + extents;
}

// Sets the scene manager
void TerrainQuadtree::setSceneManager(Ogre::SceneManager* scn)
{
	// Set the pointer
	scene = scn;
	
	// Create a node under the scene manager
	terrainNode = scene->getRootSceneNode()->createChildSceneNode();
}

// Set up the terrain quadtree
void TerrainQuadtree::setTerrainQuadtree()
{
	heightmapTree = new HeightmapTree(x, z, lod);
}