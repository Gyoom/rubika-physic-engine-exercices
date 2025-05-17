#ifndef OBSTACLE_H  
#define OBSTACLE_H  

#include "Entity.h" 

enum ObstacleType {
	STONE,
	WOOD,
	GLASS,
};


struct Obstacle : Entity {  
	ObstacleType oType;
	Obstacle() = default;
	Obstacle(VerletBody* body, const char* textureFileName, ObstacleType oType);
	~Obstacle();
};  

#endif
