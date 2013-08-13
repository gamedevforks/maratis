/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MGui
// MGui2d.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2013 Anael Seghezzi <www.maratis3d.com>
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


#include <MEngine.h>
#include "../Includes/MGui.h"


MGui2d::MGui2d(void):
m_textObject(NULL),
m_parentWindow(NULL),
m_isVisible(true),
m_isHighLight(false),
m_isPressed(false),
m_isDrawingText(true),
m_normalTexture(NULL),
m_pressedTexture(NULL),
m_highLightTexture(NULL),
m_hasShadow(false),
m_isScrollAffect(true),
m_variableType(M_VARIABLE_NULL),
m_variablePointer(NULL),
m_customPointer(NULL),
m_autoScaleFromText(true),
m_scale(16, 16)
{
	setNormalColor(MVector3(1, 1, 1));
	setHighLightColor(MVector3(1, 1, 1));
	setPressedColor(MVector3(1, 1, 1));
}

MGui2d::~MGui2d(void)
{
}

MVector2 MGui2d::getAlignedTextPosition(void)
{
	MBox3d * box = m_textObject.getBoundingBox();
	
	if(box->min == box->max)
	{
		return MVector2(0, getTextSize());
	}
	
	switch(m_textObject.getAlign())
	{
		default:
		case M_ALIGN_LEFT:
		{
			return MVector2(-box->min.x, getTextSize());
		}
		case M_ALIGN_RIGHT:
		{
			return MVector2(-box->min.x + (m_scale.x - (box->max.x - box->min.x)), getTextSize());
		}
		case M_ALIGN_CENTER:
		{
			return MVector2(
				-box->min.x + (m_scale.x - (box->max.x - box->min.x))*0.5f,
				-box->min.y + (m_scale.y - (box->max.y - box->min.y))*0.5f
			);
		}
	}
}

void MGui2d::autoScaleFromText(void)
{
	if(! isDrawingText())
		return;

	MBox3d * box = m_textObject.getBoundingBox();
	setScale(MVector2(box->max.x, box->max.y) - MVector2(box->min.x, box->min.y));
}

void MGui2d::setNormalTexture(MTextureRef * texture)
{
	m_normalTexture = texture;
	
	if(texture)
	{
		if(m_pressedTexture == NULL)
			m_pressedTexture = texture;

		if(m_highLightTexture == NULL)
			m_highLightTexture = texture;
	}
}

void MGui2d::setPressedTexture(MTextureRef * texture)
{
	m_pressedTexture = texture;
}

void MGui2d::setHighLightTexture(MTextureRef * texture)
{
	m_highLightTexture = texture;
}

void MGui2d::draw(void)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();
	
	if(! isVisible())
		return;

	if(isPressed()) // pressed
	{  
		render->setColor4(getPressedColor());
		if(hasPressedTexture())
			drawTexturedQuad(getPressedTexture()->getTextureId());
		else
			drawQuad();
	}
	else if(isHighLight()) // highLight
	{
		render->setColor4(getHighLightColor());
		if(hasHighLightTexture())
			drawTexturedQuad(getHighLightTexture()->getTextureId());
		else
			drawQuad();
	}
	else // normal	
	{
		render->setColor4(getNormalColor());
		if(hasNormalTexture())
			drawTexturedQuad(getNormalTexture()->getTextureId());
		else
			drawQuad();
	}

	if(isDrawingText())
	{
		drawText();
	}

	// draw shadows
	if(hasShadow())
	{
		render->pushMatrix();
		render->translate(MVector3(getPosition().x, getPosition().y, 0));
		drawShadow();
		render->popMatrix();
	}
}

void MGui2d::drawShadow(void)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();
	
	MVector2 g_vertices[8];
	MVector4 g_colors[8];

	render->disableTexture();

	MVector4 color0 = MVector4(0, 0, 0, 0);
	MVector4 color1 = MVector4(0, 0, 0, 0.05f);

	float size = 4;

	MVector2 dir[4];
	dir[0] = MVector2(size, size*0.4f);
	dir[1] = MVector2(size*0.4f, size);
	dir[2] = MVector2(size*0.4f, size*0.1f);
	dir[3] = MVector2(size*0.1f, size*0.4f);

	render->disableNormalArray();
	render->disableTexCoordArray();
	render->enableVertexArray();
	render->enableColorArray();

	render->setVertexPointer(M_FLOAT, 2, g_vertices);
	render->setColorPointer(M_FLOAT, 4, g_colors);

	for(int i=0; i<4; i++)
	{
		g_colors[0] = color1;
		g_vertices[0] = MVector2(0, m_scale.y) + MVector2(dir[i].x, 0);
		g_colors[1] = color0;
		g_vertices[1] = MVector2(0, m_scale.y) + MVector2(size, size) + dir[i];

		g_colors[2] = color1;
		g_vertices[2] = MVector2(m_scale.x, m_scale.y);
		g_colors[3] = color0;
		g_vertices[3] = MVector2(m_scale.x, m_scale.y) + MVector2(size, size) + dir[i];

		g_colors[4] = color1;
		g_vertices[4] = MVector2(m_scale.x, 0) + MVector2(0, dir[i].y);
		g_colors[5] = color0;
		g_vertices[5] = MVector2(m_scale.x, 0) + MVector2(size, size) + dir[i];

		render->drawArray(M_PRIMITIVE_TRIANGLE_STRIP, 0, 6);
	}
}

