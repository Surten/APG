#pragma once

#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "VBO.h"
#include "Ray.h"
#include "material.h"

class Primitive{
public:
    Material material;

    Primitive(Material &m): material(m){}
    // should be abstract, but it's evening, i'm tired and can't make it work
    virtual bool traceRay(Ray &r, float* tHit) = 0;
    virtual Vertex normalAt(Vertex &v) = 0;
};


class SphereP : public Primitive{
public:
    Vertex center;
    float radius;

    SphereP(float x, float y, float z, float radius, Material &m)
    : Primitive(m), radius(radius){
        center = Vertex{x, y, z, 1.0f};
    }

    bool traceRay(Ray &r, float* tHit);
    Vertex normalAt(Vertex &v);
};


class TraingleP : public Primitive{
public:
    // vertices
    Vertex v1, v2, v3;
    // two edges
    Vertex e1, e2;
    Vertex normal;

    TraingleP(Vertex &v1, Vertex &v2, Vertex &v3, Material &m)
        : Primitive(m), v1(v1), v2(v2), v3(v3){
            e1 = v2 - v1;
            e2 = v3 - v1;
            normal = cross(e1, e2);
        }

    bool traceRay(Ray &r, float* tHit);
    Vertex normalAt(Vertex &v);

};

// TODO: triangle fan?





#endif