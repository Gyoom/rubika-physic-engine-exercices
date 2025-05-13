#include "../Graphics.h"
#include "Body.h"
#include <math.h>
#include <iostream>


Body::Body(const Shape& shape, float x, float y, float mass, bool canCollide) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->velocity = Vec2(0, 0);
    this->acceleration = Vec2(0, 0);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;
    this->friction = 0.7;
    this->mass = mass;
	this->canCollide = canCollide;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    } else {
        this->invMass = 0.0;
    }
    I = shape.GetMomentOfInertia() * mass;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    } else {
        this->invI = 0.0;
    }

	type = BODY;
    std::cout << "Body constructor called!" << std::endl;
}

Body::~Body() {
    delete shape;
    SDL_DestroyTexture(texture);
    std::cout << "Body destructor called!" << std::endl;
}

void Body::SetTexture(const char* textureFileName) {
    SDL_Surface* surface = IMG_Load(textureFileName);
    if (surface) {
        texture = SDL_CreateTextureFromSurface(Graphics::renderer, surface);
        SDL_FreeSurface(surface);
    }
}

bool Body::IsStatic() const {
    const float epsilon = 0.005f;
    return fabs(invMass - 0.0) < epsilon;
}

void Body::AddForce(const Vec2& force) {
    sumForces += force;
}

void Body::AddTorque(float torque) {
    sumTorque += torque;
}

void Body::ClearForces() {
    sumForces = Vec2(0.0, 0.0);
}

void Body::ClearTorque() {
    sumTorque = 0.0;
}

void Body::ApplyImpulse(const Vec2& j) {
    if (IsStatic()) {
        return;
    }
    velocity += j * invMass;
}

void Body::ApplyImpulse(const Vec2& j, const Vec2& r) {
    if (IsStatic()) {
        return; 
    }
    velocity += j * invMass;
    angularVelocity += r.Cross(j) * invI;
}

void Body::IntegrateLinear(float dt) {
    if (IsStatic()) {
        return;
    }

    // Find the acceleration based on the forces that are being applied and the mass
    acceleration = sumForces * invMass;

    // Integrate the acceleration to find the new velocity
    velocity += acceleration * dt;

    // Integrate the velocity to find the new position
    position += velocity * dt;

    // Clear all the forces acting on the object before the next physics step
    ClearForces();
}

void Body::IntegrateAngular(float dt) {
    if (IsStatic()) {
        return;
    }

    // Find the angular acceleration based on the torque that is being applied and the moment of inertia
    angularAcceleration = sumTorque * invI;

    // Integrate the angular acceleration to find the new angular velocity
    angularVelocity += angularAcceleration * dt;

    // Integrate the angular velocity to find the new rotation angle
    rotation += angularVelocity * dt;

    // Clear all the torque acting on the object before the next physics step
    ClearTorque();
}

void Body::Update(float dt) {
    IntegrateLinear(dt);
    IntegrateAngular(dt);
    shape->UpdateVertices(rotation, position);
}


////////////////////////////////////////////////////////////////////////////////////////
// Verlets
////////////////////////////////////////////////////////////////////////////////////////

Point::Point(Vec2& pos, float mass, bool pinned) {
    this->pos = pos;
    this->oldPos = Vec2(this->pos.x, this->pos.y);
    this->mass = mass;
    this->pinned = pinned;
}

Constraint::Constraint(Point* a, Point* b, float length, float stiffness) {
    this->p0 = a;
    this->p1 = b;
    this->length = length;
    this->stiffness = stiffness;
}

////////////////////////////////////////////////////////////////////////////////////////
// Verlet body
////////////////////////////////////////////////////////////////////////////////////////

