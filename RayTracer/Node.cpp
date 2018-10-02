
#include "Node.h"
#define MAX_TREE_DEPTH 5

Node::Node()
{
	
}

Node * Node::buildTree( std::vector<Triangle*> triangles, int depth)
{
	
	Node * node =  new Node();
	node->tris = triangles;
	node->bbox = BoundingBox();

	if (triangles.size() == 0)
		return node;

	node->bbox = triangles[0]->getBoundingBox();

	for (int i = 1, len = triangles.size(); i < len; i++) {
		node->bbox.expand(triangles[i]->getBoundingBox());
	}

	if (/*depth == MAX_TREE_DEPTH ||*/ triangles.size() < 50) {
		// this is a leaf
		return node;
	}
	glm::vec3 midpoint(0, 0, 0);

	for (int i = 0, len = triangles.size(); i < len; i++) {
		midpoint += triangles[i]->getMidPoint();
	}

	midpoint /= triangles.size();

	std::vector<Triangle*> childTris[8];

	for (int i = 0, len = triangles.size(); i < len; i++){
		glm::vec3 midpt = triangles[i]->getMidPoint();
		if (midpt.x > midpoint.x) {
			if (midpt.y > midpoint.y) {
				if (midpt.z > midpoint.z) {
					childTris[0].push_back(triangles[i]);
				}
				else {
					childTris[1].push_back(triangles[i]);
				}
			}
			else {
				if (midpt.z > midpoint.z) {
					childTris[2].push_back(triangles[i]);
				}
				else {
					childTris[3].push_back(triangles[i]);
				}
			}
		}
		else {
			if (midpt.y > midpoint.y) {
				if (midpt.z > midpoint.z) {
					childTris[4].push_back(triangles[i]);
				}
				else {
					childTris[5].push_back(triangles[i]);
				}
			}
			else {
				if (midpt.z > midpoint.z) {
					childTris[6].push_back(triangles[i]);
				}
				else {
					childTris[7].push_back(triangles[i]);
				}
			}
		}
	}	
	for (int i = 0; i < 8; i++) {
		node->children[i] = Node::buildTree(childTris[i], depth + 1);
	}
	return node;
}

std::vector<Triangle*> Node::hit(Ray * r, Node * root, int depth)
{
	if (!root->bbox.intersect(r)) {
		return std::vector<Triangle*>(0);;
	}

	if (root->tris.size() >= 50 /*&& depth < MAX_TREE_DEPTH*/) {
		std::vector<Triangle*> returnTris(0);

		for (int i = 0; i < 8; i++) {
			std::vector<Triangle*> currentTris = Node::hit(r, root->children[i], depth + 1);
			for (int j = 0, len = currentTris.size(); j < len; j++) {
				returnTris.push_back(currentTris[j]);
			}
		}
		return returnTris;
	}
	else {
		return root->tris;
	}
}

Node::~Node()
{
}
