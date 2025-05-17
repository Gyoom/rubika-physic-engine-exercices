#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include "./Physics/Pig.h"
#include <iostream>
#include <SDL_ttf.h>
#include <string>

bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();
	debug = false;
	
    // Init texting lib 
    TTF_Init();
	mainFont = TTF_OpenFont("assets/angrybirds/Fonts/angrybirds-regular.ttf", 300);
  
    // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);

	// create background texture
    SDL_Surface* surface = IMG_Load("assets/angrybirds/background.png");
    if (surface) {
        world->bgTexture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }

    // bird
    world->catapult = Vec2(Graphics::Width() * 0.175f, Graphics::Height() * 0.6f);
    world->bird = new Bird(Graphics::Width() * 0.015f, world->catapult, "assets/angrybirds/bird-red.png", world->catapult);
	world->bird->body->entity = world->bird;
    world->AddBody(world->bird->body);

	float woodMass = 1.0f;
	float stoneMass = 1.0f;
	float glassMass = 0.5f;

    // obstacles row 1
	Obstacle* obstacle = new Obstacle(new VerletBody(BoxShape(600, 50), Graphics::Width() * 0.8f, Graphics::Height() * 0.779f, 1000, 1, true), "assets/angrybirds/rock-bridge-anchor.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
	world->AddBody(obstacle->body);

    // obstacles row 2

    obstacle = new Obstacle(new VerletBody(BoxShape(50, 50), Graphics::Width() * 0.68f, Graphics::Height() * 0.737f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(100, 50), Graphics::Width() * 0.72f, Graphics::Height() * 0.737f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(50, 50), Graphics::Width() * 0.92f, Graphics::Height() * 0.737f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(100, 50), Graphics::Width() * 0.88f, Graphics::Height() * 0.737f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    // obstacles row 3

    obstacle = new Obstacle(new VerletBody(BoxShape(100, 50), Graphics::Width() * 0.693f, Graphics::Height() * 0.695f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
	obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(50, 50), Graphics::Width() * 0.733f, Graphics::Height() * 0.695f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(100, 50), Graphics::Width() * 0.907f, Graphics::Height() * 0.695f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(50, 50), Graphics::Width() * 0.867f, Graphics::Height() * 0.695f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    // obstacles row 4

    obstacle = new Obstacle(new VerletBody(BoxShape(25, 100), Graphics::Width() * 0.68f, Graphics::Height() * 0.634f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(40, 100), Graphics::Width() * 0.705f, Graphics::Height() * 0.634f, stoneMass, 1, false), "assets/angrybirds/rock-box.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(25, 100), Graphics::Width() * 0.733f, Graphics::Height() * 0.634f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);


    obstacle = new Obstacle(new VerletBody(BoxShape(25, 100), Graphics::Width() * 0.865f, Graphics::Height() * 0.634f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(40, 100), Graphics::Width() * 0.890f, Graphics::Height() * 0.634f, stoneMass, 1, false), "assets/angrybirds/rock-box.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(25, 100), Graphics::Width() * 0.915f, Graphics::Height() * 0.634f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 5

    obstacle = new Obstacle(new VerletBody(BoxShape(140, 20), Graphics::Width() * 0.705f, Graphics::Height() * 0.582f, stoneMass, 1, false), "assets/angrybirds/rock-bridge-anchor.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(140, 20), Graphics::Width() * 0.890f, Graphics::Height() * 0.582f, stoneMass, 1, false), "assets/angrybirds/rock-bridge-anchor.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 6

    obstacle = new Obstacle(new VerletBody(BoxShape(40, 40), Graphics::Width() * 0.677f, Graphics::Height() * 0.557f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(CircleShape(Graphics::Width() * 0.01f), Graphics::Width() * 0.73f, Graphics::Height() * 0.557f, woodMass, 1, false), "assets/angrybirds/wood-bridge-step.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(300, 20), Graphics::Width() * 0.795f, Graphics::Height() * 0.5326f, woodMass, 1, false), "assets/angrybirds/wood-plank-cracked.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(CircleShape(Graphics::Width() * 0.01f), Graphics::Width() * 0.865f, Graphics::Height() * 0.557f, woodMass, 1, false), "assets/angrybirds/wood-bridge-step.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(40, 40), Graphics::Width() * 0.915f, Graphics::Height() * 0.557f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 7

    obstacle = new Obstacle(new VerletBody(BoxShape(20, 80), Graphics::Width() * 0.677f, Graphics::Height() * 0.505f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(20, 80), Graphics::Width() * 0.915f, Graphics::Height() * 0.505f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 8

    obstacle = new Obstacle(new VerletBody(CircleShape(Graphics::Width() * 0.01f), Graphics::Width() * 0.77f, Graphics::Height() * 0.508f, woodMass, 1, false), "assets/angrybirds/wood-bridge-step.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(CircleShape(Graphics::Width() * 0.01f), Graphics::Width() * 0.82f, Graphics::Height() * 0.508f, woodMass, 1, false), "assets/angrybirds/wood-bridge-step.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(140, 20), Graphics::Width() * 0.795f, Graphics::Height() * 0.4835f, stoneMass, 1, false), "assets/angrybirds/rock-bridge-anchor.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 9

    obstacle = new Obstacle(new VerletBody(BoxShape(20, 100), Graphics::Width() * 0.765f, Graphics::Height() * 0.433f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(20, 100), Graphics::Width() * 0.825f, Graphics::Height() * 0.433f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

    obstacle = new Obstacle(new VerletBody(BoxShape(140, 20), Graphics::Width() * 0.795f, Graphics::Height() * 0.383f, stoneMass, 1, false), "assets/angrybirds/rock-bridge-anchor.png", STONE);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);

	// obstacles row 10

    obstacle = new Obstacle(new VerletBody(BoxShape(50, 120), Graphics::Width() * 0.796f, Graphics::Height() * 0.323f, woodMass, 1, false), "assets/angrybirds/wood-box.png", WOOD);
    obstacle->body->entity = obstacle;
    world->obstacles.push_back(obstacle);
    world->AddBody(obstacle->body);


    // pigs
    Pig* pig = new Pig(Graphics::Width() * 0.02f, Vec2(Graphics::Width() * 0.8f, Graphics::Height() * 0.725f), "assets/angrybirds/pig-1.png");
	pig->body->entity = pig;
    world->pigs.push_back(pig);
    world->AddBody(pig->body);

    pig = new Pig(Graphics::Width() * 0.01f, Vec2(Graphics::Width() * 0.77f, Graphics::Height() * 0.744f), "assets/angrybirds/pig-1.png");
    pig->body->entity = pig;
    world->pigs.push_back(pig);
    world->AddBody(pig->body);

    pig = new Pig(Graphics::Width() * 0.01f, Vec2(Graphics::Width() * 0.7f, Graphics::Height() * 0.557f), "assets/angrybirds/pig-1.png");
    pig->body->entity = pig;
    world->pigs.push_back(pig);
    world->AddBody(pig->body);

    pig = new Pig(Graphics::Width() * 0.01f, Vec2(Graphics::Width() * 0.89f, Graphics::Height() * 0.557f), "assets/angrybirds/pig-1.png");
    pig->body->entity = pig;
    world->pigs.push_back(pig);
    world->AddBody(pig->body);

    pig = new Pig(Graphics::Width() * 0.015f, Vec2(Graphics::Width() * 0.795f, Graphics::Height() * 0.45f), "assets/angrybirds/pig-1.png");
    pig->body->entity = pig;
    world->pigs.push_back(pig);
    world->AddBody(pig->body);
	

}

///////////////////////////////////////////////////////////////////////////////
// Input processing
///////////////////////////////////////////////////////////////////////////////
void Application::Input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running = false;
                break;
			case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    running = false;
                if (event.key.keysym.sym == SDLK_d)
                    debug = !debug;
                if (event.key.keysym.sym == SDLK_r) {
					reload = true;
                    running = false;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
					Vec2 mousePos = Vec2(x, y);
					if (world->bird && !world->bird->isFlying) {
						CircleShape* circleShape = (CircleShape*)world->bird->body->shape;
                        if (circleShape && world->bird->body->position.Dist(mousePos) < circleShape->radius) {
                            world->bird->isDragging = true;
                        }
					}
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    
                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    
                }
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    if (!world->bird || world->bird->isFlying)
                        return;

                    Vec2 stretching = world->catapult - world->bird->body->position;
                    if (stretching.Magnitude() > 1) {
                        stretching *= world->bird->stretchingMultiplier;
                        world->bird->body->SetPinned(false);
                        world->bird->body->AddForce(stretching);
                        world->bird->isFlying = true;
                    }
                    world->bird->isDragging = false;
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {

                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* bird1 = new VerletBody(CircleShape(world->bird->birdSize), x, y, 1, 1, false);
                    bird1->SetTexture("assets/angrybirds/bird-red.png");
                    world->AddBody(bird1);
                }
                break;
	        case SDL_MOUSEMOTION:
				if (world->bird && world->bird->isDragging) {
					int x, y;
					SDL_GetMouseState(&x, &y);
					Vec2 mousePos = Vec2(x, y);
					Vec2 currentStretching = mousePos - world->catapult;
                    if (currentStretching.Magnitude() > world->bird->maxStretching) {
                        currentStretching = currentStretching.Normalize();
						currentStretching *= world->bird->maxStretching;
						mousePos = world->catapult + currentStretching;
                    }
                    world->bird->body->MoveTo(mousePos);
				}
	            break;
			default:
				break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
// Update function (called several times per second to update objects)
///////////////////////////////////////////////////////////////////////////////
void Application::Update() {
    Graphics::ClearScreen(0xFF222222); // background function (reset les pixels pas les objets)
    
    // Wait some time until the reach the target frame time in milliseconds
    // calcul du Delta Time, bloquant jusqu'à ce que la durée du DT soit passé.
    static int timePreviousFrame;
    int timeToWait = MILLISECS_PER_FRAME - (SDL_GetTicks() - timePreviousFrame);
    if (timeToWait > 0)
        SDL_Delay(timeToWait);

    // Calculate the deltatime in seconds
    float deltaTime = (SDL_GetTicks() - timePreviousFrame) / 1000.0f;
    if (deltaTime > 0.016)
        deltaTime = 0.016; // si on place un breakpoint, l'execution est suspendue mais le delta time continue d'augmenter

    // Set the time of the current frame to be used in the next one
    timePreviousFrame = SDL_GetTicks();

    // Update world bodies (integration, collision detection, etc.)
    world->Update(deltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// Render function (called several times per second to draw objects)
///////////////////////////////////////////////////////////////////////////////

void Application::RenderBodies() {
    for (auto body : world->GetBodies()) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*)body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
            }
            else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*)body;

                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for (Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 5, body->rotation, 0xFF00FF00);
                    }
                }
                else
                    Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, body->isColliding ? 0xF90B00FF : 0xFF00FF00);
            }
        }
        if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*)body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            }
            else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*)body;

                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for (Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 5, body->rotation, 0xFF00FF00);
                    }
                }
                else
                {
                    Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, body->isColliding ? 0xF90B00FF : 0xFF00FF00);
                }
            }
        }
        if (body->shape->GetType() == POLYGON) {
            PolygonShape* polygonShape = (PolygonShape*)body->shape;
            if (!debug) {
                Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF444444);
            }
            else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*)body;

                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for (Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 5, body->rotation, 0xFF00FF00);
                    }
                }
                else
                {
                    Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, body->isColliding ? 0xF90B00FF : 0xFF00FF00);
                }
            }
        }
    }
}

