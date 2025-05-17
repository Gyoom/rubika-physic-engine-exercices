#ifndef APPLICATION_H
#define APPLICATION_H

#include "./Graphics.h"
#include "./Physics/World.h"
#include <vector>
#include <SDL_ttf.h>

class Application {
    private:
        bool debug = false;
        bool running = false;

        World* world;

		// Texting
        TTF_Font* mainFont;
        SDL_Color black = { 0, 0, 0, 255 };
        SDL_Color Green = { 0, 255, 0, 255 };
        SDL_Color Red = { 255, 0, 0, 255 };


    public:
        bool reload = true;
        Application() = default;
        ~Application() = default;
        bool IsRunning();
        void Setup();
        void Input();
        void Update();
		void RenderBodies();;
		void RenderText();
        void Render();
        void Destroy();
};

#endif
