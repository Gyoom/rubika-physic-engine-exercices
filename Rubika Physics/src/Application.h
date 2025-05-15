#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/World.h"
#include <vector>

class Application {
    private:
        bool debug = false;
        bool running = false;
        SDL_Texture* bgTexture = nullptr;
        World* world;

		int birdMax = 3;
		int birdCount = 0;
		int pigCount = 5;


        std::vector<Body*> obstacles;
        std::vector<Body*> pigs;
		std::vector<Vec2*> trail;
		std::vector<Vec2*> preview;


        Body* currentBird;
		bool isDragging = false;
		bool isFlying = false;

    public:
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
		void RenderBodies();
		void RenderTrail();
		void RenderPreview();
		void RenderText();
        void Render();
        void Destroy();
};

#endif
