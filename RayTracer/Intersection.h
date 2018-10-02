#pragma once
#include "Material.h"
#include "Ray.h"
#include "Triangle.h"
#include "Sphere.h"
#include "glm\glm.hpp"
#include <vector>

class Intersection
{
public:
	glm::vec3 incidentRayDirection;
	glm::vec3 rayOrigin;
	glm::vec3 intersectPoint;
	glm::vec2 textureCoord = glm::vec2(1, 1);
	glm::vec3 normal;
	bool isHit;
	Material * myMat;
	Intersection(Ray * r, std::vector<Triangle*> objs, float initMin);
	Intersection(Ray * r, std::vector<Triangle*> objs, float initMin, std::vector<SceneObject*> helpers);
	~Intersection();
};
