#include "stdafx.h"
#include "Triangle.h"



Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, Material * m, glm::mat4 t, bool flip)
{
	hasVertNorms = false;
	A = a;
	B = b;
	C = c;
	material = m;
	transform = t;
	flipNormal = flip;
}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 aNorm, glm::vec3 bNorm, glm::vec3 cNorm, Material * m, glm::mat4 t, bool flip)
{
	hasVertNorms = true;
	A = a;
	B = b;
	C = c;
	Anorm = aNorm;
	Bnorm = bNorm;
	Cnorm = cNorm;
	material = m;
	transform = t;
	flipNormal = flip;
}

Material* Triangle::getMat() {
	return material;
}

glm::mat4 Triangle::getTransform() {
	return transform;
}

glm::vec3 Triangle::getNormal(glm::vec3 intersectionPoint)
{
	if (!hasVertNorms)
		return planeNormal();

	// Check to see if there is too great an angle between vertnorms, to minimize problems around edges
	// Assuming vertnorms in obj file are already normalized
	/*float aDotB = glm::dot(Anorm, Bnorm);
	float bDotC = glm::dot(Bnorm, Cnorm);
	float cDotA = glm::dot(Cnorm, Anorm);*/

	glm::vec3 normal = normalize(alpha * Anorm + beta * Bnorm + gamma * Cnorm);

	//if (aDotB < .17 || bDotC < .17 || cDotA < .17) // angle is > 80 degrees, so reduce the normal and thus make pixel darker
	//	normal *= .01f;

	return normal;


}
glm::vec3 Triangle::getNormal()
{
	return getNormal(glm::vec3());
}

glm::vec3 Triangle::planeNormal() {
	return flipNormal ? glm::normalize(glm::cross(C - A, B - A)) : glm::normalize(glm::cross(B - A, C - A));
}

float Triangle::Intersect(Ray * r) {

	float t;
	
	glm::vec3 N = glm::normalize(planeNormal());
	
	float NdotRayDirection = glm::dot(N, r->p1);

	if (fabs(NdotRayDirection) < .0000001) // almost 0
		return FLT_MAX; // they are parallel so they don't intersect !
	
	t = (glm::dot(A, N) - glm::dot(r->p0, N)) / NdotRayDirection;
	
	if (t < 0) return FLT_MAX; // the triangle is behind
	
	glm::vec3 P = r->p0 + (t * r->p1);

	float denom = glm::dot(glm::cross(B - A, C - A), N);

	gamma = glm::dot(glm::cross(B - A, P - A), N) / denom;
	
	if (gamma < 0)
		return FLT_MAX;

	beta = glm::dot(glm::cross(A - C, P - C), N) / denom;

	if (beta < 0)
		return FLT_MAX;

	alpha = glm::dot(glm::cross(C - B, P - B), N) / denom;

	return alpha > 0 ? t : FLT_MAX; 
}

glm::vec3 Triangle::getMidPoint() {
	return glm::vec3((A.x + B.x + C.x) / 3, (A.y + B.y + C.y) / 3, (A.z + B.z + C.z) / 3);
}

BoundingBox Triangle::getBoundingBox() {
	float xMin = glm::min(glm::min(A.x, B.x), C.x);
	float yMin = glm::min(glm::min(A.y, B.y), C.y);
	float zMin = glm::min(glm::min(A.z, B.z), C.z);
	float xMax = glm::max(glm::max(A.x, B.x), C.x);
	float yMax = glm::max(glm::max(A.y, B.y), C.y);
	float zMax = glm::max(glm::max(A.z, B.z), C.z);

	return BoundingBox(xMin, yMin, zMin, xMax, yMax, zMax);
}

Triangle::~Triangle()
{
}
