/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis Image tools
// Resize.cpp
//
// Image resize
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2013 Anael Seghezzi <www.maratis3d.com>
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

#include <MCore.h>
#include "Common.h"
#include "Resize.h"


bool pyrDownImage(MImage * image)
{
	if(! isImageValid(image))
		return false;

	MImage copy;
	
	int width = (int)image->getWidth();
	int height = (int)image->getHeight();
	unsigned int components = image->getComponents();
	M_TYPES type = image->getDataType();
	
	int halfWidth = (width+1)/2;
	int halfHeight = (height+1)/2;

	if(type == M_UBYTE)
	{
		int * totalColor = new int[components];
	
		copy.create(type, width, height, components);
		memcpy(copy.getData(), image->getData(), image->getSize()*sizeof(char));
		image->create(type, halfWidth, halfHeight, components);
		
		unsigned char * copyData = (unsigned char *)copy.getData();
		unsigned char * pixel = (unsigned char *)image->getData();
		
		for(int y=0; y<halfHeight; y++)
		{
			int i;
			unsigned char * color;
			for(int x=0; x<halfWidth; x++)
			{
				int xp = MIN((x*2+1), width-1);
				int yp = MIN((y*2+1), height-1);
			
				color = copyData + (y*2)*width*components + (x*2)*components;
				for(i=0; i<components; i++)
					totalColor[i] = color[i];
				
				color = copyData + yp*width*components + (x*2)*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				color = copyData + (y*2)*width*components + xp*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				color = copyData + yp*width*components + xp*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				for(i=0; i<components; i++)
					pixel[i] = totalColor[i]*0.25f;
				
				pixel+=components;
			}
		}
		
		delete [] totalColor;
		return true;
	}
	else if(type == M_FLOAT)
	{
		float * totalColor = new float[components];
		
		copy.create(type, width, height, components);
		memcpy(copy.getData(), image->getData(), image->getSize()*sizeof(float));
		image->create(type, halfWidth, halfHeight, components);
		
		float * copyData = (float *)copy.getData();
		float * pixel = (float *)image->getData();
		
		for(int y=0; y<halfHeight; y++)
		{
			int i;
			float * color;
			for(int x=0; x<halfWidth; x++)
			{
				color = copyData + (y*2)*width*components + (x*2)*components;
				for(i=0; i<components; i++)
					totalColor[i] = color[i];
				
				color = copyData + (y*2+1)*width*components + (x*2)*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				color = copyData + (y*2)*width*components + (x*2+1)*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				color = copyData + (y*2+1)*width*components + (x*2+1)*components;
				for(i=0; i<components; i++)
					totalColor[i] += color[i];
				
				for(i=0; i<components; i++)
					pixel[i] = totalColor[i]*0.25f;
				
				pixel+=components;
			}
		}
		
		delete [] totalColor;
		return true;
	}
	
	return false;
}
