#pragma once
#include "glm/glm.hpp"
class Camera
{
	glm::vec3 at;
	glm::vec3 up;
public:
	glm::vec3 from;
	float fovY;
	float fovX;
	glm::vec3 u;
	glm::vec3 v;
	glm::vec3 w;
	Camera(float fovy, glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 upVec);
	~Camera();
};
////Cylinder(double r, double h) : base (r), height(h) {}

//float fovX = atan(tan(fovY / 2.)*(float(width) / float(height))) * 2.;