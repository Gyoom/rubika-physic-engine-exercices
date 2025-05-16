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

void World::ConcreteUpdate(float dt, std::vector<Body*> bodies)
{
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
	ConcreteUpdate(dt, birds);
	ConcreteUpdate(dt, obstacles);
    ConcreteUpdate(dt, pigs);
}

void World::CheckCollisions() { // Narrow type only
    // Check all the bodies with all other bodies detecting collisions
    // !!! ne prend pas en compte si le body est tellement rapide qu'il le traverse entre 2 frames sans jamais lui �tre superpos� -> raycast preview pour g�rer �a  
    for (int i = 0; i <= bodies.size() - 1; i++) {
        for (int j = i + 1; j < bodies.size(); j++) { // i + 1 evite de tester 2 fois les collision de 2 points de vie diff�rent A sur B V, B sur A X
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

            }
        }
    }
}
