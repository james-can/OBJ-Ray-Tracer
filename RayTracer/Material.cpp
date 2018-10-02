

#include "Material.h"

Material::Material(std::string n, glm::vec3 amb, glm::vec3 emm, glm::vec3 dif, glm::vec3 spec, float r, float shin, float rNess) {
	hasTexture = false;
	emissive = emm;
	diffuse = dif;
	specular = spec;
	shininess = shin;
	ambient = amb;
	refractiveIndex = r;
	refractiveness = rNess;
	name = n;
}
Material::Material(std::string n, glm::vec3 amb, glm::vec3 emm, glm::vec3 dif, glm::vec3 spec, float r, float shin, float rNess, texture * t) {
	hasTexture = true;
	tex = t;
	emissive = emm;
	diffuse = dif;
	specular = spec;
	shininess = shin;
	ambient = amb;
	refractiveIndex = r;
	refractiveness = rNess;
	name = n;
}
Material::Material() {

}