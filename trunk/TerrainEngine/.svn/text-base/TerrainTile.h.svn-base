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

#ifndef H_TERRAIN_TILE

#define H_TERRAIN_TILE

#include <OgreEntity.h>
#include <OgreSceneNode.h>
#include <OgrePlatform.h>
#include <OgreTexture.h>

#include "HeightmapTree.h"


class TerrainTile {
public:
	// Const/destruct.
	TerrainTile();
	~TerrainTile();

	bool isReady;

	// Terrain metadata
	float minY;
	float maxY;

	bool isSea;

	void Load(Ogre::SceneManager*, int, int, int, HeightmapTree*);
	void Unload(Ogre::SceneManager*);

	Ogre::Entity* getTile();

protected:
	
	// Location/lod
	int x;
	int z;
	int lod;

	// Material name
	char materialName[128];
	char textureName[128];

	// Object used for the terrain tile
	Ogre::Entity* terrain;

	// Material used for the terrain tile
	Ogre::MaterialPtr material;

	// Texture
	Ogre::TexturePtr texture;

	// Source data for the heightmap
	Ogre::uint32* heightData;

	void setupMaterial(HeightmapTree*);



};

#endif