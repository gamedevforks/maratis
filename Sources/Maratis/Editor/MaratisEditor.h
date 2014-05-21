/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MaratisEditor.h
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


#ifndef _MARATIS_EDITOR_H
#define _MARATIS_EDITOR_H

enum M_AXIS
{
	M_AXIS_NONE = 0,
	M_AXIS_X,
	M_AXIS_Y,
	M_AXIS_Z,
	M_AXIS_VIEW
};

enum M_ORIENTATION_MODE
{
	M_ORIENTATION_WORLD = 0,
	M_ORIENTATION_LOCAL
};

enum M_TRANSFORM_MODE
{
	M_TRANSFORM_MOUSE = 0,
	M_TRANSFORM_POSITION,
	M_TRANSFORM_ROTATION,
	M_TRANSFORM_SCALE
};


class MaratisEditor
{
public:

	// constructor / destructor
	MaratisEditor(void);
	~MaratisEditor(void);

	// instance
	static MaratisEditor * getInstance(void)
	{
		static MaratisEditor m_instance;
		return &m_instance;
	}

private:
	
	MOEntity * m_sphereEntity;
	MOEntity * m_coneEntity;
	MOEntity * m_cubeEntity;
	MOEntity * m_planeEntity;
	MOEntity * m_xEntity;
	MOEntity * m_yEntity;
	MOEntity * m_zEntity;
	MOEntity * m_xcircleEntity;
	MOEntity * m_ycircleEntity;
	MOEntity * m_zcircleEntity;
	MOEntity * m_lightEntity;
	MOEntity * m_cameraEntity;
	MOEntity * m_soundEntity;

	// mesh manager
	MDataManager m_meshManager;

	// empty text
	MOText m_emptyText;
};

#endif