

// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"


// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
	float r = degrees * (pi / 180.0);
	vec3 a = normalize(axis);

	mat3 identity = mat3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
	mat3 secondTerm = (1.0f - cos(r)) * mat3(
		pow(a.x, 2), a.x*a.y, a.x*a.z,
		a.x*a.y, pow(a.y, 2), a.y*a.z,
		a.x*a.z, a.y*a.z, pow(a.z, 2));
	mat3 thirdTerm = sin(r) * mat3(
		0, a.z, -a.y,
		-a.z, 0, a.x,
		a.y, -a.x, 0);


	return cos(r) * identity + secondTerm + thirdTerm;
}

void Transform::left(float degrees, vec3& eye, vec3& up)
{
	mat3 rot = rotate(degrees, up);
	eye = rot * eye;
}

void Transform::up(float degrees, vec3& eye, vec3& up)
{
	vec3 u = cross(normalize(eye), normalize(up));
	mat3 rot = rotate(degrees, (u));

	eye = rot * eye;
	up = rot * up;
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up)
{
	vec3 eyeDir = eye - center;
	vec3 w = normalize(eyeDir);
	vec3 u = normalize(cross(up, w));
	vec3 v = normalize(cross(w, u));

	vec4 row1(u, -dot(eyeDir, u));
	vec4 row2(v, -dot(eyeDir, v));
	vec4 row3(w, -dot(eyeDir, w));
	vec4 row4(0.0, 0.0, 0.0, 1.0);

	return transpose(mat4(row1, row2, row3, row4));
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
	float d = 1 / glm::tan(glm::radians(fovy) / 2);
	float A = -((zFar + zNear) / (zFar - zNear));
	float B = -(2 * zFar*zNear / (zFar - zNear));
	vec4 r1(d / aspect, 0, 0, 0);
	vec4 r2(0, d, 0, 0);
	vec4 r3(0, 0, A, B);
	vec4 r4(0, 0, -1, 0);
	return transpose(mat4(r1, r2, r3, r4));
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz)
{
	vec4 r1(sx, 0, 0, 0);
	vec4 r2(0, sy, 0, 0);
	vec4 r3(0, 0, sz, 0);
	vec4 r4(0, 0, 0, 1);
	return mat4(r1, r2, r3, r4);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz)
{
	vec4 r1(1, 0, 0, tx);
	vec4 r2(0, 1, 0, ty);
	vec4 r3(0, 0, 1, tz);
	vec4 r4(0, 0, 0, 1);
	//return glm::translate(tx, ty, tz);
	return transpose(mat4(r1, r2, r3, r4));
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec)
{
	vec3 x = glm::cross(up, zvec);
	vec3 y = glm::cross(zvec, x);
	vec3 ret = glm::normalize(y);
	return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
