#include "World.h"
#include "Constants.h"
#include "CollisionDetection.h"
#include "../Graphics.h"
#include <iostream>

World::World(float gravity) {
    G = -gravity;
    std::cout << "World constructor called!" << std::endl;
}

World::~World() {
    for (auto body: bodies) {
        delete body;
    }
    std::cout << "World destructor called!" << std::endl;
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

void World::Update(float dt) {

    //this->AddTorque(20.f);

	// Loop all bodies of the world applying forces
    for (auto body: bodies) {
        // Apply the weight force to all bodies
        Vec2 weight = Vec2(0.0, body->mass * G * PIXELS_PER_METER);
        body->AddForce(weight);

        // Apply global forces (wind etc.) to all bodies
        for (auto force: forces) {
            body->AddForce(force);
        }

        // Apply global torque (wind etc.) to all bodies
        for (auto torque: torques) {
            body->AddTorque(torque);
        }
    }

    // Update all the bodies in the world (integrating and transforming vertices)
    for (auto body: bodies) {
        body->Update(dt);
    }

    CheckCollisions();
}

// !!!
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

                Graphics::DrawFillCircle(contact.start.x, contact.start.y, 3, 0xFFFF00FF);
                Graphics::DrawFillCircle(contact.end.x, contact.end.y, 3, 0xFFFFFFFF);
                Graphics::DrawLine(contact.start.x, contact.start.y, contact.start.x + contact.normal.x * 15, contact.start.y + contact.normal.y * 15, 0xFFFFFFFF);

            }
        }
    }
}
