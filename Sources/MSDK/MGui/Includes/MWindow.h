/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MGui
// MWindow.h
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


#ifndef _M_WINDOW_H
#define _M_WINDOW_H


class M_GUI_EXPORT MWindow
{
public:

	MWindow(int x, int y, unsigned int width, unsigned int height)
	{
		m_drawCallback = NULL;
		m_eventCallback = NULL;
		m_currentKey = 0;
		m_currentMouseButton = 0;
		m_pos[0] = x;
		m_pos[1] = y;
		m_width = width;
		m_height = height;
		memset(m_key, 0, sizeof(bool)*256);
		memset(m_mouseButton, 0, sizeof(bool)*3);
	}
	
	virtual ~MWindow(void){ clear(); }

protected:

	// pos / scale
	int m_pos[2];
	unsigned int m_width;
	unsigned int m_height;

	// keys
	bool m_key[256];
	int m_currentKey;

	// mouse
	bool m_mouseButton[3];
	int m_currentMouseButton;
	MVector2 m_mousePos;
	MVector2 m_mouseDir;
	MVector2 m_mouseScroll;
	
	// local windows
	vector <MGuiWindow *> m_windows;

	// pointer event
	void (* m_eventCallback)(MWindow * rootWindow, MWIN_EVENT_TYPE event);

	// draw callback
	void (* m_drawCallback)(MWindow * rootWindow);

	// events
	void onEvent(MWIN_EVENT_TYPE event);

public:

	// pos / scale
	inline int getXPosition(void){ return m_pos[0]; }
	inline int getYPosition(void){ return m_pos[1]; }
	inline unsigned int getWidth(void){ return m_width; }
	inline unsigned int getHeight(void){ return m_height; }
	
	// keys
	inline int getKey(void){ return m_currentKey; }
	inline bool isKeyPressed(unsigned char id){ return m_key[id]; }

	// mouse
	inline int getMouseButton(void){ return m_currentMouseButton; }
	inline bool isMouseButtonPressed(unsigned char id){ if(id > 2) return false; return m_mouseButton[id]; }
	inline MVector2 getMousePosition(void) const { return m_mousePos; }
	inline MVector2 getMouseScroll(void) const { return m_mouseScroll; }
	inline MVector2 getMouseDir(void) const { return m_mouseDir; }
	
	// windows
	void unHighLightAllWindows(void);
	bool isSomethingEditing(void);
	
	MGuiWindow * addNewWindow(void);
	inline unsigned int getWindowsNumber(void){ return m_windows.size(); }
	inline MGuiWindow * getWindow(unsigned int id){ return m_windows[id]; }
	void deleteWindow(MGuiWindow * window);

	// events
	inline void onChar(int character){ m_currentKey = character; onEvent(MWIN_EVENT_CHAR); }
	inline void onKeyDown(unsigned char key){ m_key[key] = true;  m_currentKey = key; onEvent(MWIN_EVENT_KEY_DOWN); }
	inline void onKeyUp(unsigned char key)  { m_key[key] = false; m_currentKey = key; onEvent(MWIN_EVENT_KEY_UP); }
	
	inline void onCreate(void){ onEvent(MWIN_EVENT_WINDOW_CREATE); }
	inline void onMove(int x, int y){ m_pos[0] = x; m_pos[1] = y; onEvent(MWIN_EVENT_WINDOW_MOVE); }
	inline void onResize(unsigned int width, unsigned int height){ m_width = width; m_height = height; onEvent(MWIN_EVENT_WINDOW_RESIZE); }
	inline void onClose(void){ onEvent(MWIN_EVENT_WINDOW_CLOSE); }
	
	inline void onMouseButtonDown(unsigned char button){ if(button > 2) return; m_mouseButton[button] = true;  m_currentMouseButton = button; onEvent(MWIN_EVENT_MOUSE_BUTTON_DOWN); }
	inline void onMouseButtonUp(unsigned char button)  { if(button > 2) return; m_mouseButton[button] = false; m_currentMouseButton = button; onEvent(MWIN_EVENT_MOUSE_BUTTON_UP); }
	inline void onMouseMove(const MVector2 & pos){ m_mouseDir = pos - m_mousePos; m_mousePos = pos; onEvent(MWIN_EVENT_MOUSE_MOVE); }
	inline void onMouseScroll(const MVector2 & scroll){ m_mouseScroll = scroll; onEvent(MWIN_EVENT_MOUSE_SCROLL); }

	// pointer event
	inline void setEventCallback(void (* eventCallback)(MWindow * rootWindow, MWIN_EVENT_TYPE event)){ m_eventCallback = eventCallback; }

	// draw callback
	inline void setDrawCallback(void (* drawCallback)(MWindow * rootWindow)){ m_drawCallback = drawCallback; }

	// draw
	void draw(void);

	// clear
	void clear(void);
};

#endif
