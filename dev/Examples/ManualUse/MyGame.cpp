/////////////////////////////////////////////////////////////////////////////////////////////////////////
// ManualUse example
// MyGame.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
//  Maratis, Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//========================================================================

// This game example is showing how to manually create a scene and load objects
// it can also be used in Maratis as game plugin (like WaterGameDemo)


#include "MyGame.h"
#include <MLog.h>

MyGame::MyGame(void):
MGame()
{
    MLOG(6, "new MyGame...");
}

MyGame::~MyGame(void)
{
}

void MyGame::onBegin(void)
{
	MEngine * engine = MEngine::getInstance();
	MSystemContext * system = engine->getSystemContext();

	// get level
	MLevel * level = engine->getLevel();
	if(! level)
	{
	    MLOG(4, "Cannot get level from engine");
	    return;
	}

	// create a scene manually
	if(level->getScenesNumber() == 0)
	{
		char filename[256];
		const char * workingDir = system->getWorkingDirectory();

		// add scene
		MScene * scene = level->addNewScene();
        if (!scene)
            MLOG(4, "Failed to add a new scene");
		// camera
		MOCamera * camera = scene->addNewCamera();
		if (!camera)
            MLOG(4, "Failed to add a new camera");
		camera->setClearColor(MVector3(0.5f, 0.5f, 0.5f)); // set grey clear color
		camera->setPosition(MVector3(0.0f, -80.0f, 20.0f));
		camera->setEulerRotation(MVector3(90.0f, 0.0f, 0.0f));

		// add robot entity
		getGlobalFilename(filename, workingDir, "meshs/robot.mesh");
		MMeshRef * meshRef = level->loadMesh(filename);
		if (!meshRef)
            MLOG(4, "cannot load mesh robot");
		MOEntity* re=scene->addNewEntity(meshRef);
        if (!re)
            MLOG(4, "Cannot add Robot entity to scene");

		// add cubes with physics
		{
			// create entities
			getGlobalFilename(filename, workingDir, "meshs/box.mesh");
			meshRef = level->loadMesh(filename);
			if (!meshRef)
                MLOG(4, "Cannot load box mesh")
            else
                MLOG(5, "Box mesh loaded: "<<filename);

			MOEntity * box1 = scene->addNewEntity(meshRef);
			MOEntity * box2 = scene->addNewEntity(meshRef);
			MOEntity * box3 = scene->addNewEntity(meshRef);

			// set coords
			box1->setPosition(MVector3(70, 65, 0));
			box1->setScale(MVector3(4, 4, 0.2f));

			box2->setPosition(MVector3(70, 65, 40));
			box2->setEulerRotation(MVector3(0, -35, 0));

			box3->setPosition(MVector3(75, 65, 70));

			// enable physics, MPhysicsProperties create a static box shape by default
			MPhysicsProperties * phyProps = box1->createPhysicsProperties();
            if (!phyProps)
                MLOG(4, "Cannot create phys prop from box");

			phyProps = box2->createPhysicsProperties();
			phyProps->setMass(1);

			phyProps = box3->createPhysicsProperties();
			phyProps->setMass(1);
		}

		// add text
		getGlobalFilename(filename, workingDir, "fonts/Gentium102/GenR102.TTF");
		MFontRef * fontRef = level->loadFont(filename);
		MOText * text = scene->addNewText(fontRef);
		text->setPosition(MVector3(0, 0, 40));
		text->setEulerRotation(MVector3(-90, 0, 0));
		text->setText("Hello");
		text->setSize(8);
		text->setAlign(M_ALIGN_CENTER);

		// add light
		MOLight * light = scene->addNewLight();
		if (!light)
            MLOG(4, "Cannot add light");
		light->setPosition(MVector3(0.0f, 0.0f, 100.0f));
		light->setRadius(1000.0f);
	}
}

void MyGame::update(void)
{
	MEngine * engine = MEngine::getInstance();
	MInputContext * input = engine->getInputContext();

	MGame::update(); // call standard update, or replace it with cutom code

	// get level
	MLevel * level = engine->getLevel();
	if(! level)
	{
	    MLOG(4, "Cannot get level from engine");
	    return;
	}

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
	{
	    MLOG(4, "Cannot get scene from level");
	    return;
	}

	// rotate the entity
	MOEntity * entity = scene->getEntityByIndex(0);
	if (entity)
        entity->addAxisAngleRotation(MVector3(0.0f, 0.0f, 1.0f), 1.0f);
    else
        MLOG(4, "Cannot retrieve entity 0");

	// change light intensity with keyboard
	MOLight * light = scene->getLightByIndex(0);

	if(light && input->isKeyPressed("UP"))
	{
	    MLOG(5, "Pumping up light intensity: "<<light->getIntensity());
	    // inputs are also virtual, you can create your hown keys or axis
		light->setIntensity(light->getIntensity() + 0.1f);
	}

	if(light && input->isKeyPressed("DOWN"))
	{
	    MLOG(5, "Decreasing light intensity: "<<light->getIntensity())
		light->setIntensity(MAX(0.0f, light->getIntensity() - 0.1f));
	}
}

/*
// custom draw, if not defined, draw is MGame::draw()
void MyGame::draw(void)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();

	// get level
	MLevel * level = engine->getLevel();
	if(! level)
		return;

	// get current scene
	MScene * scene = level->getCurrentScene();
	if(! scene)
		return;

	render->enableDepthTest();

	MOCamera * camera = scene->getCameraByIndex(0);

	render->setClearColor(*camera->getClearColor());
	render->clear(M_BUFFER_COLOR | M_BUFFER_DEPTH);

	camera->enable();
	camera->updateListener();
	scene->draw(camera);
}
*/
