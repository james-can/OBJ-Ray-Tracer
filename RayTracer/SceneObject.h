#pragma once
#include "glm/glm.hpp"

#include "Ray.h"

#include "Material.h"
class SceneObject
{
	//float radius;
public:
	SceneObject();
	virtual glm::mat4 getTransform() = 0;
	virtual Material* getMat() = 0;
	virtual float Intersect(Ray*) = 0;
	virtual glm::vec3 getNormal(glm::vec3 intersectPoint) = 0;
	~SceneObject();
};


