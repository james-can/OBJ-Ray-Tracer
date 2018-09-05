#pragma once
#include "Ray.h"
class BoundingBox
{
	glm::vec3 bounds[2];
public:
	float xMax, yMax, zMax, xMin, yMin, zMin;
	BoundingBox();
	BoundingBox(float x, float y, float z, float X, float Y, float Z);

	int longest_axis();
	void expand(BoundingBox bb);
	bool intersect(Ray * r);
	bool hit(Ray * r);
	~BoundingBox();
};

