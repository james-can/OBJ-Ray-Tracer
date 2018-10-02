
#include "Ray.h"

const float PI = 3.14159265359;
float degToRad(float deg) {
	return deg *  PI / 180.;
}

// Camera Ray
Ray::Ray(Camera * cam, float i, float j, int width, int height)
{
	float fovX = atan(tan(degToRad(cam->fovY) / 2.)*(float(width) / float(height))) * 2.;
	float alpha = tan(fovX / 2.)*((j - width / 2.) / (width / 2.));
	float beta = tan(degToRad(cam->fovY) / 2.)*((height/2.-i)/(height/2.));

	p0 = cam->from;
	p1 = glm::normalize(alpha*cam->u + (beta * cam->v) - cam->w);
}

// Shadow / Reflective Ray
Ray::Ray(glm::vec3 orig, glm::vec3 dir) {
	p0 = orig;
	p1 = glm::normalize(dir);
}

Ray::Ray() {

}


Ray::~Ray()
{
	
}
