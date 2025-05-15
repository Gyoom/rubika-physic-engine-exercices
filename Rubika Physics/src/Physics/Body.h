#ifndef BODY_H
#define BODY_H

#include <SDL.h>
#include <SDL_image.h>
#include "Vec2.h"
#include "Shape.h"

enum BodyType {
    BODY,
	VERLET_BODY,
};

struct Point {
    Vec2 pos;
    Vec2 oldPos;
    float mass;
    float pinned;
    Point(Vec2& pos, float mass, bool pinned);
};

struct Constraint {
    Point* p0;
    Point* p1;
    float length;
    float stiffness;

    Constraint(Point* p0, Point* p1, float length, float stiffness);
    //void Update();
};

struct Body {
    bool isColliding = false;
	bool canCollide = true;

    // Linear motion
    Vec2 position;
    Vec2 velocity;
    Vec2 acceleration;

    // Angular motion
    float rotation;
    float angularVelocity;
    float angularAcceleration;
    
    // Forces and torque
    Vec2 sumForces;
    float sumTorque;

    // Mass and Moment of Inertia
    float mass;
    float invMass; // 1 * mass
    float I;
    float invI;

    // Coefficient of restitution (elasticity)
    float restitution;

    // Coefficient of friction
    float friction;

    // Pointer to the shape/geometry of this rigid body
    Shape* shape = nullptr;

    // Pointer to an SDL texture
    SDL_Texture* texture = nullptr;

    BodyType type;

    Body() = default;
    Body(const Shape& shape, float x, float y, float mass, bool canCollide = true);
    ~Body();

    bool IsStatic() const;

    void AddForce(const Vec2& force);
    void AddTorque(float torque);
    void ClearForces();
    void ClearTorque();

    void SetTexture(const char* textureFileName);

    void ApplyImpulse(const Vec2& j);
    void ApplyImpulse(const Vec2& j, const Vec2& r);

    void IntegrateLinear(float dt);
    void IntegrateAngular(float dt);

    virtual void Update(float dt);
};


struct VerletBody : public Body {
    std::vector<Point> points;
    std::vector<Constraint> constraints;
    float initialRotation;


    VerletBody(const Shape& shape, float x, float y, float mass, float stiffness, bool pinned);
    ~VerletBody();
	void Update(float dt) override;
    void applyContraints();
    void handleCollision();
    bool Contains(const Constraint& c);
    void recalculateCenter();
	void ApplyImpulse(const Vec2& j, const Vec2& r, const Vec2& pos);
    void recalculateVertices();
};

#endif
