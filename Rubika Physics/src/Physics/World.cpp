#include "World.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "../Graphics.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity;
}

World::~World() {
    for (auto body: bodies) {
        delete body;
    }
    delete bird;
	SDL_DestroyTexture(bgTexture);
    for (auto body : obstacles) {
        delete body;
    }
	for (auto body : pigs) {
		delete body;
	}
}

void World::AddBody(Body* body) {
    bodies.push_back(body);
}

std::vector<Body*>& World::GetBodies() {
    return bodies;
}

void World::AddForce(const Vec2& force) {
    forces.push_back(force);
}

void World::AddTorque(float torque) {
    torques.push_back(torque);
}

void World::bodiesUpdate(float dt) {
    //this->AddTorque(20.f);

   // Loop all bodies of the world applying forces
    for (auto body : bodies) {
        // Apply the weight force to all bodies
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);

        // Apply global forces (wind etc.) to all bodies
        for (auto force : forces) {
            body->AddForce(force);
        }

        // Apply global torque (wind etc.) to all bodies
        for (auto torque : torques) {
            body->AddTorque(torque);
        }
    }

    // Update all the bodies in the world (integrating and transforming vertices)

    for (auto body : bodies) {
        body->Update(dt);
        body->ClearForces();
    }
    for (int i = 0; i < 5; ++i) { // sim_iteration_count
        for (auto body : bodies) {
            if (body->type == VERLET_BODY) {
                VerletBody* vBody = (VerletBody*)body;
                vBody->handleCollision();
            }
        }
        for (auto body : bodies) {
            if (body->type == VERLET_BODY) {
                VerletBody* vBody = (VerletBody*)body;
                vBody->applyContraints();
            }
        }
    }
    for (auto body : bodies) {
        if (body->type == VERLET_BODY) {
            VerletBody* vBody = (VerletBody*)body;
            vBody->recalculateCenter();
            vBody->recalculateVertices();
        }
    }
    CheckCollisions();
}

void World::Update(float dt) 
{
	World::bodiesUpdate(dt);
	bird->Update(dt, this);
}

void World::VerletCollisionsEffects(VerletBody* a, VerletBody* b)
{
	// Bird/Pig collision
    if (a->entity->type == BIRD && b->entity->type == PIG) 
    {
        pigs.erase(std::remove(pigs.begin(), pigs.end(), b->entity), pigs.end());
        bodies.erase(std::remove(bodies.begin(), bodies.end(), b), bodies.end());
        delete b->entity;
        delete b;

		pigKilled++;
        return;
	}
    else if (a->entity->type == PIG && b->entity->type == BIRD) 
    {
        pigs.erase(std::remove(pigs.begin(), pigs.end(), a->entity), pigs.end());
        bodies.erase(std::remove(bodies.begin(), bodies.end(), a), bodies.end());
        delete a->entity;
        delete a;

        pigKilled++;
		return;
    }
	// Obstacle/Pig collision
	else if (a->entity->type == OBSTACLE && b->entity->type == PIG) {
		Vec2 aDir = (a->points[0].pos - a->points[0].oldPos).Normalize();
		Vec2 bDir = (b->points[0].pos - b->points[0].oldPos).Normalize();
		float aVelocity = abs(a->points[0].pos.Dist(a->points[0].oldPos));
		bool bDamaged = aVelocity > 3.f;

        if (bDamaged && aDir.Dot(bDir) < 0.1f) 
        {
			bodies.erase(std::remove(bodies.begin(), bodies.end(), b), bodies.end());
		    pigs.erase(std::remove(pigs.begin(), pigs.end(), b->entity), pigs.end());
			delete b->entity;
			delete b;
            pigKilled++;
			return;
        }

	}
    else if (a->entity->type == PIG && b->entity->type == OBSTACLE) 
    {
        Vec2 aDir = (a->points[0].pos - a->points[0].oldPos).Normalize();
        Vec2 bDir = (b->points[0].pos - b->points[0].oldPos).Normalize();
		float bVelocity = abs(b->points[0].pos.Dist(b->points[0].oldPos));
		bool aDamaged = bVelocity > 3.f;

        if (aDamaged && bDir.Dot(aDir) < 0.1f) 
        {
			bodies.erase(std::remove(bodies.begin(), bodies.end(), a), bodies.end());
			pigs.erase(std::remove(pigs.begin(), pigs.end(), a->entity), pigs.end());
			delete a->entity;
			delete a;
			pigKilled++;
			return;
        }
    }
	// Bird/Obstacle collision
    else if (a->entity->type == BIRD && b->entity->type == OBSTACLE) 
    {
    
    }
    else if (a->entity->type == OBSTACLE && b->entity->type == BIRD) 
    {

    }
}

void World::CheckCollisions() { // Narrow type only
    // Check all the bodies with all other bodies detecting collisions
    // !!! ne prend pas en compte si le body est tellement rapide qu'il le traverse entre 2 frames sans jamais lui être superposé -> raycast preview pour gérer ça  
    for (int i = 0; i <= bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) { // i + 1 evite de tester 2 fois les collision de 2 points de vie différent A sur B V, B sur A X
            Body* a = bodies[i];
            Body* b = bodies[j];
            a->isColliding = false;
            b->isColliding = false;

            Contact contact;
            if (CollisionDetection::IsColliding(a, b, contact)) {
                // Resolve the collision
                contact.ResolveCollision();

                a->isColliding = true;
                b->isColliding = true;

                //Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                //Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFFFFFF);
                //Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFFFFFF);

				VerletBody* vBodyA = (VerletBody*)a;
				VerletBody* vBodyB = (VerletBody*)b;

                if (vBodyA && vBodyB) 
					World::VerletCollisionsEffects(vBodyA, vBodyB);
            }
        }
    }
}
