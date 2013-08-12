/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis Image tools
// Color.cpp
//
// Color tools
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
#include "Color.h"


bool contrast(MImage * image, float factor)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] = CLAMP(imageData[i] + factor*(imageData[i] - 127), 0, 255);
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] = imageData[i] + factor*(imageData[i] - 0.5f);
		return true;
	}
	
	return false;
}

bool clamp(MImage * image, float min, float max)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] = CLAMP(imageData[i], min, max);
		return true;
	}
	
	return false;
}

bool normalize(MImage * image)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_FLOAT)
	{
		float min, max;
		float * imageData = (float *)image->getData();
		
		min = max = imageData[0];
		
        for(i=0; i<size; i++)
		{
            min = MIN(min, imageData[i]);
			max = MAX(max, imageData[i]);
		}
		
		for(i=0; i<size; i++)
			imageData[i] = (imageData[i]-min)/(max-min);
			
		return true;
	}
	
	return false;
}

bool product(MImage * image, float factor)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] = CLAMP(imageData[i]*factor, 0, 255);
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] *= factor;
		return true;
	}
	
	return false;
}

bool product(MImage * image, MImage * image2)
{
	if(! (isImageValid(image) && isImageValid(image)))
		return false;

	unsigned int i, size = image->getSize();
	unsigned int c1 = image->getComponents();
	unsigned int c2 = image2->getComponents();
	
	if(image->getDataType() == M_UBYTE && image2->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
		unsigned char * image2Data = (unsigned char *)image2->getData();
		
		if(c1 == c2)
		{
			for(i=0; i<size; i++)
				imageData[i] = CLAMP(imageData[i] * (image2Data[i]/255.0f), 0, 255);
			return true;
		}
		else if(c2 == 1)
		{
			for(i=0; i<image2->getSize(); i++)
			{
				float factor = image2Data[i]/255.0f;
				for(int c=0; c<c1; c++)
					imageData[i*c1+c] = CLAMP(imageData[i*c1+c] * factor, 0, 255);
			}
			return true;
		}
	}
	else if(image->getDataType() == M_FLOAT && image2->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
		float * image2Data = (float *)image2->getData();
		
		if(c1 == c2)
		{
			for(i=0; i<size; i++)
				imageData[i] *= image2Data[i];
			return true;
		}
		else if(c2 == 1)
		{
			for(i=0; i<image2->getSize(); i++)
			{
				for(int c=0; c<c1; c++)
					imageData[i*c1+c] *= image2Data[i];
			}
			return true;
		}
		
	}
	
	return false;
}

bool addition(MImage * image, float factor)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] = CLAMP(imageData[i] + factor, 0, 255);
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
            imageData[i] += factor;
		return true;
	}
	
	return false;
}

bool addition(MImage * image, MImage * image2)
{
	if(! (isImageValid(image) && isImageValid(image)))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE && image2->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
		unsigned char * image2Data = (unsigned char *)image2->getData();
        for(i=0; i<size; i++)
            imageData[i] = CLAMP(imageData[i] + image2Data[i], 0, 255);
		return true;
	}
	else if(image->getDataType() == M_FLOAT && image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
		float * image2Data = (float *)image2->getData();
        for(i=0; i<size; i++)
            imageData[i] += image2Data[i];
		return true;
	}
	
	return false;
}

bool negative(MImage * image)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * imageData = (unsigned char *)image->getData();
        for(i=0; i<size; i++)
			imageData[i] = 255 - imageData[i];
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
			imageData[i] = 1 - imageData[i];
		return true;
	}
	
	return false;
}

bool premultiply(MImage * image)
{
	if(! isImageValid(image))
		return false;
		
	if(image->getComponents() != 4)
		return false;
		
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * pixel = (unsigned char *)image->getData();
		unsigned int p, pSize = image->getSize();
		for(p=0; p<pSize; p+=4)
		{
			float a = pixel[3]/255.0f;
			pixel[0] = (unsigned char)(pixel[0]*a);
			pixel[1] = (unsigned char)(pixel[1]*a);
			pixel[2] = (unsigned char)(pixel[2]*a);
			pixel+=4;
		}
		
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * pixel = (float *)image->getData();
		unsigned int p, pSize = image->getSize();
		for(p=0; p<pSize; p+=4)
		{
			pixel[0] *= pixel[3];
			pixel[1] *= pixel[3];
			pixel[2] *= pixel[3];
			pixel+=4;
		}
		
		return true;
	}
	
	return false;
}

bool unpremultiply(MImage * image)
{
	if(! isImageValid(image))
		return false;
		
	if(image->getComponents() != 4)
		return false;
		
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char * pixel = (unsigned char *)image->getData();
		unsigned int p, pSize = image->getSize();
		for(p=0; p<pSize; p+=4)
		{
			MVector4 color = MVector4(pixel[0], pixel[1], pixel[2], pixel[3])/255.0f;
		
			if(color.w > 0)
			{
				float div = 1.0f/color.w;
				color.x *= div;
				color.y *= div;
				color.z *= div;
			
				pixel[0] = MIN(color.x*255, 255);
				pixel[1] = MIN(color.y*255, 255);
				pixel[2] = MIN(color.z*255, 255);
			}
		
			pixel+=4;
		}
		
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * pixel = (float *)image->getData();
		unsigned int p, pSize = image->getSize();
		for(p=0; p<pSize; p+=4)
		{
			if(pixel[3] > 0)
			{
				float div = 1.0f/pixel[3];
				pixel[0] *= div;
				pixel[1] *= div;
				pixel[2] *= div;
			}
		
			pixel+=4;
		}
		
		return true;
	}
	
	return false;
}

bool threshold(MImage * image, float threshold)
{
	if(! isImageValid(image))
		return false;

	unsigned int i, size = image->getSize();
	
	if(image->getDataType() == M_UBYTE)
	{
		unsigned char ubth = threshold*255;
		unsigned char * imageData = (unsigned char *)image->getData();
        for(i=0; i<size; i++)
		{
			if(imageData[i] < ubth)
				imageData[i] = 0;
			else
				imageData[i] = 255;
		}
		return true;
	}
	else if(image->getDataType() == M_FLOAT)
	{
		float * imageData = (float *)image->getData();
        for(i=0; i<size; i++)
		{
			if(imageData[i] < threshold)
				imageData[i] = 0;
			else
				imageData[i] = 1;
		}
		return true;
	}
	
	return false;
}
