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


#include "TerrainTile.h"

#include <OgreSceneManager.h>
#include <OgreMaterialManager.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreGpuProgramManager.h>
#include <OgreGpuProgramParams.h>
#include "LodMeshManager.h"
#include "Config.h"

TerrainTile::TerrainTile()
{
	// Set default values before loading
	minY = 0; maxY = 0; isSea = false;

	// At first the terrain tile won't be ready
	isReady = false;
}

Ogre::Entity* TerrainTile::getTile()
{
	// Return the terrain object- hopefully it's set up!
	return terrain;
}

void TerrainTile::Load(Ogre::SceneManager* scene, int x, int z, int lod, HeightmapTree* tileCache)
{
	this->x = x;
	this->z = z;
	this->lod = lod;

	Ogre::MeshPtr mesh = LODMeshManager::getMesh();

	// TODO: Do we want to name the object?
	terrain = scene->createEntity(mesh->getName());

	// Set the material of the node.
	setupMaterial(tileCache);
	terrain->setMaterial(material);

	// Now we're ready to display this tile
	isReady = true;
}

void TerrainTile::setupMaterial(HeightmapTree* tileCache)
{
	sprintf_s(materialName, 128, "TlMat_%d_%d_%d", lod, x, z);
	sprintf_s(textureName, 128, "TlTex_%d_%d_%d", lod, x, z);

	material = Ogre::MaterialManager::getSingletonPtr()->create(materialName, "General");

	// Set the heightmap pointer
	HeightmapData* d = tileCache->getData(x, z, lod);
	heightData = (Ogre::uint32*)d->data;

	minY = d->minY;
	maxY = d->maxY;
	isSea = d->isSea;

	// Create the texture
	texture = Ogre::TextureManager::getSingleton().createManual(
		textureName, "General",
		Ogre::TEX_TYPE_2D,
		// TODO: this will change when we get down to sublevels of detail
		Config::tileSize, Config::tileSize,
		1,
		0,
		Ogre::PF_A8R8G8B8,
		Ogre::TU_DYNAMIC_WRITE_ONLY
		);

	// Access the hardware buffer
	Ogre::HardwarePixelBufferSharedPtr pixbuf = texture->getBuffer(0, 0);

	// Lock the buffer for writing
	pixbuf->lock(Ogre::HardwareBuffer::HBL_DISCARD);
	const Ogre::PixelBox &pb = pixbuf->getCurrentLock();

	Ogre::uint32 *pixdata = static_cast<Ogre::uint32*>(pb.data);
	size_t stride = pb.rowPitch;

	for(int i = 0; i < Config::tileSize; ++i ) {
		for(int j = 0; j < Config::tileSize; ++j ) {
			pixdata[i + (j * stride)] = heightData[i + (j * stride)];
		}
	}

	pixbuf->unlock();


	// This makes the following a little less pointer soup
	Ogre::Pass* pass = material->getTechnique(0)->getPass(0);
	// pass->setTextureFiltering(Ogre::TFO_NONE);



	// Bind the texture we just wrote to the material for both vertex and pixel shader
	Ogre::TextureUnitState* texStateV = pass->createTextureUnitState("m_HeightMapV");
	//texStateV->setContentType(Ogre::TextureUnitState::CONTENT_NAMED);
	texStateV->setBindingType(Ogre::TextureUnitState::BT_VERTEX);
	texStateV->setTextureName(textureName);
	texStateV->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	texStateV->setTextureFiltering(Ogre::TFO_NONE);

	//Ogre::TextureUnitState* texStateP = pass->createTextureUnitState("m_HeightMapP");
	//texStateP->setContentType(Ogre::TextureUnitState::CONTENT_NAMED);
	//texStateP->setBindingType(Ogre::TextureUnitState::BT_FRAGMENT);
	//texStateP->setTextureName("landscape.bmp");
	//texStateP->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);

	Ogre::TextureUnitState* texStateColourP = pass->createTextureUnitState("m_ColourMapP");
	texStateColourP->setBindingType(Ogre::TextureUnitState::BT_FRAGMENT);
	texStateColourP->setTextureName("landscape.bmp");
	texStateColourP->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);


	//texStateV->setNumMipmaps(0);

	// Attach the shaders
	pass->setVertexProgram("TerrainVertexShader", true);
	Ogre::GpuProgramParametersSharedPtr vertParams = pass->getVertexProgramParameters();

	// Pass constants from OGRE
	vertParams->setIgnoreMissingParams(true);
	vertParams->setNamedAutoConstant("g_WorldViewProjectionMatrix", Ogre::GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
	// No longer used
	//vertParams->setNamedConstant("m_Scale", (maxY-minY));
	
	pass->setFragmentProgram("TerrainFragmentShader", true);

	// Weird OGRE feature/buglet - setting the name of the textures is
	// not enough, we also need to assign their order before they
	// will work in the fragment shader.
	Ogre::GpuProgramParametersSharedPtr fragParams = pass->getFragmentProgramParameters();
	fragParams->setNamedConstant("m_HeightMapV", 0);
	fragParams->setNamedConstant("m_ColourMapP", 1);
	fragParams->setNamedConstant("m_TileScale", pow(2.0f, (float)lod));


}

void TerrainTile::Unload(Ogre::SceneManager* scene)
{
	//scene->destroyEntity(terrain);
}