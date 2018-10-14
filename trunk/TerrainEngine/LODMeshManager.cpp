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

#include "LODMeshManager.h"
#include "math.h"
#include "Config.h"

#include <OgreMeshManager.h>
#include <OgreHardwareBufferManager.h>
#include <OgreSubMesh.h>

#define SKIRTS true

Ogre::MeshPtr LODMeshManager::mesh = (Ogre::MeshPtr)NULL;

Ogre::MeshPtr LODMeshManager::getMesh()
{
	// TODO: Check for null referencing when this hasn't
	// been instantiated - does new() cover it?
	if(!mesh.isNull()) { return mesh; }

	// Mesh didn't exist - we need to create it.
	int meshSize = Config::tileSize;
	int meshSizeScaled = meshSize * (float)Config::scale;

	
	mesh = Ogre::MeshManager::getSingleton().createManual("TerTile", "General");

	// Create the submesh - is this needed?
	// Ogre::SubMesh* submesh = mesh[subdivision]->createSubMesh();

	// Create the vertex data.
	Ogre::VertexData* data = new Ogre::VertexData();

	mesh->sharedVertexData = data;
	data->vertexCount = meshSize * meshSize;

	// Create the vertex declaration
	Ogre::VertexDeclaration* decl = data->vertexDeclaration;

	// Get the vertices
	// TODO: add skirts to the full size tiles
	getVertices(decl, data, meshSize, SKIRTS);

	// Now get the indices.
	// First create a submesh for the index data to go in
	Ogre::SubMesh* subMesh = mesh->createSubMesh();
	subMesh->useSharedVertices = true;

	// Get the indices to the submesh
	getIndices(subMesh, meshSize, SKIRTS);

	// Set the bounding box
	mesh->_setBounds(Ogre::AxisAlignedBox(-meshSizeScaled/2, -10.0f, -meshSizeScaled/2, meshSizeScaled/2, Config::tileBBoxHeight, meshSizeScaled/2));

	// Load the mesh
	mesh->load();

	// Now the mesh is created we can return it.
	return mesh;
}

// Method to populate the indices
void LODMeshManager::getIndices(Ogre::SubMesh* mesh, int meshSize, bool skirts)
{
	int idx = 0;

	int pts = meshSize - 1;
	if (skirts) {
		pts+=2;
		meshSize+=2;
	}

	unsigned short* indexData = new unsigned short[pts*pts*6];

	for(unsigned short i = 0; i < pts; ++i) {
		for(unsigned short j = 0; j < pts; ++j) {
			if(!( (skirts) && (
				( (i == 0) && (j==0) ) ||
				( (i == 0) && (j==(pts-1)) ) ||
				( (i ==(pts-1)) && (j==0) ) ||
				( (i == (pts-1)) && (j == (pts - 1) ) ) ) ) )
			{
				
					
					// Triangle 1
					indexData[idx++] = (i + (j * meshSize));
					indexData[idx++] = ((i+1) + (j * meshSize));
					indexData[idx++] = ((i+meshSize) + (j * meshSize));
					
					// Triangle 2
					indexData[idx++] = ((i+1) + (j * meshSize));
					indexData[idx++] = ((i+1+meshSize) + (j * meshSize));
					indexData[idx++] = ((i+meshSize) + (j * meshSize));
				
			}
		}
	}


	//mesh->operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;

	Ogre::HardwareIndexBufferSharedPtr ibuf = 
		Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT,
		idx, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	//ibuf->lock(Ogre::HardwareBuffer::LockOptions::HBL_DISCARD);
	ibuf->writeData(0, ibuf->getSizeInBytes(), indexData, true);
	//ibuf->unlock();

	mesh->indexData->indexBuffer = ibuf;
	mesh->indexData->indexCount = idx;
	mesh->indexData->indexStart = 0;

}

// Method to populate the mesh
void LODMeshManager::getVertices(Ogre::VertexDeclaration* decl, Ogre::VertexData* data, int meshSize, bool skirts)
{
	// Used for the offset storage
	size_t offset = 0;

	// We add texture co-ordinates and vertex positions.  Why just those two?
	// Well, everything is done in a shader... material mapping, normal mapping, 
	// even the heights of the vertices.  That way we can repeat just one tile
	// across all the different sections of terrain.
	decl->addElement(0, offset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	decl->addElement(0, offset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
	offset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);

	// Some info about tile dimensions
	float tileOffset = (float)meshSize / 2;
	float tileMultiplier = (float)meshSize / (float)(meshSize - 1);

	int idx = 0;

	int start = 0; int end = meshSize;
	if (skirts)
	{
		// Add a row/col for the skirts
		start = -1; end = meshSize + 1;
	}

	int meshDataSize = (end - start);

	// The vertex data
	float* vertexData;
	vertexData = new float[meshDataSize*meshDataSize*offset];

	float x, y, z, u, v;
	float skirtSize = 1.0f;

	// Now create the array of vertices and texture co-ords!
	for(int i = start; i < end; ++i)
	{
		for(int j = start; j < end; ++j)
		{
			x = (((float)i * tileMultiplier) - tileOffset);
			z = (((float)j * tileMultiplier) - tileOffset);
			y = 0.0f;
			u = (float)i / (float)(meshSize - 1);
			v = (float)j / (float)(meshSize - 1);

			if(i == -1)
			{
				y = -skirtSize;
				u = 0; x = -tileOffset;
			}
			if(j == -1)
			{
				y = -skirtSize;
				v = 0; z = -tileOffset;
			}
			if(i == meshSize)
			{
				y = -skirtSize;
				u = 1.0f; x = ((float)(meshSize - 1) * tileMultiplier) - tileOffset;
			}
			if(j == meshSize)
			{
				y = -skirtSize;
				v = 1.0f; z = ((float)(meshSize - 1) * tileMultiplier) - tileOffset;
			}

			// Vertices
			vertexData[idx++] = x;
			vertexData[idx++] = y;
			vertexData[idx++] = z;

			// Texture co-ordinates
			vertexData[idx++] = u;
			vertexData[idx++] = v;
		}
	}


	// Create the hardware vertex buffer
	Ogre::HardwareVertexBufferSharedPtr vbuf = 
		Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(offset, 
		meshDataSize * meshDataSize, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY);

	// Link the buffer to the mesh
	Ogre::VertexBufferBinding* binding = data->vertexBufferBinding;
	binding->setBinding(0, vbuf);

	//vbuf->lock(Ogre::HardwareBuffer::LockOptions::HBL_DISCARD);
	// Write the vertex data
	vbuf->writeData(0, vbuf->getSizeInBytes(), vertexData, true);
	// Commit
	//vbuf->unlock();



	// Free memory
	delete [] vertexData;

}

