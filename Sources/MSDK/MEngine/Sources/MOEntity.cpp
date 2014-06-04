/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MOEntity.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
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


#include "../Includes/MEngine.h"


MPhysicsConstraint::MPhysicsConstraint(void):
constraintId(0),
parentObjectId(0),
parentName("..."),
lowerAngularLimit(-180),
upperAngularLimit(180),
disableParentCollision(false)
{}

MPhysicsConstraint::MPhysicsConstraint(const MPhysicsConstraint & constraint):
constraintId(0),
parentObjectId(0),
parentName(constraint.parentName),
pivot(constraint.pivot),
lowerLinearLimit(constraint.lowerLinearLimit),
upperLinearLimit(constraint.upperLinearLimit),
lowerAngularLimit(constraint.lowerAngularLimit),
upperAngularLimit(constraint.upperAngularLimit),
disableParentCollision(constraint.disableParentCollision)
{}


MPhysicsProperties::MPhysicsProperties(void):
	m_constraint(NULL),
	m_shapeId(0),
	m_collisionObjectId(0),
	m_collisionShape(M_COLLISION_SHAPE_BOX),
	m_ghost(false),
	m_mass(0),
	m_friction(0.5f),
	m_restitution(0.0f),
	m_linearDamping(0.01f),
	m_angularDamping(0.01f),
	m_angularFactor(1),
	m_linearFactor(1, 1, 1)
{}

MPhysicsProperties::MPhysicsProperties(const MPhysicsProperties & physicsProperties):
	m_constraint(NULL),
	m_shapeId(physicsProperties.m_shapeId),
	m_collisionObjectId(0),
	m_collisionShape(physicsProperties.m_collisionShape),
	m_ghost(physicsProperties.m_ghost),
	m_mass(physicsProperties.m_mass),
	m_friction(physicsProperties.m_friction),
	m_restitution(physicsProperties.m_restitution),
	m_linearDamping(physicsProperties.m_linearDamping),
	m_angularDamping(physicsProperties.m_angularDamping),
	m_angularFactor(physicsProperties.m_angularFactor),
	m_linearFactor(physicsProperties.m_linearFactor)
{
	if(physicsProperties.m_constraint)
		m_constraint = new MPhysicsConstraint(*physicsProperties.m_constraint);
}

MPhysicsProperties::~MPhysicsProperties(void)
{
	deleteConstraint();
}

void MPhysicsProperties::deleteConstraint(void)
{
	SAFE_DELETE(m_constraint);
}

MPhysicsConstraint * MPhysicsProperties::createConstraint(void)
{
	deleteConstraint();
	m_constraint = new MPhysicsConstraint();
	return m_constraint;
}


MOEntity::MOEntity(MMeshRef * meshRef):
MObject3d(),
m_needToUpdateCache(true),
m_subMeshCachesNumber(0),
m_subMeshCaches(NULL),
m_isInvisible(false),
m_currentLoop(0),
m_animationId(0),
m_animationSpeed(1),
m_currentFrame(0),
m_physicsProperties(NULL)
{
	setMeshRef(meshRef);
}

MOEntity::MOEntity(const MOEntity & entity):
MObject3d(entity),
m_needToUpdateCache(true),
m_subMeshCachesNumber(0),
m_subMeshCaches(NULL),
m_isInvisible(entity.m_isInvisible),
m_currentLoop(entity.m_currentLoop),
m_animationId(entity.m_animationId),
m_animationSpeed(entity.m_animationSpeed),
m_currentFrame(entity.m_currentFrame),
m_physicsProperties(NULL)
{
	setMeshRef(entity.m_meshRef);
	if(entity.m_physicsProperties)
		m_physicsProperties = new MPhysicsProperties(*entity.m_physicsProperties);
}

MOEntity::~MOEntity(void)
{
	clearPrivateMaterials();
	clearSubMeshCaches();
	deletePhysicsProperties();
	MObject3d::clearObject3d();
}

void MOEntity::changeAnimation(unsigned int animationId)
{
	MMesh * mesh = getMesh();
	if(! mesh)
	{
		m_animationId = animationId;
		return;
	}

	if(animationId >= mesh->getAnimsRangesNumber())
		return;

	bool init = false;
	
	// change anim
	if(animationId != m_animationId)
	{
		m_animationId = animationId;
		init = true;
	}
	else
	{
		// or restart stopped anim
		MAnimRange * animRange = &mesh->getAnimsRanges()[m_animationId];
		if(m_currentLoop == animRange->loops)
			init = true;
	}
	
	if(init)
	{
		MAnimRange * animRange = &mesh->getAnimsRanges()[m_animationId];
		
		if(m_animationSpeed >= 0)
			m_currentFrame = animRange->start;
		else
			m_currentFrame = animRange->end;
		
		m_currentLoop = 0;
	}
}

