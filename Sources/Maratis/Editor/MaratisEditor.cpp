/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MaratisEditor.cpp
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


#include <MEngine.h>
#include <MGui.h>
#include <Loaders/Mesh/MMeshLoad.h>
#include "MaratisEditor.h"


MaratisEditor::MaratisEditor(void)
{
	// view entities
	/*m_sphereEntity = new MOEntity	(loadEditorMesh("gui/meshs/sphere.mesh"));
	m_coneEntity = new MOEntity		(loadEditorMesh("gui/meshs/cone.mesh"));
	m_cubeEntity = new MOEntity		(loadEditorMesh("gui/meshs/box.mesh"));
	m_planeEntity = new MOEntity	(loadEditorMesh("gui/meshs/plane.mesh"));
	m_xEntity = new MOEntity		(loadEditorMesh("gui/meshs/x.mesh"));
	m_yEntity = new MOEntity		(loadEditorMesh("gui/meshs/y.mesh"));
	m_zEntity = new MOEntity		(loadEditorMesh("gui/meshs/z.mesh"));
	m_xcircleEntity = new MOEntity	(loadEditorMesh("gui/meshs/xcircle.mesh"));
	m_ycircleEntity = new MOEntity	(loadEditorMesh("gui/meshs/ycircle.mesh"));
	m_zcircleEntity = new MOEntity	(loadEditorMesh("gui/meshs/zcircle.mesh"));
	m_lightEntity = new MOEntity	(loadEditorMesh("gui/meshs/light.mesh"));
	m_cameraEntity = new MOEntity	(loadEditorMesh("gui/meshs/camera.mesh"));
	m_soundEntity = new MOEntity	(loadEditorMesh("gui/meshs/sound.mesh"));*/
}

MaratisEditor::~MaratisEditor(void)
{
	// view entities
	SAFE_DELETE(m_sphereEntity);
	SAFE_DELETE(m_coneEntity);
	SAFE_DELETE(m_cubeEntity);
	SAFE_DELETE(m_planeEntity);
	SAFE_DELETE(m_xEntity);
	SAFE_DELETE(m_yEntity);
	SAFE_DELETE(m_zEntity);
	SAFE_DELETE(m_xcircleEntity);
	SAFE_DELETE(m_ycircleEntity);
	SAFE_DELETE(m_zcircleEntity);
	SAFE_DELETE(m_lightEntity);
	SAFE_DELETE(m_cameraEntity);
	SAFE_DELETE(m_soundEntity);
}
/*
MMeshRef * MaratisEditor::loadEditorMesh(const char * filename)
{
	MMesh * mesh = MMesh::getNew();
	if(xmlMeshLoad(filename, mesh))
	{
		MMeshRef * ref = MMeshRef::getNew(mesh, filename);
		m_meshManager.addRef(ref);
		return ref;
	}

	mesh->destroy();
	return NULL;
}*/