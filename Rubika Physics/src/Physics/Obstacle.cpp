#include "Obstacle.h"

Obstacle::Obstacle(VerletBody* body, const char* textureFileName, ObstacleType oType)
{
	this->body = body;
	body->SetTexture(textureFileName);
	type = OBSTACLE;
	this->oType = oType;
}

Obstacle::~Obstacle()
{
	//delete body; // already delete in vector bodies of world.h
}
