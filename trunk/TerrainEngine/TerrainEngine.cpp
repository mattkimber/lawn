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

#include "TerrainEngine.h"
#include "Config.h"


#include <OgreWindowEventUtilities.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreHighLevelGpuProgram.h>



// Constructor/destructor
TerrainEngine::TerrainEngine()
{
	frame = 0;


}

TerrainEngine::~TerrainEngine()
{

}

void TerrainEngine::start()
{


	new OgreFramework();

	if(!OgreFramework::getSingletonPtr()->initOgre("Terrain Engine", this, 0))
		return;


	bShutdown = false;

	setup();
	run();
}

void TerrainEngine::setup()
{
	// Set up the scene here
	Config::load();
	terrainTree = new TerrainQuadtree();

	//Ogre::Entity* et = OgreFramework::getSingletonPtr()->m_pSceneMgr->createEntity("cubester", Ogre::SceneManager::PT_CUBE);
	//Ogre::SceneNode* sc = OgreFramework::getSingletonPtr()->m_pSceneMgr->getRootSceneNode()->createChildSceneNode();

	// Prefab cube is 100x100x100
	// Set it to the size of a typical car
	//sc->setScale(0.01813f, 0.01428f, 0.04576f);
	//sc->setPosition(0.0f, 1.0f, 0.0f);
	//sc->attachObject(et);

	// Tell the terrain quadtree which scene manager to use
	terrainTree->setSceneManager(OgreFramework::getSingletonPtr()->m_pSceneMgr);
	terrainTree->setTerrainQuadtree();

	// Load the shaders
	Ogre::HighLevelGpuProgramPtr terrainVertProg = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		"TerrainVertexShader", "General", "glsl", Ogre::GPT_VERTEX_PROGRAM);
	Ogre::HighLevelGpuProgramPtr terrainFragProg = Ogre::HighLevelGpuProgramManager::getSingleton().createProgram(
		"TerrainFragmentShader", "General", "glsl", Ogre::GPT_FRAGMENT_PROGRAM);

	terrainVertProg->setSourceFile("LawnTerrain.vert");
	terrainFragProg->setSourceFile("LawnTerrain.frag");

	terrainVertProg->setParameter("uses_vertex_texture_fetch","true");

	terrainVertProg->load();
	terrainFragProg->load();

	// Load the landscape textures
	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().load("landscape.bmp", "General");
	Ogre::String s = texture->getName();

	// Set camera location
	OgreFramework::getSingletonPtr()->m_pCamera->setPosition(0.0f, 50.0f, 0.0f);

}

void TerrainEngine::run()
{
	OgreFramework::getSingletonPtr()->m_pLog->logMessage("Firing up...");

	double timeSinceLastFrame = 0;
	double startTime = 0;

	OgreFramework::getSingletonPtr()->m_pRenderWnd->resetStatistics();
	
	while(!bShutdown && !OgreFramework::getSingletonPtr()->isOgreToBeShutDown()) 
	{
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isClosed())bShutdown = true;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_LINUX
		Ogre::WindowEventUtilities::messagePump();
#endif	
		if(OgreFramework::getSingletonPtr()->m_pRenderWnd->isActive())
		{
			startTime = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU();

			// Walk the terrain quadtree with the camera's current position.
			terrainTree->walk(OgreFramework::getSingletonPtr()->m_pCamera->getPosition());
					
			OgreFramework::getSingletonPtr()->m_pKeyboard->capture();
			OgreFramework::getSingletonPtr()->m_pMouse->capture();

			OgreFramework::getSingletonPtr()->updateOgre(timeSinceLastFrame);
			OgreFramework::getSingletonPtr()->m_pRoot->renderOneFrame();
		
			timeSinceLastFrame = OgreFramework::getSingletonPtr()->m_pTimer->getMillisecondsCPU() - startTime;
		}
		else
		{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            Sleep(1000);
#else
            sleep(1);
#endif
		}
	}

		OgreFramework::getSingletonPtr()->m_pLog->logMessage("Shutting down...");

}


bool TerrainEngine::keyPressed(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyPressed(keyEventRef);
	
	if(OgreFramework::getSingletonPtr()->m_pKeyboard->isKeyDown(OIS::KC_F))
	{
		 //do something
	}

	return true;
}

bool TerrainEngine::keyReleased(const OIS::KeyEvent &keyEventRef)
{
	OgreFramework::getSingletonPtr()->keyReleased(keyEventRef);
	
	return true;
}