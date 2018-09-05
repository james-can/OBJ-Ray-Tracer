#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "Triangle.h"
#include "BoundingBox.h"
class Node
{
public:
	Node();
	glm::vec3 min;
	glm::vec3 max;
	Node * children [8];
	BoundingBox bbox;
	std::vector<Triangle*> tris;
	static Node * buildTree(std::vector<Triangle*> triangles, int depth);
	static std::vector<Triangle*> hit(Ray * r, Node * root, int depth);
	~Node();
};

