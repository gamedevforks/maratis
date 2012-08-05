/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MGame.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#include "../Includes/MEngine.h"


static MOCamera * getCurrentCamera(MScene * scene)
{
	if(scene->getCamerasNumber() > 0)
	{
		unsigned int currentCamera = scene->getCurrentCamera();
		if(currentCamera < scene->getCamerasNumber())
			return scene->getCameraByIndex(currentCamera);
		else
			return scene->getCameraByIndex(0);
	}
	
	return NULL;
}


void MGame::update(void)
{
	MEngine * engine = MEngine::getInstance();
	MScriptContext * scriptContext = engine->getScriptContext();

	// update script
	if(scriptContext)
		scriptContext->callFunction("onSceneUpdate");

	// get level
	MLevel * level = MEngine::getInstance()->getLevel();
	if(! level)
		return;

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
		return;

	// update behaviors
	scene->updateObjectsBehaviors();

	// update scene
	scene->update();

	// update physics
	scene->updatePhysics();

	// update objects matrices
	scene->updateObjectsMatrices();

	// update scene layer
	MOCamera * camera = getCurrentCamera(scene);
	if(camera)
	{
		unsigned int sceneLayerId = camera->getSceneLayer();
		if(sceneLayerId > 0 && sceneLayerId <= level->getScenesNumber())
		{
			MScene * sceneLayer = level->getSceneByIndex(sceneLayerId-1);
			
			sceneLayer->updateObjectsBehaviors();
			sceneLayer->update();
			sceneLayer->updateObjectsMatrices();
		}
	}
	
	// flush input
	engine->getInputContext()->flush();

	// update postponed requests
	engine->updateRequests();
}

void MGame::draw(void)
{
	MRenderingContext * render = MEngine::getInstance()->getRenderingContext();

	// get level
	MLevel * level = MEngine::getInstance()->getLevel();
	if(! level)
		return;

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
		return;

	// clear buffer
	render->enableDepthTest();

	if(scene->getCamerasNumber() == 0)
	{
		// draw scene with default camera
		MOCamera camera;
		
		render->setClearColor(*camera.getClearColor());
		render->clear(M_BUFFER_COLOR | M_BUFFER_DEPTH);

		camera.enable();
		camera.updateListener();
		scene->draw(&camera);
		scene->drawObjectsBehaviors();
	}
	else
	{
		MOCamera * camera = getCurrentCamera(scene);
		
		render->setClearColor(*camera->getClearColor());
		render->clear(M_BUFFER_COLOR | M_BUFFER_DEPTH);

		camera->enable();
		camera->updateListener();
		scene->draw(camera);
		scene->drawObjectsBehaviors();
		
		// scene layer
		unsigned int sceneLayerId = camera->getSceneLayer();
		if(sceneLayerId > 0 && sceneLayerId <= level->getScenesNumber())
		{
			MScene * sceneLayer = level->getSceneByIndex(sceneLayerId-1);
			MOCamera * layerCamera = getCurrentCamera(sceneLayer);
			if(layerCamera)
			{
				layerCamera->enable();
				camera = layerCamera;
			}
			
			// draw on top
			render->clear(M_BUFFER_DEPTH);
			sceneLayer->draw(camera);
			sceneLayer->drawObjectsBehaviors();
		}
	}
}

void MGame::onBeginScene(void)
{
	// get level
	MLevel * level = MEngine::getInstance()->getLevel();
	if(! level)
		return;

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
		return;

	// begin scene
	scene->begin();
}

void MGame::onEndScene(void)
{
	MEngine * engine = MEngine::getInstance();
	engine->getPhysicsContext()->clear();

	// get level
	MLevel * level = MEngine::getInstance()->getLevel();
	if(! level)
		return;

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
		return;

	// end scene
	scene->end();
}