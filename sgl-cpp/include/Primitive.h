#pragma once

#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "VBO.h"
#include "Ray.h"


class Primitive{
public:
    float r, g, b;

    Primitive(float r, float g, float b): r(r), g(g), b(b){}
    // should be abstract, but it's evening, i'm tired and can't make it work
    virtual bool traceRay(Ray r, float* tHit) = 0;
};


class SphereP : public Primitive{
public:
    Vertex center;
    float radius;

    SphereP(float x, float y, float z, float radius, float r, float g, float b)
    : Primitive(r, g, b), radius(radius){
        center = Vertex{x, y, z, 1.0f};
    }

    bool traceRay(Ray r, float* tHit);
};


class TraingleP : public Primitive{
public:
    // vertices
    Vertex v1, v2, v3;
    // two edges
    Vertex e1, e2;
    Vertex normal;

    TraingleP(Vertex &v1, Vertex &v2, Vertex &v3, float r, float g, float b)
        : Primitive(r, g, b), v1(v1), v2(v2), v3(v3){
            e1 = v2 - v1;
            e2 = v3 - v1;
            normal = cross(e1, e2);
        }

    bool traceRay(Ray r, float* tHit);

};

// TODO: triangle fan?





#endif