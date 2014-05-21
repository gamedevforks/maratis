/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MPreferences.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
//  Maratis, Copyright (c) 2003-2014 Anael Seghezzi <www.maratis3d.com>
//  Mario Pispek, 2012
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
#include <tinyxml.h>
#include "MPreferences.h"


bool MShortcut::isEngaged(MWindow * rootWindow)
{
	if(key1>=0)
	{
		if(rootWindow->getKey() != key1)
			return false;
	}
	
	if(key2>=0)
	{
		if(! rootWindow->isKeyPressed(key2))
			return false;
	}
	
	if(key3>=0)
	{
		if(! rootWindow->isKeyPressed(key3))
			return false;
	}
 
	return true;
}

MPreferences::MPreferences(void)
{
    initShortcuts();
}

MPreferences::~MPreferences(void)
{
	map<string, MShortcut*>::iterator
		mit (m_shortcuts.begin()),
		mend(m_shortcuts.end());

	for(; mit!=mend; mit++)
	{
		SAFE_DELETE(mit->second);
	}
}

int MPreferences::getKey(const char * name)
{
	map<string, int>::iterator iter = m_keys.find(name);
    
    if(iter != m_keys.end())
    {
        return iter->second;
    }
	
	return -1;
}

MShortcut * MPreferences::getShortcut(const char * name)
{
    map<string, MShortcut*>::iterator iter = m_shortcuts.find(name);
    
    if(iter != m_shortcuts.end())
    {
        return iter->second;
    }
    
    return NULL;
}

bool MPreferences::isShortCutEngaged(MWindow * rootWindow, const char * name)
{
	MShortcut * shortcut = getShortcut(name);
	if(shortcut)
	{
		return shortcut->isEngaged(rootWindow);
	}
	
	return false;
}

