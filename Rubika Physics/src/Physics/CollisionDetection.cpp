#include "CollisionDetection.h"
#include "../Graphics.h"
#include <limits>
#include <iostream>

bool CollisionDetection::IsColliding(Body* a, Body* b, Contact& contact) {

	if (a->canCollide == false || b->canCollide == false) {
		return false;
	}

    bool aIsCircle = a->shape->GetType() == CIRCLE;
    bool bIsCircle = b->shape->GetType() == CIRCLE;
    bool aIsPolygon = a->shape->GetType() == POLYGON || a->shape->GetType() == BOX;
    bool bIsPolygon = b->shape->GetType() == POLYGON || b->shape->GetType() == BOX;

    if (aIsCircle && bIsCircle) {
        return IsCollidingCircleCircle(a, b, contact);
    }
    if (aIsPolygon && bIsPolygon) {
        return IsCollidingPolygonPolygon(a, b, contact);
    }
    if (aIsPolygon && bIsCircle) {
        return IsCollidingPolygonCircle(a, b, contact);
    }
    if (aIsCircle && bIsPolygon) {
        return IsCollidingPolygonCircle(b, a, contact);
    }
    return false;
}

// !!!
bool CollisionDetection::IsCollidingCircleCircle(Body* a, Body* b, Contact& contact) {

    CircleShape* aCircleShape = (CircleShape*)a->shape;
    CircleShape* bCircleShape = (CircleShape*)b->shape;

    const Vec2 ab = b->position - a->position;
    const float radiusSum = aCircleShape->radius + bCircleShape->radius;
    
    bool isColliding = ab.MagnitudeSquared() <= (radiusSum * radiusSum);

    if (!isColliding) return false;

    contact.a = a;
    contact.b = b;
    contact.normal = ab;
    contact.normal.Normalize();
    contact.start = b->position - contact.normal * bCircleShape->radius;
    contact.end = a->position + contact.normal * aCircleShape->radius;
    contact.depth = (contact.end - contact.start).Magnitude();

    return true;
}

// !!!
bool CollisionDetection::IsCollidingPolygonPolygon(Body* a, Body* b, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*) a->shape;
    const PolygonShape* bPolygonShape = (PolygonShape*) b->shape;

    // a et b
    Vec2 aAxis; // edge de a utilisé pour calculer la plus petite separation
    Vec2 aPoint; // point de b utilisé par a  pour caluler la plus petite sep
    float abSeparation = aPolygonShape->FindMinSeparation(bPolygonShape, aAxis, aPoint);

    if (abSeparation >= 0) return false; // pas de penetration

    // b et a
    Vec2 bAxis;
    Vec2 bPoint;  // point de a ou la séparation de ba est la plus petite
    float baSeparation = bPolygonShape->FindMinSeparation(aPolygonShape, bAxis, bPoint);

    if (baSeparation >= 0) return false; // pas de penetration

    contact.a = a;
    contact.b = b;
    if (abSeparation > baSeparation) {
        contact.normal = -aAxis.Normal();
        contact.depth = abSeparation;
        contact.end = aPoint;
        contact.start = contact.end + contact.normal * contact.depth;
    }
    else
    {
        contact.normal = bAxis.Normal();
        contact.depth = baSeparation;
        contact.start = bPoint;
        contact.end = contact.start - contact.normal * contact.depth;
    }
 
    return true;
}

// !!!
bool CollisionDetection::IsCollidingPolygonCircle(Body* polygon, Body* circle, Contact& contact) {
    const PolygonShape* aPolygonShape = (PolygonShape*)polygon->shape;
    const CircleShape* circleShape = (CircleShape*)circle->shape;

    Vec2 edge;
    Vec2 minCurveVertex; 
    Vec2 minNextCurveVertex;
    bool isOutside = false;
    float distanceCircleEdge = std::numeric_limits<float>::lowest();

    for (int i = 0; i < aPolygonShape->worldVertices.size(); i++) {
        int nextVertexIndex = (i + 1) % aPolygonShape->worldVertices.size();

        Vec2 currentVertex = aPolygonShape->worldVertices[i];
        Vec2 edgeNormal = aPolygonShape->EdgeAt(i).Normal();
        Vec2 circlePosition = circle->position;

        float projection = (circlePosition - currentVertex).Dot(edgeNormal);
        
        // 
        if (projection > 0) {
            distanceCircleEdge = projection;
            minCurveVertex = currentVertex;
            minNextCurveVertex = aPolygonShape->worldVertices[nextVertexIndex];
            isOutside = true;
        }
        else {
            // Keep the min 
            if (projection > distanceCircleEdge) {
                distanceCircleEdge = projection;
                minCurveVertex = currentVertex;
                minNextCurveVertex = aPolygonShape->worldVertices[nextVertexIndex];
            }
        }
    }

    if (isOutside) {
        Vec2 v1 = circle->position - minCurveVertex;
        Vec2 v2 = minNextCurveVertex - minCurveVertex;
        
        // check region A
        if (v1.Dot(v2) < 0) {
            if (v1.Magnitude() > circleShape->radius) {
                return false;
            }
            else {
                contact.a = polygon;
                contact.b = circle;
                contact.depth = circleShape->radius - v1.Magnitude();
                contact.normal = v1.Normalize();
                contact.start = circle->position + (contact.normal * -circleShape->radius);
                contact.end = contact.start + contact.normal * contact.depth;
            }
        }
        // check region B
        else {
            v1 = circle->position - minNextCurveVertex;
            v2 = minCurveVertex - minNextCurveVertex;
            if (v1.Dot(v2) < 2) {
                if (v1.Magnitude() > circleShape->radius) {
                    return false;
                }
                else {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - v1.Magnitude();
                    contact.normal = v1.Normalize();
                    contact.start = circle->position + (contact.normal * -circleShape->radius);
                    contact.end = contact.start + contact.normal * contact.depth;
                }
            // check region C
            } else {
                if (distanceCircleEdge > circleShape->radius) {
                    return false;
                }
                else {
                    contact.a = polygon;
                    contact.b = circle;
                    contact.depth = circleShape->radius - distanceCircleEdge;
                    contact.normal = (minNextCurveVertex - minCurveVertex).Normal();
                    contact.start = circle->position - contact.normal * circleShape->radius;
                    contact.end = contact.start + contact.normal * contact.depth;
                }
            }
        }
    } else {
        contact.a = polygon;
        contact.b = circle;
        contact.depth = circleShape->radius - distanceCircleEdge;
        contact.normal = (minNextCurveVertex - minCurveVertex).Normal();
        contact.start = circle->position - contact.normal * circleShape->radius;
        contact.end = contact.start + contact.normal * contact.depth;
    }
    
    return true;
}
