#include "Contact.h"

// !!!
void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }

    // e fonction de la masse, les 2 objets sont +/- déplacés de manière à ce que le deph soit réduit à 0
    // plus la masse est importante vis à vis de l'autre moins on bouge.
    const float da = depth / (a->invMass + b->invMass) * a->invMass;
    const float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;

    a->shape->UpdateVertices(a->rotation, a->position);
    b->shape->UpdateVertices(b->rotation, b->position);


}

// !!!
///////////////////////////////////////////////////////////////////////////////
// Resolves the collision using the impulse method
///////////////////////////////////////////////////////////////////////////////
void Contact::ResolveCollision() {
    // //////////////////////////////////////////////////////////////////////////////////
    // Apply positional correction using the projection method
    // //////////////////////////////////////////////////////////////////////////////////

    ResolvePenetration();

    // application des forces linéaire
    /*const float e = std::min(a->restitution, b->restitution);
    const Vec2 vrel = a->velocity - b->velocity;
    Vec2 J = normal * (-(1 + e) * vrel.Dot(normal) / (a->invMass + b->invMass));
    
    a->ApplyImpulse(J);
    b->ApplyImpulse(-J);*/

    // //////////////////////////////////////////////////////////////////////////////////
    // application des forces avec rotation
    // //////////////////////////////////////////////////////////////////////////////////

    const float e = std::min(a->restitution, b->restitution);
    
    Vec2 ra = end - a->position;
    Vec2 rb = start - b->position;
    
    // comment le point de contact entre a et b bouge vis à vis de la rotation de l'objet et de sa velocity
    Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
    Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

    const Vec2 vRel = va - vb;

    const float vRelDotNormal = vRel.Dot(normal);
    const float impulseMagitudeN = -(1 + e) * vRelDotNormal / ((a->invMass + b->invMass) + 
    ra.Cross(normal) * ra.Cross(normal) * a->invI + 
    rb.Cross(normal) * rb.Cross(normal) * b->invI);

    const Vec2 impulseDirectionN = normal;

    Vec2 jN = impulseDirectionN * impulseMagitudeN;

    /*a->ApplyImpulse(jN, ra);
    b->ApplyImpulse(-jN, rb)*/;

    // ///////////////////////////////////////////////////////////////////////////////
    // appliction des forces avec rotations et frictions
    // ///////////////////////////////////////////////////////////////////////////////

    const float f = std::min(a->friction, b->friction);
    Vec2 t = normal.Normal();

    const float vRelDotT = vRel.Dot(t);
    const float impulseMagitudeT = -(1 + e) * vRelDotT / ((a->invMass + b->invMass) +
    ra.Cross(t) * ra.Cross(t) * a->invI + 
    rb.Cross(t) * rb.Cross(t) * b->invI);

    const Vec2 impulseDirectionT = t;

    Vec2 jT = impulseDirectionT * impulseMagitudeT * f;

    // jT

    a->ApplyImpulse(jN + jT, ra);
    b->ApplyImpulse(-jN + -jT, rb);

    // J = impulse
    // n = normal
    // t = tangente
}
