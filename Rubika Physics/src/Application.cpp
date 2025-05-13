#include "Application.h"
#include "./Physics/Constants.h"
#include "./Physics/Force.h"
#include "./Physics/CollisionDetection.h"
#include "./Physics/Contact.h"

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

    // Add a floor and walls to contain objects
    // Body === rigidbody
    // mass à 0 === no gravity
    Body* floor = new Body(BoxShape(Graphics::Width() - 50, 50), Graphics::Width() / 2.0, Graphics::Height() - 50, 0.0);
    Body* leftWall = new Body(BoxShape(50, Graphics::Height() - 100), 50, Graphics::Height() / 2.0 - 25, 0.0);
    Body* rightWall = new Body(BoxShape(50, Graphics::Height() - 100), Graphics::Width() - 50, Graphics::Height() / 2.0 - 25, 0.0);
    floor->restitution = 1.0; // absporption d'impact
    leftWall->restitution = 0.2;
    rightWall->restitution = 0.2;
    //world->AddBody(floor);
    //world->AddBody(leftWall);
    //world->AddBody(rightWall);

	Body* pig1 = new Body(BoxShape(60, 60), 30, 30, 0, false);
    pig1->SetTexture("assets/angrybirds/pig-2.png");
    pig1->restitution = 0.7;
    world->AddBody(pig1);
     
    Body* pig2 = new Body(CircleShape(30), 500, 200, 0, false);
    pig2->SetTexture("assets/angrybirds/pig-2.png");
    pig2->restitution = 0.7;
    world->AddBody(pig2);
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
                    debug = !debug; // affiche les formes des objets en jeu
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);
                    Body* ball = new VerletBody(CircleShape(30), x, y, 1, 0.05, false);
                    ball->SetTexture("assets/angrybirds/pig-2.png");
                    ball->restitution = 0.7;
                    world->AddBody(ball);
                }
                if (event.button.button == SDL_BUTTON_RIGHT) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    Body* box = new VerletBody(BoxShape(60, 60), x, y, 1.0, 1, false);
                    box->SetTexture("assets/angrybirds/wood-box.png");
                    box->restitution = 0.05;
                    world->AddBody(box);
                }
                if (event.button.button == SDL_BUTTON_MIDDLE) {
                    int x, y;
                    SDL_GetMouseState(&x, &y);

					std::vector<Vec2> localVertices = {
						Vec2(-50, -50),
						Vec2(0, -75),
						Vec2(50, -50),
						Vec2(50, 50),
						Vec2(-50, 50)
					};
                    Body* box = new VerletBody(PolygonShape(localVertices), x, y, 1.0, 0.05, false);
                    //box->SetTexture("assets/angrybirds/wood-box.png");
                    box->restitution = 0.05;
                    world->AddBody(box);
                }
                break;
	                case SDL_MOUSEMOTION:
                        int x, y;
                        SDL_GetMouseState(&x, &y);
                        world->GetBodies()[1]->position.x = x;
                        world->GetBodies()[1]->position.y = y;
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
void Application::Render() {
    debug = false;
    // Draw all bodies
    for (auto body: world->GetBodies()) {
        if (body->shape->GetType() == CIRCLE) {
            CircleShape* circleShape = (CircleShape*) body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, circleShape->radius * 2, circleShape->radius * 2, body->rotation, body->texture);
            } else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*)body;

                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for (Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 10, body->rotation, 0xFF00FF00);
                    }
                }
                else
                    Graphics::DrawCircle(body->position.x, body->position.y, circleShape->radius, body->rotation, body->isColliding ?  0xF90B00FF : 0xFF00FF00);
            }
        }
        if (body->shape->GetType() == BOX) {
            BoxShape* boxShape = (BoxShape*) body->shape;
            if (!debug && body->texture) {
                Graphics::DrawTexture(body->position.x, body->position.y, boxShape->width, boxShape->height, body->rotation, body->texture);
            } else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*) body;
                   
                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for(Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 10, body->rotation, 0xFF00FF00);
                    }
                }
                else 
                {
                    Graphics::DrawPolygon(body->position.x, body->position.y, boxShape->worldVertices, body->isColliding ? 0xF90B00FF : 0xFF00FF00);
                }
            }
        }
        if (body->shape->GetType() == POLYGON) {
            PolygonShape* polygonShape = (PolygonShape*) body->shape;
            if (!debug) {
                Graphics::DrawFillPolygon(body->position.x, body->position.y, polygonShape->worldVertices, 0xFF444444);
            } else {
                if (body->type == VERLET_BODY) {
                    VerletBody* vBody = (VerletBody*) body;
                   
                    for (Constraint c : vBody->constraints)
                    {
                        Graphics::DrawLine(c.p0->pos.x, c.p0->pos.y, c.p1->pos.x, c.p1->pos.y, 0xFF00FF00);
                    }
                    for(Point p : vBody->points)
                    {
                        Graphics::DrawCircle(p.pos.x, p.pos.y, 10, body->rotation, 0xFF00FF00);
                    }
                }
                else 
                {
                    Graphics::DrawPolygon(body->position.x, body->position.y, polygonShape->worldVertices, body->isColliding ? 0xF90B00FF : 0xFF00FF00);
                }
            }
        }
    }
    Graphics::RenderFrame();
}

///////////////////////////////////////////////////////////////////////////////
// Destroy function to delete objects and close the window
///////////////////////////////////////////////////////////////////////////////
void Application::Destroy() {
    delete world;
    Graphics::CloseWindow();
}
