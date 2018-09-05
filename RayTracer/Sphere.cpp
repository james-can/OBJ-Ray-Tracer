#include "stdafx.h"
#include "Sphere.h"
#include "glm\glm.hpp"

Sphere::Sphere(float r, glm::vec3 c, Material * m, glm::mat4 t) {
	radius = r;
	center = c;
	material = m;
	transform = t;
}
Sphere::Sphere(float r, glm::vec3 c, glm::mat4 trans) {
	radius = r;
	center = c;
	material = new Material(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(.7, 1, .3), 1.4, 20., 1.0f);
	transform = trans;
}

Sphere::Sphere(float r, glm::vec3 c) {
	radius = r;
	center = c;
	material = new Material(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(.7,1,.3), 1.4, 20., 1.0f);
	transform = glm::mat4(1.);
}


Material* Sphere::getMat() {
	return material;
}

glm::mat4 Sphere::getTransform() {
	return transform;
}

float Sphere::Intersect(Ray * r) {
	float a = dot(r->p1, r->p1);
	float b = 2.0f * dot(r->p1,(r->p0 - center));
	float c = dot(r->p0 - center, r->p0 - center) - radius * radius;

	//delete r;

	float discriminate = (b * b - 4 * a * c );
	if (discriminate < 0)
		return FLT_MAX; 
	float r1 = (-b + sqrt(discriminate)) / (2 * a);
	float r2 = (-b - sqrt(discriminate)) / (2 * a);

	if (r1 == r2) 
		return FLT_MAX;
	else if ((r1 < 0 && r2 > 0) || (r2 < 0 && r1 > 0)) 
		return glm::max(r1, r2);
	else
		return glm::min(r1, r2);
}
	
glm::vec3 Sphere::getNormal(glm::vec3 intersectPoint)
{
	return intersectPoint - center;
}
Sphere::~Sphere()
{
}
