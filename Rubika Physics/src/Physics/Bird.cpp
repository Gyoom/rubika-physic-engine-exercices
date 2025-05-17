#include "Bird.h"
#include "../Graphics.h"
#include "Body.h"
#include "World.h"

Bird::Bird(float size, Vec2 pos, const char* textureFileName, Vec2 catapultPos)
{
	body = new VerletBody(CircleShape(size), pos.x, pos.y, 1, 1, true);
	body->SetTexture(textureFileName);
	isFlying = false;
	isDragging = false;
	stretchingMultiplier = 500.f;
	birdSize = size;
	this->catapultPos = catapultPos;
	catapultBackPos = catapultPos + Vec2(30, 10);
	catapultFrontPos = catapultPos + Vec2(-30, 10);
	maxStretching = Graphics::Width() * 0.15f;
	type = BIRD;
}

Bird::~Bird()
{
	//delete body; // already delete in vector bodies of world.h
}

void Bird::RenderTrail() {
	if (!body)
		return;

}
void Bird::RenderStretching(bool back)
{
	Vec2 pos;
	Uint32 color;
	std::vector<Vec2> vertices;

	if (!isDragging && back)
	{
		pos = catapultPos;
		color = 0xF9000000;
		vertices.push_back(catapultBackPos + Vec2(0, -10));
		vertices.push_back(catapultBackPos + Vec2(0, 10));
		vertices.push_back(catapultFrontPos + Vec2(0, 10));
		vertices.push_back(catapultFrontPos + Vec2(0, -10));
	}
	else if (isDragging){
		
		if (back)
		{
			pos = catapultBackPos;
			color = 0xF9222222;
		}
		else
		{
			pos = catapultFrontPos;
			color = 0xF9000000;
		}

		vertices.push_back(pos + Vec2(0, -10));
		vertices.push_back(pos + Vec2(0, 10));

		Vec2 bodyPos = body->position - catapultPos;
		bodyPos = bodyPos.Normalize();

		bodyPos *= ((CircleShape*) body->shape)->radius;
		bodyPos += body->position;

		vertices.push_back(bodyPos + Vec2(5, 5));
		vertices.push_back(bodyPos + Vec2(0, -5));	
	}
	else
		return;

	Graphics::DrawFillPolygon(pos.x, pos.y, vertices, color);

	
}

void Bird::Update(float dt, World* world)
{

	if (isFlying) {
		float velocity = abs(body->points[0].pos.Dist(body->points[0].oldPos));
		if (velocity < 1.f) {
			isFlying = false;
			world->birdThrow++;
			if (world->birdThrow < world->birdMax) 
			{
				body = new VerletBody(CircleShape(birdSize), catapultPos.x, catapultPos.y, 1, 1, true);
				body->SetTexture("assets/angrybirds/bird-red.png");
				world->AddBody(body);
				body->entity = this;
			}
		}
	}
}
void Bird::RenderPreview() {
	if (isDragging)
		return;
}
