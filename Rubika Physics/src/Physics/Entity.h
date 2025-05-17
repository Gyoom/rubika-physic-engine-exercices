#ifndef ENTITY_H  
#define ENTITY_H  

#include "../Graphics.h"
#include "Vec2.h"
#include "Body.h"
#include <vector>  
#include <iostream>  


enum EntityType {
	ENTITY,
	BIRD,
	PIG,
	OBSTACLE,
};

struct Entity {  
  VerletBody* body;  
  EntityType type;

  Entity() = default;
  Entity(VerletBody* body, Vec2 pos, const char* textureFileName);
  ~Entity();
;
};  

#endif
