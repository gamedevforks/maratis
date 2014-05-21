/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// M3dView.h
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

#include "MViewport.h"
#include "MView.h"
#include "MPreferences.h"
#include "M3dView.h"


M3dView::M3dView(void):
m_window(NULL)
{}

M3dView::~M3dView(void)
{
	if(m_window)
	{
		m_window->deleteMe();
		m_window = NULL;
	}
}

void M3dView::create(MWindow * rootWindow)
{
	if(! m_window)
		m_window = rootWindow->addNewWindow();
}

void M3dView::constraintTo(MVector2 position, MVector2 scale)
{
	if(m_window)
	{
		m_window->setPosition(position);
		m_window->setScale(scale);
	}
}

void M3dView::hide(void)
{
	if(m_window)
		m_window->setVisible(false);
}

void M3dView::show(void)
{
	if(m_window)
		m_window->setVisible(true);
}

void M3dView::onEvent(MWindow * rootWindow, MWIN_EVENT_TYPE event)
{
	MPreferences * prefs = MPreferences::getInstance();

	if(! m_window)
		return;
		
	if(rootWindow->isSomethingEditing() || (! m_window->isVisible()))
		return;

	switch(event)
	{
        case MWIN_EVENT_MOUSE_MOVE:
		{
			MVector2 mouseDir = rootWindow->getMouseDir();
			
			/*if(mouse->isLeftButtonPushed())
			{
				maratis->transformSelectedObjects();
				break;
			}*/

			if(m_window->isScrolled() && rootWindow->isMouseButtonPressed(MMOUSE_BUTTON_MIDDLE))
			{
				if(prefs->isShortCutEngaged(rootWindow, "Pan View"))
				{
					m_view.pan(mouseDir.x, mouseDir.y);
				}
				else
				{
					m_view.rotate(mouseDir.x, mouseDir.y);
				}
			}
			
			break;
		}
	
        case MWIN_EVENT_MOUSE_SCROLL:
		{
			if(m_window->isHighLight() && !rootWindow->isMouseButtonPressed(MMOUSE_BUTTON_MIDDLE))
				m_view.zoom(rootWindow->getMouseScroll().y);
				
			break;
		}

		case MWIN_EVENT_KEY_UP:
		{
			if(prefs->isShortCutEngaged(rootWindow, "Pan View"))
			{
				//maratis->updateViewCenter();
				break;
			}
			
			if(prefs->isShortCutEngaged(rootWindow, "Orthogonal View"))
			{
				m_view.switchProjectionMode();
				break;
            }
			
			if(prefs->isShortCutEngaged(rootWindow, "Face View"))
            {
                m_view.initOrtho(1);
                break;
            }
			
            if(prefs->isShortCutEngaged(rootWindow, "Right View"))
            {
                m_view.initOrtho(3);
                break;
            }
			
            if(prefs->isShortCutEngaged(rootWindow, "Top View"))
            {
                m_view.initOrtho(7);
                break;
            }
			
            if(prefs->isShortCutEngaged(rootWindow, "Bottom View"))
            {
                m_view.initOrtho(9);
                break;
            }
			
			break;
		}
		
		default:
			break;
	}
}