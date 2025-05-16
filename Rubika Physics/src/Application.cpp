#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"
#include <iostream>
#include <SDL_ttf.h>




bool Application::IsRunning() {
    return running;
}

///////////////////////////////////////////////////////////////////////////////
// Setup function (executed once in the beginning of the simulation)
///////////////////////////////////////////////////////////////////////////////
void Application::Setup() {
    running = Graphics::OpenWindow();

    // Create a physics world with gravity of -9.8 m/s2
    world = new World(-9.8);

	// create background texture
    SDL_Surface* surface = IMG_Load("assets/angrybirds/background.png");
    if (surface) {
        world->bgTexture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }

    // bird
    world->catapultPos = Vec2(Graphics::Width() * 0.175f, Graphics::Height() * 0.6f);
    world->bird = new Bird(Graphics::Width() * 0.015f, world->catapultPos, "assets/angrybirds/bird-red.png");
	world->birds.push_back(world->bird->body);
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
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
					Vec2 mousePos = Vec2(x, y);
					if (world->bird) {
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
                    if (!world->bird)
                        return;

                    Vec2 stretching = world->catapultPos - world->bird->body->position;
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
                    world->bird->body->MoveTo(Vec2(x, y));
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
	SDL_Color black = { 0, 0, 0, 255 };
	TTF_Font* font = TTF_OpenFont("assets/angrybirds/Fonts/angrybirds-regular.ttf", 24);
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Hello World", black);

    if (surface) {
        Graphics::DrawTexture(
            Graphics::Width() / 2, 
            Graphics::Height() / 2, 
            Graphics::Width() / 4, 
            Graphics::Height()/ 4, 
            0, 
            SDL_CreateTextureFromSurface(Graphics::renderer, surface)
        );
        SDL_FreeSurface(surface);
    }
}
void Application::Render() {
    Graphics::DrawTexture(Graphics::Width() / 2, Graphics::Height() / 2, Graphics::Width(), Graphics::Height(), 0, world->bgTexture);
    
    // Draw all bodies
    RenderBodies();
    // Render bird trail
	world->bird->RenderTrail();
	// Render bird preview
    world->bird->RenderPreview();
	// Draw the info
	RenderText();

    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}
