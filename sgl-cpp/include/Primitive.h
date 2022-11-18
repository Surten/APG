
#ifndef _PRIMITIVE_H
#define _PRIMITIVE_H

#include "Vertex.h"
#include "Ray.h"
#include "material.h"
#include "Matrix4f.h"

class Primitive{
public:
    Material material;

    Primitive(Material &m): material(m){}
    // should be abstract, but it's evening, i'm tired and can't make it work
    virtual bool traceRay(Ray &r, float* tHit) = 0;
    virtual Vertex normalAt(Vertex &v) = 0;
    virtual void transform(Matrix4f &mat) = 0;
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
    virtual void transform(Matrix4f &mat);
};


class TriangleP : public Primitive{
public:
    // vertices
    Vertex v0, v1, v2;
    // two edges
    Vertex e0, e1, e2;
    Vertex normal;

    TriangleP(Vertex &v0, Vertex &v1, Vertex &v2, Material &m)
        : Primitive(m), v0(v0), v1(v1), v2(v2){
            e0 = v2 - v1;
            e1 = v1 - v0;
            e2 = v2 - v0;
            normal = cross(e0, e1);
            normal.normalize();
        }

    bool traceRay(Ray &r, float* tHit);
    Vertex normalAt(Vertex &v);
    virtual void transform(Matrix4f &mat);

};

// TODO: triangle fan?





#endif