void Application::RenderText() {
    std::string text = "Verlet Angry Birds";
    SDL_Surface* surface = TTF_RenderText_Solid(mainFont, text.c_str(), black);
    SDL_Texture* texture = nullptr;
    // Title
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
        Graphics::DrawTexture(
            Graphics::Width() * 0.2,
            Graphics::Height() * 0.1,
            Graphics::Width() * 0.365,
            Graphics::Height() * 0.16666,
            0,
            texture
        );
        SDL_DestroyTexture(texture);
	}
	// Birds count
	text = std::to_string(world->birdMax - world->birdThrow) + "/" + std::to_string(world->birdMax);
    surface = TTF_RenderText_Solid(mainFont, text.c_str(), Red);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
        Graphics::DrawTexture(
            Graphics::Width() * 0.05,
            Graphics::Height() * 0.9,
            Graphics::Width() * 0.052,
            Graphics::Height() * 0.083,
            0,
            texture
        );
        SDL_DestroyTexture(texture);
    }
	// Pigs count
    text = std::to_string(world->pigCount - world->pigKilled) + "/" + std::to_string(world->pigCount);
    surface = TTF_RenderText_Solid(mainFont, text.c_str(), Green);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
        Graphics::DrawTexture(
            Graphics::Width() * 0.2,
            Graphics::Height() * 0.9,
            Graphics::Width() * 0.052,
            Graphics::Height() * 0.083,
            0,
            texture
        );
        SDL_DestroyTexture(texture);
        
    }
	bool isGameOver = false;
    // Win
    if (world->pigCount == world->pigKilled) 
    {
		isGameOver = true;
        text = "You Win!";
        surface = TTF_RenderText_Solid(mainFont, text.c_str(), Green);
        if (surface) {
            texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
            SDL_FreeSurface(surface);
            Graphics::DrawTexture(
                Graphics::Width() * 0.5,
                Graphics::Height() * 0.5,
                Graphics::Width() * 0.2,
                Graphics::Height() * 0.1,
                0,
                texture
            );
            SDL_DestroyTexture(texture);
        }
	}
    // Lose
	else if (world->birdThrow >= world->birdMax) 
    {
		isGameOver = true;
		text = "You Lose!";
		surface = TTF_RenderText_Solid(mainFont, text.c_str(), Red);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
			SDL_FreeSurface(surface);
			Graphics::DrawTexture(
				Graphics::Width() * 0.5,
				Graphics::Height() * 0.5,
				Graphics::Width() * 0.2,
				Graphics::Height() * 0.1,
				0,
				texture
			);
			SDL_DestroyTexture(texture);
		}
	}
    // Reload Game
    if (isGameOver) {
		text = "Press R to reload";
		surface = TTF_RenderText_Solid(mainFont, text.c_str(), black);
		if (surface) {
			texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
			SDL_FreeSurface(surface);
			Graphics::DrawTexture(
				Graphics::Width() * 0.5,
				Graphics::Height() * 0.6,
				Graphics::Width() * 0.1,
				Graphics::Height() * 0.05,
				0,
				texture
			);
			SDL_DestroyTexture(texture);
		}
    }

}

void Application::Render() {
    Graphics::DrawTexture(Graphics::Width() / 2, Graphics::Height() / 2, Graphics::Width(), Graphics::Height(), 0, world->bgTexture);
    
	// Draw the catapult back
	world->bird->RenderStretching(true);
    // Draw all bodies
    RenderBodies();
    // Render bird trail
	world->bird->RenderTrail();
	// Render bird preview
    world->bird->RenderPreview();
	// Draw the catapult front
    world->bird->RenderStretching(false);
	// Draw the info
	RenderText();

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete world;
	delete mainFont;
}