void MPreferences::initShortcuts(void)
{
	m_keys["A"] = 'A';
    m_keys["B"] = 'B';
    m_keys["C"] = 'C';
    m_keys["D"] = 'D';
    m_keys["E"] = 'E';
    m_keys["F"] = 'F';
    m_keys["G"] = 'G';
    m_keys["H"] = 'H';
    m_keys["I"] = 'I';
    m_keys["J"] = 'J';
    m_keys["K"] = 'K';
    m_keys["L"] = 'L';
    m_keys["M"] = 'M';
    m_keys["N"] = 'N';
    m_keys["O"] = 'O';
    m_keys["P"] = 'P';
    m_keys["Q"] = 'Q';
    m_keys["R"] = 'R';
    m_keys["S"] = 'S';
    m_keys["T"] = 'T';
    m_keys["U"] = 'U';
    m_keys["V"] = 'V';
    m_keys["W"] = 'W';
    m_keys["X"] = 'X';
    m_keys["Y"] = 'Y';
    m_keys["Z"] = 'Z';
    m_keys["1"] = '1';
    m_keys["2"] = '2';
    m_keys["3"] = '3';
    m_keys["4"] = '4';
    m_keys["5"] = '5';
    m_keys["6"] = '6';
    m_keys["7"] = '7';
    m_keys["8"] = '8';
    m_keys["9"] = '9';
    m_keys["0"] = '0';
    m_keys["-"] = '-';
    m_keys["="] = '=';
    m_keys["/"] = '/';
    m_keys["."] = '.';
    m_keys[","] = ',';
    m_keys["\\"] = '\\';
    m_keys["["] = '[';
    m_keys["]"] = ']';
    m_keys[";"] = ';';
    m_keys["'"] = '\'';
    m_keys["`"] = '`';
    m_keys["Escape"] = MKEY_ESCAPE;
    m_keys["Enter"] = MKEY_RETURN;
    m_keys["Num Enter"] = MKEY_KP_ENTER;
    m_keys["Tab"] = MKEY_TAB;
    m_keys["Backspace"] = MKEY_BACKSPACE;
    m_keys["Space"] = MKEY_SPACE;
    m_keys["Right Control"] = MKEY_RCONTROL;
    m_keys["Left Control"] = MKEY_LCONTROL;
    m_keys["Right Super"] = MKEY_RSUPER;
    m_keys["Left Super"] = MKEY_LSUPER;
    m_keys["Right Alt"] = MKEY_RALT;
    m_keys["Left Alt"] = MKEY_LALT;
    m_keys["Right Shift"] = MKEY_RSHIFT;
    m_keys["Left Shift"] = MKEY_LSHIFT;
    m_keys["Right Super"] = MKEY_RSUPER;
    m_keys["Left Super"] = MKEY_LSUPER;
    m_keys["F1"] = MKEY_F1;
    m_keys["F2"] = MKEY_F2;
    m_keys["F3"] = MKEY_F3;
    m_keys["F4"] = MKEY_F4;
    m_keys["F5"] = MKEY_F5;
    m_keys["F6"] = MKEY_F6;
    m_keys["F7"] = MKEY_F7;
    m_keys["F8"] = MKEY_F8;
    m_keys["F9"] = MKEY_F9;
    m_keys["F10"] = MKEY_F10;
    m_keys["F11"] = MKEY_F11;
    m_keys["F12"] = MKEY_F12;
    m_keys["Print Screen"] = MKEY_PRINT;
    m_keys["Pause"] = MKEY_PAUSE;
    m_keys["Insert"] = MKEY_INSERT;
    m_keys["Home"] = MKEY_HOME;
    m_keys["Page Up"] = MKEY_PAGEUP;
    m_keys["Page Down"] = MKEY_PAGEDOWN;
    m_keys["End"] = MKEY_END;
    m_keys["Delete"] = MKEY_DELETE;
    m_keys["Num Lock"] = MKEY_NUMLOCK;
    m_keys["Num 1"] = MKEY_KP_1;
    m_keys["Num 2"] = MKEY_KP_2;
    m_keys["Num 3"] = MKEY_KP_3;
    m_keys["Num 4"] = MKEY_KP_4;
    m_keys["Num 5"] = MKEY_KP_5;
    m_keys["Num 6"] = MKEY_KP_6;
    m_keys["Num 7"] = MKEY_KP_7;
    m_keys["Num 8"] = MKEY_KP_8;
    m_keys["Num 9"] = MKEY_KP_9;
    m_keys["Num 0"] = MKEY_KP_0;
    m_keys["Left"] = MKEY_LEFT;
    m_keys["Right"] = MKEY_RIGHT;
    m_keys["Down"] = MKEY_DOWN;
    m_keys["Up"] = MKEY_UP;


	// add default shortcuts
	MShortcut * sc;
	
	sc = new MShortcut();
    sc->key1 = getKey("S");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Save"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Z");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Undo"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Y");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Redo"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("O");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Load Level"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Q");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Quit"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("D");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Duplicate"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("A");
    sc->key2 = getKey("Left Control");
    m_shortcuts["Select All"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Num 5");
    m_shortcuts["Orthogonal View"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Num 1");
    m_shortcuts["Face View"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Num 3");
    m_shortcuts["Right View"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Num 7");
    m_shortcuts["Top View"] = sc;

	sc = new MShortcut();
    sc->key1 = m_keys["Num 9"];
    m_shortcuts["Bottom View"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("M");
    m_shortcuts["Transform Mouse"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("T");
    m_shortcuts["Transform Position"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("R");
    m_shortcuts["Transform Rotation"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("S");
    m_shortcuts["Transform Scale"] = sc;

	sc = new MShortcut();
    sc->key2 = getKey("Left Control");
    m_shortcuts["Multiple Selection"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("F");
    m_shortcuts["Focus Selection"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Backspace");
    m_shortcuts["Delete"] = sc;

	sc = new MShortcut();
    sc->key1 = getKey("Delete");
    m_shortcuts["Delete2"] = sc;

	sc = new MShortcut();
    sc->key2 = getKey("Left Control");
    m_shortcuts["Pan View"] = sc;
}

void MPreferences::load(const char * filename)
{
	TiXmlDocument doc(filename);
	if(! doc.LoadFile())
        return;
		
    TiXmlHandle hDoc(&doc);
	TiXmlElement * rootNode;
	TiXmlHandle hRoot(0);

	// Maratis
	rootNode = hDoc.FirstChildElement().Element();
	if(! rootNode)
		return;

	if(strcmp(rootNode->Value(), "Maratis") != 0)
		return;

	hRoot = TiXmlHandle(rootNode);

	// Preferences
	TiXmlElement * preferencesNode = rootNode->FirstChildElement("Preferences");
	if(! preferencesNode)
		return;

	// Shortcuts
	TiXmlElement * shortcutsNode = preferencesNode->FirstChildElement("Shortcuts");
	if(shortcutsNode)
	{
		// Shortcut
		TiXmlElement * shortcutNode = shortcutsNode->FirstChildElement("Shortcut");
		if(shortcutNode)
		{
			do {
	
				const char * name = shortcutNode->Attribute("name");
				if(! name)
					continue;
		
				MShortcut * shortcut = getShortcut(name);
				if(! shortcut)
				{
					shortcut = new MShortcut();
					m_shortcuts[name] = shortcut;
				}
		
				const char * key1 = shortcutNode->Attribute("key1");
				const char * key2 = shortcutNode->Attribute("key2");
				const char * key3 = shortcutNode->Attribute("key3");
		
				if(key1)
					m_shortcuts[name]->key1 = getKey(key1);
				if(key2)
					m_shortcuts[name]->key2 = getKey(key2);
				if(key3)
					m_shortcuts[name]->key3 = getKey(key3);
		
			} while ((shortcutNode = shortcutNode->NextSiblingElement()));
		}
	}
}