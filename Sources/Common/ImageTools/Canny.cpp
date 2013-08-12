/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis Image tools
// Canny.cpp
//
// Canny edge detection
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
#include "fast-edge/fast-edge.h"

#include "Canny.h"


bool canny(MImage * src, MImage * dest, int low, int high)
{
	if(! isImageValid(src) || ! dest)
		return false;

	if(! (src->getDataType() == M_UBYTE && src->getComponents() == 1))
		return false;

	int width = (int)src->getWidth();
	int height = (int)src->getHeight();

	// if src and dest are the same, create a temporary copy
	bool self = src==dest;
	if(self)
		src = new MImage(*dest);
	else
		dest->create(M_UBYTE, width, height, 1);

	// use fast-edge lib
	struct image im1, im2;
	im1.width = im2.width = src->getWidth();
	im1.height = im2.height = src->getHeight();
	im1.pixel_data = (unsigned char *)src->getData();
	im2.pixel_data = (unsigned char *)dest->getData();
	
	canny_edge_detect(&im1, &im2, high, low);
	
	// delete temporary copy
	if(self)
		delete src;
		
	return true;
}
