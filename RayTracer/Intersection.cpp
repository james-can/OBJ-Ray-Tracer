
#include <vector>
#include<iostream>


#include "Intersection.h"
Intersection::Intersection(Ray * r, std::vector<Triangle*> objs, float initMin) {
	this->~Intersection();
	std::vector<SceneObject*> helperObjs;
	helperObjs.resize(0);
	new (this) Intersection(r, objs, initMin, helperObjs);
}
Intersection::Intersection(Ray * r, std::vector<Triangle*> objs, float initMin, std::vector<SceneObject*> helpers  )
{
	incidentRayDirection = ( r->p1);

	// Public variable needed by main for recursion
	rayOrigin = r->p0;

	// For shadows, we don't want to hit objects behind the light
	float mindist = initMin; 

	Triangle * hitobject = NULL;
	float t = 0.;
	
	for(unsigned int i = 0; i < objs.size(); i++){  // Find closest intersection; test all objects
		t = objs[i]->Intersect(r);
		
		if (t > 0 && t < mindist) { // closer than previous closest object
			mindist = t;
			hitobject = objs[i];
			intersectPoint = r->p0 + (r->p1 * mindist);
		}
	}

	if (mindist == initMin) {
		isHit = false;
		return;
	}
	else {
		isHit = true;
	}

	normal = (hitobject->getNormal(intersectPoint));
	textureCoord = hitobject->getTextureCoord();
	myMat = hitobject->getMat();
}

Intersection::~Intersection()
{
}
