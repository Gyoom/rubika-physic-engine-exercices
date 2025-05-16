#ifndef WORLD_H
#define WORLD_H

#include "./Body.h"
#include "./Bird.h"
#include <vector>

class World {
    private:
        float G = 9.8;
        std::vector<Body*> bodies;

		// Forces and torque
        std::vector<Vec2> forces;
        std::vector<float> torques;


    public:
        SDL_Texture* bgTexture = nullptr;
        // stats
        int birdMax = 3;
        int birdCount = 0;
        int pigCount = 5;
        Vec2 catapultPos;

        // entities
        Bird* bird;
		std::vector<Body*> birds;
        std::vector<Body*> obstacles;
        std::vector<Body*> pigs;

        World(float gravity);
        ~World();

        void AddBody(Body* body);
        std::vector<Body*>& GetBodies();

        void AddForce(const Vec2& force);
        void AddTorque(float torque);

        void Update(float dt);
        void ConcreteUpdate(float dt, std::vector<Body*> bodies);

        void CheckCollisions();
};

#endif
