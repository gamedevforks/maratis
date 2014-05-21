/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MRayUtils.cpp
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

#include <MEngine.h>
#include "MSkinCache.h"
#include "MRayUtils.h"


bool getEntityNearestDistance(MOEntity * entity, const MVector3 & origin, const MVector3 & dest, float * distance)
{
	MMesh * mesh = entity->getMesh();
	if(! mesh)
		return false;

	MArmature * armature = mesh->getArmature();
	if(armature)
	{
		if(mesh->getArmatureAnim())
		{
			animateArmature(
				mesh->getArmature(),
				mesh->getArmatureAnim(),
				entity->getCurrentFrame()
			);
		}
		else
		{
			armature->processBonesLinking();
			armature->updateBonesSkinMatrix();
		}
	}

	MMatrix4x4 * matrix = entity->getMatrix();
	MMatrix4x4 iMatrix = matrix->getInverse();

	MVector3 localOrigin = iMatrix * origin;
	MVector3 localDest = iMatrix * dest;

	MBox3d * ebox = entity->getBoundingBox();
	if(! isEdgeToBoxCollision(localOrigin, localDest, ebox->min, ebox->max))
		return false;
		
	bool raytraced = false;

	float dist;
	float nearDist;
	MVector3 I, intersectionPoint;
	MVector3 rayVector = localDest - localOrigin;

	// init near dist
	nearDist = rayVector.getSquaredLength();

	unsigned int s;
	unsigned int sSize = mesh->getSubMeshsNumber();
	for(s=0; s<sSize; s++)
	{
		MSubMesh * subMesh = &mesh->getSubMeshs()[s];
		MBox3d * box = subMesh->getBoundingBox();

		// skinning
		MVector3 * vertices = subMesh->getVertices();
		if(entity && mesh->getArmature() && subMesh->getSkinData())
		{
			unsigned int verticesSize = subMesh->getVerticesSize();
			MVector3 * skinVertices = MSkinCache::getInstance()->getVertices(verticesSize);
			computeSkinning(mesh->getArmature(), subMesh->getSkinData(), vertices, NULL, NULL, skinVertices, NULL, NULL);
			vertices = skinVertices;
			box->initFromPoints(vertices, verticesSize);
		}

		if(isEdgeToBoxCollision(localOrigin, localDest, box->min, box->max))
		{
			unsigned int d;
			unsigned int dSize = subMesh->getDisplaysNumber();
			for(d=0; d<dSize; d++)
			{
				MDisplay * display = subMesh->getDisplay(d);

				// indices
				void * indices = subMesh->getIndices();
				switch(subMesh->getIndicesType())
				{
					case M_USHORT:
					{
						unsigned short * idx = (unsigned short *)indices;
						indices = (void *)(idx + display->getBegin());
						break;
					}
					case M_UINT:
					{
						unsigned int * idx = (unsigned int *)indices;
						indices = (void *)(idx + display->getBegin());
						break;
					}

                    default:
                        break;
				}

				// BACK or FRONT and BACK, scan ray
				if((display->getCullMode() == M_CULL_BACK) || (display->getCullMode() == M_CULL_NONE))
				{
					if(getNearestRaytracedPosition(
						localOrigin, localDest,
						indices,
						subMesh->getIndicesType(),
						vertices,
						display->getSize(),
						&I))
					{
						dist = (I - localOrigin).getSquaredLength();
						if(dist < nearDist)
						{
							intersectionPoint = I;
							nearDist = dist;
							raytraced = true;
						}
					}
				}

				// FRONT or FRONT and BACK, scan invert
				if((display->getCullMode() == M_CULL_FRONT) || (display->getCullMode() == M_CULL_NONE))
				{
					if(getNearestRaytracedPosition(
						localOrigin, localDest,
						indices,
						subMesh->getIndicesType(),
						vertices,
						display->getSize(),
						&I, 1))
					{
						dist = (I - localOrigin).getSquaredLength();
						if(dist < nearDist)
						{
							intersectionPoint = I;
							nearDist = dist;
							raytraced = true;
						}
					}
				}
			}
		}
	}

	if(raytraced)
		*distance = (((*matrix) * intersectionPoint) - origin).getLength();

	return raytraced;
}

MOEntity * getSceneNearestEntity(MScene * scene, const MVector3 & origin, const MVector3 & dest, MVector3 * intersectPoint)
{
	float distance = 0;
	MOEntity * nearestEntity = NULL;
	
	unsigned int i;
	unsigned int oSize = scene->getEntitiesNumber();
	for(i=0; i<oSize; i++)
	{
		MOEntity * entity = scene->getEntityByIndex(i);
		
		if(! entity->isActive())
			continue;
		
		if(entity->isInvisible())
			continue;

		MMesh * mesh = entity->getMesh();
		if(! mesh)
			continue;
		
		MMatrix4x4 * matrix = entity->getMatrix();
		
		float dist;
		if(! getEntityNearestDistance(entity, origin, dest, &dist))
			continue;
	
		if((! nearestEntity) || (dist < distance))
		{
			nearestEntity = entity;
			distance = dist;
		}
	}
	
	if(intersectPoint && nearestObject)
		*intersectPoint = origin + (dest - origin).getNormalized()*distance;
	
	return nearestEntity;
}