/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MSkinCache.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
//  Maratis, Copyright (c) 2003-2014 Anael Seghezzi <www.maratis3d.com>
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

#ifndef _M_SKIN_CACHE_H
#define _M_SKIN_CACHE_H


class MSkinCache
{
private:

	unsigned int m_verticesNumber;
	unsigned int m_normalsNumber;
	MVector3 * m_vertices;
	MVector3 * m_normals;

public:

	MSkinCache(void):
    m_verticesNumber(0),
    m_normalsNumber(0),
    m_vertices(NULL),
    m_normals(NULL)
	{
		// pre-alloc
		getVertices(256*256);
		getNormals(256*256);
	}

	virtual ~MSkinCache(void)
	{
		SAFE_DELETE_ARRAY(m_vertices);
		SAFE_DELETE_ARRAY(m_normals);
	}

	static MSkinCache * getInstance(void)
	{
		static MSkinCache m_instance;
		return &m_instance;
	};

	MVector3 * getVertices(unsigned int size)
	{
		if(size > m_verticesNumber)
		{
			SAFE_DELETE_ARRAY(m_vertices);
			m_vertices = new MVector3[size];
			m_verticesNumber = size;
		}

		return m_vertices;
	}

	MVector3 * getNormals(unsigned int size)
	{
		if(size > m_normalsNumber)
		{
			SAFE_DELETE_ARRAY(m_normals);
			m_normals = new MVector3[size];
			m_normalsNumber = size;
		}

		return m_normals;
	}
};

#endif