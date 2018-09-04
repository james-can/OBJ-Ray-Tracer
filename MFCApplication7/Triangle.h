#pragma once
#include "SceneObject.h"
#include "BoundingBox.h"
class Triangle /*:
	public SceneObject*/
{
	Material * material;
	glm::mat4 transform;

	// 3 vertices of triangle
	glm::vec3 A;
	glm::vec3 B;
	glm::vec3 C;

	glm::vec3 Anorm = glm::vec3(0,0,0);
	glm::vec3 Bnorm = glm::vec3(0, 0, 0);;
	glm::vec3 Cnorm = glm::vec3(0, 0, 0);;

	float alpha = 0, beta = 0, gamma = 0;


	bool hasVertNorms;

	bool flipNormal;
public:
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, Material * m, glm::mat4 t, bool flip);
	Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 aNorm, glm::vec3 bNorm, glm::vec3 cNorm, Material * m, glm::mat4 t, bool flip);
	Material * getMat();
	glm::mat4 getTransform();
	glm::vec3 getNormal();
	glm::vec3 planeNormal();
	glm::vec3 getNormal(glm::vec3 intersectionPoint);
	float Intersect(Ray * r);
	glm::vec3 getMidPoint();
	BoundingBox getBoundingBox();
	~Triangle();
};

