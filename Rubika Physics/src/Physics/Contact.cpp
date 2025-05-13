#include "Contact.h"
#include <iostream>

// !!!
void Contact::ResolvePenetration() {
    if (a->IsStatic() && b->IsStatic()) {
        return;
    }

    // e fonction de la masse, les 2 objets sont +/- d�plac�s de mani�re � ce que le deph soit r�duit � 0
    // plus la masse est importante vis � vis de l'autre moins on bouge.
    const float da = depth / (a->invMass + b->invMass) * a->invMass;
    const float db = depth / (a->invMass + b->invMass) * b->invMass;

    a->position -= normal * da;
    b->position += normal * db;

    a->shape->UpdateVertices(a->rotation, a->position);
    b->shape->UpdateVertices(b->rotation, b->position);


}


void Contact::ResolveCollision() {
    
    ///////////////////////////////////////////////////////////////////////////////
    // Resolves the collision using the impulse method
    ///////////////////////////////////////////////////////////////////////////////
    if (a->type == BODY && b->type == BODY) {
		std::cout << a->shape->GetType() << std::endl;
        
        
        // //////////////////////////////////////////////////////////////////////////////////
        // Apply positional correction using the projection method
        // //////////////////////////////////////////////////////////////////////////////////

        ResolvePenetration();

        // //////////////////////////////////////////////////////////////////////////////////
        // application des forces avec rotation
        // //////////////////////////////////////////////////////////////////////////////////

        const float e = std::min(a->restitution, b->restitution);

        Vec2 ra = end - a->position;
        Vec2 rb = start - b->position;

        // comment le point de contact entre a et b bouge vis � vis de la rotation de l'objet et de sa velocity
        Vec2 va = a->velocity + Vec2(-a->angularVelocity * ra.y, a->angularVelocity * ra.x);
        Vec2 vb = b->velocity + Vec2(-b->angularVelocity * rb.y, b->angularVelocity * rb.x);

        const Vec2 vRel = va - vb;

        const float vRelDotNormal = vRel.Dot(normal);
        const float impulseMagitudeN = -(1 + e) * vRelDotNormal / ((a->invMass + b->invMass) +
            ra.Cross(normal) * ra.Cross(normal) * a->invI +
            rb.Cross(normal) * rb.Cross(normal) * b->invI);

        const Vec2 impulseDirectionN = normal;

        Vec2 jN = impulseDirectionN * impulseMagitudeN;

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
    ///////////////////////////////////////////////////////////////////////////////
	// Resolves the collision using the verlet method
    ///////////////////////////////////////////////////////////////////////////////
    else 
    {
        VerletBody* vA = (VerletBody*) a;
        VerletBody* vB = (VerletBody*) b;
        vA->ApplyImpulse(-normal * depth, Vec2(0, 0), end);
		vB->ApplyImpulse(normal * depth, Vec2(0, 0), start);
    }
}