VerletBody::VerletBody(const Shape& shape, float x, float y, float mass, float stiffness, bool pinned) {
    this->shape = shape.Clone();
    this->position = Vec2(x, y);
    this->rotation = 0.0;
    this->angularVelocity = 0.0;
    this->angularAcceleration = 0.0;
    this->sumForces = Vec2(0, 0);
    this->sumTorque = 0.0;
    this->restitution = 1.0;
    this->friction = 0.7;
    this->mass = mass;
    if (mass != 0.0) {
        this->invMass = 1.0 / mass;
    }
    else {
        this->invMass = 0.0;
    }
    I = shape.GetMomentOfInertia() * mass;
    if (I != 0.0) {
        this->invI = 1.0 / I;
    }
    else {
        this->invI = 0.0;
    }

    switch (this->shape->GetType()) {
	    case CIRCLE: {
            CircleShape* box = (CircleShape*)shape.Clone();
            float radius = box->radius;
            int sideCount = 8;
            int angle = 360 / sideCount;

            for (size_t i = 0; i < sideCount; i++)
            {
                float x = radius * cos(angle * i * M_PI / 180);
                float y = radius * sin(angle * i * M_PI / 180);
                Vec2 pos = Vec2(position.x + x, position.y + y);
                points.push_back(Point(pos, 1.0, pinned));
            }
            for (Point& p0 : points) {
                for (Point& p1 : points) {
                    Constraint c = Constraint(&p0, &p1, p0.pos.Dist(p1.pos), stiffness);
                    
                    if (&p0 != &p1 && !Contains(c)) {
                        constraints.push_back(c);
                    }
                }
            }
		    break;
	    }
	    case POLYGON: {
			PolygonShape* polygon = (PolygonShape*)shape.Clone();
			for (size_t i = 0; i < polygon->localVertices.size(); i++)
			{
				Vec2 pos = Vec2(position.x + polygon->localVertices[i].x, position.y + polygon->localVertices[i].y);
				points.push_back(Point(pos, 1.0, pinned));
			}
			for (Point& p0 : points) {
				for (Point& p1 : points) {
					Constraint c = Constraint(&p0, &p1, p0.pos.Dist(p1.pos), stiffness);

					if (&p0 != &p1 && !Contains(c)) {
						constraints.push_back(c);
					}
				}
			}
		    break;
	    }
	    case BOX: {
		    BoxShape* box = (BoxShape*) shape.Clone();
		    float width = box->width;
		    float height = box->height;
            float halfWidth = width / 2.0f;
            float halfHeight = height / 2.0f;

            Vec2 pos = Vec2(position.x - halfWidth, position.y - halfHeight);
            points.push_back(Point(pos, 1.0, pinned));
            pos = Vec2(position.x + halfWidth, position.y - halfHeight);
            points.push_back(Point(pos, 1.0, pinned));
            pos = Vec2(position.x + halfWidth, position.y + halfHeight);
            points.push_back(Point(pos, 1.0, pinned));
            pos = Vec2(position.x - halfWidth, position.y + halfHeight);
            points.push_back(Point(pos, 1.0, pinned));

            constraints.push_back(Constraint(&points[0], &points[1], width, stiffness));
            constraints.push_back(Constraint(&points[1], &points[2], height, stiffness));
            constraints.push_back(Constraint(&points[2], &points[3], width, stiffness));
            constraints.push_back(Constraint(&points[3], &points[0], height, stiffness));

            constraints.push_back(Constraint(&points[0], &points[2], sqrtf(width * width + height * height), stiffness));
            constraints.push_back(Constraint(&points[1], &points[3], sqrtf(width * width + height * height), stiffness));
		    break;
	    }
    }
	type = VERLET_BODY;
}

VerletBody::~VerletBody()
{
	delete shape;
	SDL_DestroyTexture(texture);
	std::cout << "VerletBody destructor called!" << std::endl;
}

void VerletBody::Update(float dt)
{
	//std::cout << "VerletBody Update called! " << sumForces.x << " : "  << sumForces.y << std::endl;
    for (Point& p :  points) {
        if (p.pinned)
            continue;

        // 1. compute implicite velocity (we dont store it)
        Vec2 vel = p.pos - p.oldPos;
        vel = vel * 0.993;
       
        // 2. compute acceleration
        Vec2 acc = sumForces / p.mass;
        acc = acc * dt * dt;

        // 3. integrate velocity on accelleration
        vel += acc;

        // 4. Integrate pos
        p.oldPos.x = p.pos.x;
        p.oldPos.y = p.pos.y;
        p.pos = p.pos + vel;
    }
}

void VerletBody::applyContraints()
{
   for (Constraint& c : constraints) {
        Vec2 v = c.p1->pos - c.p0->pos;
        float dist = v.Magnitude();
        float diff = c.length - dist;
        float ratio = diff / dist * c.stiffness;
        Vec2 offset = v * ratio * 0.5;

        if (!c.p0->pinned)
        {
            c.p0->pos.Sub(offset);
        }
        if (!c.p1->pinned)
        {
            c.p1->pos.Add(offset);
        }

    }
}

void VerletBody::handleCollision()
{
    float offset = 100;
    for (Point& p : points) {
        if (p.pos.y > Graphics::Height() - offset)
        {
            p.pos.y = Graphics::Height() - offset;
        }
        else if (p.pos.y < offset)
        {
            p.pos.y = offset;
        }

        if (p.pos.x > Graphics::Width() - offset)
        {
            p.pos.x = Graphics::Width() - offset;
        }
        else if (p.pos.x < offset)
        {
            p.pos.x = offset;
        }
    
    }
}

bool VerletBody::Contains(const Constraint& newConstr)
{
    for (Constraint& c : constraints) {
        bool a = newConstr.p0 == c.p0 && newConstr.p1 == c.p1;
        bool b = newConstr.p0 == c.p1 && newConstr.p1 == c.p0;
        if (a || b)
            return true;

    }
    return false;
}

// shape collisions functions

void VerletBody::recalculateCenter()
{
    Vec2 center = Vec2(0, 0);
    for (Point& p : points) {
        center += p.pos;
    }

    center.x /= points.size();
    center.y /= points.size();

    position = center;
}



void VerletBody::ApplyImpulse(const Vec2& j, const Vec2& r, const Vec2& pos)
{
    if (IsStatic()) {
        return;
    }
	Point* p = nullptr;
    float distance = FLT_MAX;
	
    for (Point& point : points) {
		Vec2 d = point.pos - pos;
		float dist = d.Magnitude();
		if (dist < distance) {
			distance = dist;
			p = &point;
		}
	}
    if(p == nullptr)
		return;

    p->oldPos.x = p->pos.x;
    p->oldPos.y = p->pos.y;
    p->pos += j * invMass;
}

void VerletBody::recalculateVertices()
{
	if (shape->GetType() == BOX || shape->GetType() == POLYGON) {
		BoxShape* boxShape = (BoxShape*)shape;

        for (size_t i = 0; i < points.size(); i++)
        {
            boxShape->worldVertices[i].x = points[i].pos.x;
			boxShape->worldVertices[i].y = points[i].pos.y;
        }   
	}
	
}




