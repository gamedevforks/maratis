/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MV3dEdit.h
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


#include "MEditor.h"


MV3dEdit::MV3dEdit(void):
MV3dView(),
m_tools(NULL)
{}

MV3dEdit::~MV3dEdit(void)
{}

void MV3dEdit::create(MWindow * rootWindow)
{
	MV3dView::create(rootWindow);
	
	MEditor * editor = MEditor::getInstance();
	MLevel * guiData = editor->getGuiData();
	
	if(m_window && !m_tools)
	{
		MTextureRef * ref1_1 = guiData->loadTexture("Tool_Select.png");
		MTextureRef * ref1_2 = guiData->loadTexture("Tool_Select_Pressed.png");
		MTextureRef * ref2_1 = guiData->loadTexture("Tool_Translate.png");
		MTextureRef * ref2_2 = guiData->loadTexture("Tool_Translate_Pressed.png");
		MTextureRef * ref3_1 = guiData->loadTexture("Tool_Rotate.png");
		MTextureRef * ref3_2 = guiData->loadTexture("Tool_Rotate_Pressed.png");
		MTextureRef * ref4_1 = guiData->loadTexture("Tool_Scale.png");
		MTextureRef * ref4_2 = guiData->loadTexture("Tool_Scale_Pressed.png");
		MTextureRef * ref5_1 = guiData->loadTexture("Tool_Magnet.png");
		MTextureRef * ref5_2 = guiData->loadTexture("Tool_Magnet_Pressed.png");
		MTextureRef * ref6_1 = guiData->loadTexture("Tool_Mode.png");
		MTextureRef * ref6_2 = guiData->loadTexture("Tool_Mode_Pressed.png");
		
		m_tools = rootWindow->addNewWindow();
		m_tools->setColor(MVector4(0, 0, 0, 0));
			
		int y = 0;
		MGuiButton * button;
		button = m_tools->addNewButton();
		button->setMode(MGUI_BUTTON_GROUP);
		button->setNormalTexture(ref1_1);
		button->setPressedTexture(ref1_2);
		button->setScale(MVector2(ref1_1->getWidth(), ref1_1->getHeight()));
		button->setYPosition(y);
		button->setPressed(true);
		y+=button->getScale().y;
		
		button = m_tools->addNewButton();
		button->setMode(MGUI_BUTTON_GROUP);
		button->setNormalTexture(ref2_1);
		button->setPressedTexture(ref2_2);
		button->setScale(MVector2(ref2_1->getWidth(), ref2_1->getHeight()));
		button->setYPosition(y);
		y+=button->getScale().y;
		
		button = m_tools->addNewButton();
		button->setMode(MGUI_BUTTON_GROUP);
		button->setNormalTexture(ref3_1);
		button->setPressedTexture(ref3_2);
		button->setScale(MVector2(ref3_1->getWidth(), ref3_1->getHeight()));
		button->setYPosition(y);
		y+=button->getScale().y;
		
		button = m_tools->addNewButton();
		button->setMode(MGUI_BUTTON_GROUP);
		button->setNormalTexture(ref4_1);
		button->setPressedTexture(ref4_2);
		button->setScale(MVector2(ref4_1->getWidth(), ref4_1->getHeight()));
		button->setYPosition(y);
		y+=button->getScale().y;
		
		button = m_tools->addNewButton();
		button->setNormalTexture(ref5_1);
		button->setPressedTexture(ref5_2);
		button->setScale(MVector2(ref5_1->getWidth(), ref5_1->getHeight()));
		button->setYPosition(y);
		
		int x = button->getScale().x;
		button = m_tools->addNewButton();
		button->setNormalTexture(ref6_1);
		button->setPressedTexture(ref6_2);
		button->setScale(MVector2(ref6_1->getWidth(), ref6_1->getHeight()));
		button->setXPosition(x);
		button->setYPosition(y);
		
		m_tools->setMargin(MVector2(0, 0));
		m_tools->autoScale();
	}
}

void MV3dEdit::resize(MVector2 position, MVector2 scale)
{
	MV3dView::resize(position, scale);
	
	if(m_window && m_tools)
	{
		m_tools->setXPosition(m_window->getPosition().x + m_window->getScale().x - m_tools->getScale().x - MUI_TOOLS_X);
		m_tools->setYPosition(m_window->getPosition().y + MUI_TOOLS_Y);
	}
}

void MV3dEdit::hide(void)
{
	MV3dView::hide();
	
	if(m_tools)
		m_tools->setVisible(false);
}

void MV3dEdit::show(void)
{
	MV3dView::show();
	
	if(m_tools)
		m_tools->setVisible(true);
}

void MV3dEdit::onEvent(MWindow * rootWindow, MWIN_EVENT_TYPE event)
{
	MV3dView::onEvent(rootWindow, event);
}

M_TRANSFORM_MODE MV3dEdit::getTransformMode(void)
{
	if(m_tools)
	{
		for(int i=0; i<4; i++)
		{
			MGuiButton * button = m_tools->getButton(i);
			if(button->isPressed())
				return (M_TRANSFORM_MODE)i;
		}
	}
	
	return M_TRANSFORM_MOUSE;
}