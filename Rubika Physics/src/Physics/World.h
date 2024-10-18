#ifndef WORLD_H
#define WORLD_H

#include "./Body.h"
#include <vector>

class World {
    private:
        float G = 9.8;
        std::vector<Body*> bodies;
        std::vector<Vec2> forces;
        std::vector<float> torques; // aVelocity


    public:
        World(float gravity);
        ~World();

        void AddBody(Body* body);
        std::vector<Body*>& GetBodies();

        void AddForce(const Vec2& force);
        void AddTorque(float torque);

        void Update(float dt);

        void CheckCollisions();
};

#endif
