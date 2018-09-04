#pragma once
#include "Camera.h"
#include "glm\glm.hpp"
class Ray
{
public:
	glm::vec3 p0;
	glm::vec3 p1;
	Ray(Camera * cam, float i, float j, int width, int height);
	Ray(glm::vec3 orig, glm::vec3 dir);
	Ray();
	~Ray();
	
};

