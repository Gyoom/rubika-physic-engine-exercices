#ifndef WORLD_H
#define WORLD_H

#include "./Body.h"
#include "./Bird.h"
#include "./Pig.h"
#include "./Obstacle.h"
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
        int birdThrow = 0;
        int pigCount = 5;
        int pigKilled = 0;

        // entities
        Vec2 catapult;
        Bird* bird;
        std::vector<Obstacle*> obstacles;
        std::vector<Pig*> pigs;

        World(float gravity);
        ~World();

        void AddBody(Body* body);
        std::vector<Body*>& GetBodies();

        void AddForce(const Vec2& force);
        void AddTorque(float torque);

		void bodiesUpdate(float dt);
        void Update(float dt);

		void VerletCollisionsEffects(VerletBody* a, VerletBody* b);
        void CheckCollisions();
};

#endif
