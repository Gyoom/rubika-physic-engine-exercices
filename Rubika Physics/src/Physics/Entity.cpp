#include "Entity.h"
#include "../Graphics.h"
#include "Body.h"

Entity::Entity(VerletBody* body, Vec2 pos, const char* textureFileName)
{
	this->body = body;
	body->SetTexture(textureFileName);
	type = ENTITY;
}

Entity::~Entity()
{
	//delete body; // already delete in vector bodies of world.h
}
