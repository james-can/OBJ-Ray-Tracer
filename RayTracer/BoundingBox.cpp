#include "stdafx.h"
#include "BoundingBox.h"
#include "glm/glm.hpp"


BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(float x, float y , float z, float X, float Y, float Z)
{
	xMin = x;
	yMin = y;
	zMin = z;
	xMax = X;
	yMax = Y;
	zMax = Z;

	bounds[0] = glm::vec3(x, y, z);
	bounds[1] = glm::vec3(X, Y, Z);
}
void BoundingBox::expand(BoundingBox bb)
{
	xMin = glm::min(xMin, bb.xMin);
	yMin = glm::min(yMin, bb.yMin);
	zMin = glm::min(zMin, bb.zMin);

	xMax = glm::max(xMax, bb.xMax);
	yMax = glm::max(yMax, bb.yMax);
	zMax = glm::max(zMax, bb.zMax);

	bounds[0] = glm::vec3(xMin, yMin, zMin);
	bounds[1] = glm::vec3(xMax, yMax, zMax);
}

bool BoundingBox::intersect(Ray * r) {

	// r.dir is unit direction vector of ray
	glm::vec3 dirfrac;

	dirfrac.x = 1.0f / r->p1.x;
	dirfrac.y = 1.0f / r->p1.y;
	dirfrac.z = 1.0f / r->p1.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (bounds[0].x - r->p0.x)*dirfrac.x;
	float t2 = (bounds[1].x - r->p0.x)*dirfrac.x;
	float t3 = (bounds[0].y - r->p0.y)*dirfrac.y;
	float t4 = (bounds[1].y - r->p0.y)*dirfrac.y;
	float t5 = (bounds[0].z - r->p0.z)*dirfrac.z;
	float t6 = (bounds[1].z - r->p0.z)*dirfrac.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		//t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		//t = tmax;
		return false;
	}

	//t = tmin;
	return true;
}



BoundingBox::~BoundingBox()
{
}