MWindow * MGui2d::getRootWindow(void)
{
	if(m_parentWindow)
		return m_parentWindow->getRootWindow();

	else if(getType() == M_GUI_WINDOW)
		return ((MGuiWindow *)this)->getRootWindow();
	
	return NULL;
}

bool MGui2d::isMouseInside(const MVector2 & margin)
{
	MWindow * rootWindow = getRootWindow();
	if(rootWindow)
		return isPointInside(getPointLocalPosition(rootWindow->getMousePosition()), margin);
		
	return false;
}

bool MGui2d::isPointInside(const MVector2 & point, const MVector2 & margin)
{
	MVector2 position = getPosition() - margin;
	MVector2 scale = getScale() + margin*2;

	if(point.x >= position.x && point.x < (position.x + scale.x) &&
	   point.y >= position.y && point.y < (position.y + scale.y))
	   return true;

	return false;
}

void MGui2d::drawQuad(void)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();
	
	MVector2 g_vertices[8];

	render->disableTexture();

	render->disableNormalArray();
	render->disableTexCoordArray();
	render->disableColorArray();
	render->enableVertexArray();

	g_vertices[0] = MVector2(m_position.x, m_position.y);
	g_vertices[1] = MVector2(m_position.x, m_position.y + m_scale.y);
	g_vertices[3] = MVector2(m_position.x + m_scale.x, m_position.y + m_scale.y);
	g_vertices[2] = MVector2(m_position.x + m_scale.x, m_position.y);

	render->setVertexPointer(M_FLOAT, 2, g_vertices);
	render->drawArray(M_PRIMITIVE_TRIANGLE_STRIP, 0, 4);
}

void MGui2d::drawTexturedQuad(unsigned int textureId)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();
	
	MVector2 g_vertices[8];
	MVector2 g_texCoords[8];

	render->enableTexture();
	
	render->disableNormalArray();
	render->disableColorArray();
	render->enableVertexArray();
	render->enableTexCoordArray();

	g_vertices[0] = MVector2(m_position.x, m_position.y);
	g_vertices[1] = MVector2(m_position.x, m_position.y + m_scale.y);
	g_vertices[3] = MVector2(m_position.x + m_scale.x, m_position.y + m_scale.y);
	g_vertices[2] = MVector2(m_position.x + m_scale.x, m_position.y);

	g_texCoords[0] = MVector2(0, 0);
	g_texCoords[1] = MVector2(0, 1);
	g_texCoords[3] = MVector2(1, 1);
	g_texCoords[2] = MVector2(1, 0);

	render->setTexCoordPointer(M_FLOAT, 2, g_texCoords);
	render->setVertexPointer(M_FLOAT, 2, g_vertices);

	render->bindTexture(textureId);
	render->drawArray(M_PRIMITIVE_TRIANGLE_STRIP, 0, 4);
}

// draw text
void MGui2d::drawText(void)
{
	MEngine * engine = MEngine::getInstance();
	MRenderingContext * render = engine->getRenderingContext();
	
	MVector2 pos = getPosition() + getAlignedTextPosition();
	{
		// make sure the text is not filtered
		pos.x = (int)(pos.x+0.5f);
		pos.y = (int)(pos.y+0.5f);
	}
	
	render->pushMatrix();
	render->translate(pos);
	engine->getRenderer()->drawText(&m_textObject);
	render->popMatrix();
}

MVector2 MGui2d::getPointLocalPosition(const MVector2 & point)
{
	if(m_parentWindow)
	{
		if(isScrollAffected())
		{
			return (point - m_parentWindow->getPosition() - m_parentWindow->getScroll())/m_parentWindow->getZoom();
		}
		else
			return point - m_parentWindow->getPosition();
	}

	return point;
}
