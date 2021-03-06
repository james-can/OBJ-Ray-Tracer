#pragma once
#include "glm\glm.hpp"
#include <string>
#include "texture.h"

//#include "FreeImage.h"

////Cylinder(double r, double h) : base (r), height(h) {}
struct Material {
	bool hasTexture;
	texture * tex;
	std::string name;
	glm::vec3 **difftextureVals;
	float refractiveIndex;
	float refractiveness = 0;
	glm::vec3 emissive;
	glm::vec3 diffuse;
	glm::vec3 specular;
	glm::vec3 ambient;
	glm::vec2 diffuseTextureSize;
	float shininess;
	Material(std::string n, glm::vec3 amb, glm::vec3 emm, glm::vec3 dif, glm::vec3 spec, float r, float shin, float rNess);
	Material(std::string n, glm::vec3 amb, glm::vec3 emm, glm::vec3 dif, glm::vec3 spec, float r, float shin, float rNess, texture * t);
	Material();
};
struct Light {
	glm::vec4 pos;
	glm::vec3 col;
	glm::vec3 atten;
	Light(glm::vec4 p, glm::vec3 c, glm::vec3 a) : pos(p), col(c), atten(a) {}
};