#pragma once
#include "SceneObject.h"
#include "Ray.h"
class Sphere :
	public SceneObject

{
	Material * material;
	glm::mat4 transform;
public:
	float radius;
	glm::vec3 center;
	Material* getMat();
	glm::mat4 getTransform();
	Sphere(float r, glm::vec3 c, Material * m, glm::mat4 t);
	Sphere(float r, glm::vec3 c, glm::mat4 trans);
	Sphere(float r, glm::vec3 c);
	float Intersect(Ray*);
	glm::vec3 getNormal(glm::vec3 intersectPoint);
	~Sphere();
};

////Cylinder(double r, double h) : base (r), height(h) {}

