#include "Bird.h"

Bird::Bird(float size, Vec2 pos, const char* textureFileName)
{
	body = new VerletBody(CircleShape(size), pos.x, pos.y, 1, 1, true);
	body->SetTexture(textureFileName);
	isFlying = false;
	isDragging = false;
	stretchingMultiplier = 500.f;
	birdSize = size;
}

Bird::~Bird()
{
	delete body;
}

void Bird::RenderTrail() {
	if (!body)
		return;

}
void Bird::RenderPreview() {
	if (isDragging)
		return;
}
