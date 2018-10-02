
#include "Camera.h"


Camera::Camera(float fovy, glm::vec3 lookFrom, glm::vec3 lookAt, glm::vec3 upVec)
{
	from = lookFrom;
	at = lookAt;
	up = upVec;
	fovY = fovy;
	glm::vec3 eyeDir = from - at;
	w = normalize(eyeDir);
	u = normalize(cross(up, w));
	v = (cross(w, u));
}


Camera::~Camera()
{
}