void MOEntity::deletePhysicsProperties(void)
{
	SAFE_DELETE(m_physicsProperties);
}

MPhysicsProperties * MOEntity::createPhysicsProperties(void)
{
	deletePhysicsProperties();
	MPhysicsProperties * physicsProperties = new MPhysicsProperties();
	m_physicsProperties = physicsProperties;
	return physicsProperties;
}

void MOEntity::setActive(bool active)
{
	m_isActive = active;
}

void MOEntity::setMeshRef(MMeshRef * meshRef)
{
	// set mesh ref
	m_meshRef = meshRef;
	if(meshRef)
	{
		MMesh * mesh = meshRef->getMesh();
		if(mesh)
		{
			// copy nounding box
			m_boundingBox = (*mesh->getBoundingBox());
			
			// armature
			if(mesh->getArmature())
				return;
		}
	}
	
	clearSubMeshCaches();
}

void MOEntity::clearSubMeshCaches(void)
{
	m_subMeshCachesNumber = 0;
	SAFE_DELETE_ARRAY(m_subMeshCaches);
}

bool MOEntity::getRayNearestIntersectionDistance(const MVector3 & origin, const MVector3 & dest, float * distance)
{
	MMesh * mesh = getMesh();
	if(! mesh)
		return false;

	MMatrix4x4 * matrix = getMatrix();
	MMatrix4x4 iMatrix = matrix->getInverse();

	MVector3 localOrigin = iMatrix * origin;
	MVector3 localDest = iMatrix * dest;

	MBox3d * ebox = getBoundingBox();
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
		MVector3 * vertices = subMesh->getVertices();
		MBox3d * box = subMesh->getBoundingBox();
		
		if(getSubMeshCachesNumber() == sSize)
		{
			MSubMeshCache * subMeshCache = &getSubMeshCaches()[s];
			vertices = subMeshCache->getVertices();
			box = subMeshCache->getBoundingBox();
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

MMaterial * MOEntity::createPrivateMaterial(unsigned int id)
{
	map<unsigned int, MMaterial*>::iterator iter = m_materials.find(id);
    if(iter != m_materials.end())
    {
        return iter->second;
    }
	else
	{
		MMaterial * material, * meshMaterial = getMaterial(id);
		if(meshMaterial)
			material = new MMaterial(*meshMaterial);
		else
			material = new MMaterial();
		m_materials[id] = material;
		return material;
	}
}

MMaterial * MOEntity::getMaterial(unsigned int id)
{
	map<unsigned int, MMaterial*>::iterator iter = m_materials.find(id);
	if(iter != m_materials.end())
    {
        return iter->second;
    }
	
	if(m_meshRef)
	{
		MMesh * mesh = m_meshRef->getMesh();
		if(mesh)
		{
			if(id < mesh->getMaterialsNumber())
				return mesh->getMaterial(id);
		}
	}
	
	return NULL;
}

void MOEntity::deletePrivateMaterial(unsigned int id)
{
	map<unsigned int, MMaterial*>::iterator iter = m_materials.find(id);
    if(iter != m_materials.end())
    {
        SAFE_DELETE(iter->second);
		m_materials.erase(iter);
    }
}

void MOEntity::clearPrivateMaterials(void)
{
	map<unsigned int, MMaterial*>::iterator
		mit (m_materials.begin()),
		mend(m_materials.end());
		
	for(; mit!=mend; mit++)
		SAFE_DELETE(mit->second);
	
	m_materials.clear();
}

MMesh * MOEntity::getMesh(void)
{
	if(! m_meshRef)
		return NULL;

	return m_meshRef->getMesh();
}

void MOEntity::updateVisibility(MOCamera * camera)
{
	if(m_isInvisible){
		setVisible(false);
		return;
	}

	MFrustum * frustum = camera->getFrustum();

	MVector3 * min = &m_boundingBox.min;
	MVector3 * max = &m_boundingBox.max;

	MVector3 points[8] = {
		getTransformedVector(MVector3(min->x, min->y, min->z)),
		getTransformedVector(MVector3(min->x, max->y, min->z)),
		getTransformedVector(MVector3(max->x, max->y, min->z)),
		getTransformedVector(MVector3(max->x, min->y, min->z)),
		getTransformedVector(MVector3(min->x, min->y, max->z)),
		getTransformedVector(MVector3(min->x, max->y, max->z)),
		getTransformedVector(MVector3(max->x, max->y, max->z)),
		getTransformedVector(MVector3(max->x, min->y, max->z))
	};

	// is box in frustum
	setVisible(frustum->isVolumePointsVisible(points, 8));
}

bool MOEntity::isAnimationOver(void)
{
	MMesh * mesh = getMesh();
	if(mesh)
	{
		if(mesh->getAnimsRangesNumber() > 0)
		{
			MAnimRange * animRange = &mesh->getAnimsRanges()[m_animationId];
			
			// end loops
			if(animRange->loops >= 0 && m_currentLoop == animRange->loops)
				return true;
			
			// next frame
			float nextFrame = m_currentFrame + m_animationSpeed;
			
			if(m_animationSpeed > 0)
				return (nextFrame >= (animRange->end + 1));
			else
				return (nextFrame <= animRange->start);
		}
	}
	
	return false;
}

void MOEntity::updateCache(void)
{
	if(! m_needToUpdateCache)
		return;

	MMesh * mesh = getMesh();
	if(! mesh)
		return;
		
	MArmature * armature = mesh->getArmature();
	if(! armature)
		return;
		
		
	// update armature
	if(mesh->getArmatureAnim())
	{
		animateArmature(
			mesh->getArmature(),
			mesh->getArmatureAnim(),
			m_currentFrame
		);
	}
	else
	{
		armature->processBonesLinking();
		armature->updateBonesSkinMatrix();
	}
		
		
	// update cache
	unsigned int s, sSize = mesh->getSubMeshsNumber();
	
	if(m_subMeshCachesNumber != sSize)
	{
		// alloc cache
		SAFE_DELETE(m_subMeshCaches);
		m_subMeshCachesNumber = sSize;
		if(m_subMeshCachesNumber > 0)
			m_subMeshCaches = new MSubMeshCache[m_subMeshCachesNumber];
	}
	
	for(s=0; s<sSize; s++)
	{
		MSubMesh * subMesh = &mesh->getSubMeshs()[s];
		MSubMeshCache * subMeshCache = &getSubMeshCaches()[s];

		if(! subMesh->getVertices() || ! subMesh->getSkinData())
			continue;
			
		MVector3 * vertices = subMesh->getVertices();
		MVector3 * normals = subMesh->getNormals();
		MVector3 * tangents = subMesh->getTangents();
		
		unsigned int verticesSize = subMesh->getVerticesSize();
		unsigned int normalsSize = subMesh->getNormalsSize();
		unsigned int tangentsSize = subMesh->getTangentsSize();

		MVector3 * skinVertices = subMeshCache->allocVertices(verticesSize);
		MVector3 * skinNormals = subMeshCache->allocNormals(normalsSize);
		MVector3 * skinTangents = subMeshCache->allocTangents(tangentsSize);
		
		MSkinData * skinData = subMesh->getSkinData();
		computeSkinning(armature, skinData, vertices, normals, tangents, skinVertices, skinNormals, skinTangents);
		
		subMesh->getBoundingBox()->initFromPoints(skinVertices, verticesSize);
		*subMeshCache->getBoundingBox() = *subMesh->getBoundingBox();
	}

	mesh->updateBoundingBox();
	m_boundingBox = *mesh->getBoundingBox();
	m_needToUpdateCache = false;
}

void MOEntity::update(void)
{
	MMesh * mesh = getMesh();
	if(mesh)
	{
		// anim
		if(mesh->getAnimsRangesNumber() > 0 && m_animationSpeed > 0)
		{
			MAnimRange * animRange = &mesh->getAnimsRanges()[m_animationId];

			// end loop
			if(animRange->loops >= 0 && m_currentLoop == animRange->loops)
				return;
			
			// next frame
			float nextFrame = m_currentFrame + m_animationSpeed;
		
			// update current frame
			if(animRange->loops < 0)
			{
				m_currentFrame = loopFloat(nextFrame, animRange->start, (animRange->end + 1));
			}
			else
			{
				if(isAnimationOver())
				{
					m_currentLoop++;
				}
				
				if(m_currentLoop < animRange->loops)
				{
					m_currentFrame = loopFloat(nextFrame, animRange->start, (animRange->end + 1));
				}
			}
		}
		
		m_needToUpdateCache = true;
		updateCache();
	}
}
