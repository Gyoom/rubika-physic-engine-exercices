#include "Pig.h"


Pig::Pig(float size, Vec2 pos, const char* textureFileName)
{
	body = new VerletBody(CircleShape(size), pos.x, pos.y, 1, 1, false);
	body->SetTexture(textureFileName);
	type = PIG;
}

Pig::~Pig()
{
	//delete body; // already delete in vector bodies of world.h
}
