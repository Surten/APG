#pragma once

#ifndef _RAY_H
#define _RAY_H

#include "VBO.h"

struct Ray{
    float x;
    float y;
    float z;
    float dx;
    float dy;
    float dz;
    float tMin;
    float tMax;
    
    Ray(float x, float y, float z, float dx, float dy, float dz, float tMin, float tMax):
        x(x), y(y), z(z), dx(dx), dz(dz), tMin(tMin), tMax(tMax){}

    Ray(Vertex& position, Vertex& direction, float tMin, float tMax):
        tMin(tMin), tMax(tMax){
            x = position.x;
            y = position.y;
            z = position.z;
            dx = direction.x;
            dy = direction.y;
            dz = direction.z;

        }

    Vertex operator-(Vertex& v){
        return Vertex(
            x - v.x,
            y - v.y,
            z - v.z,
            1.0
        );
}
};

inline float dot(Vertex& v1, Vertex& v2){
    // TODO: w division?
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline float dot(Ray& r1, Vertex& v2){
    // TODO: w division?
    return r1.dx * v2.x + r1.dy * v2.y + r1.dz * v2.z;
}

inline float dot(Vertex& v2, Ray& r1){
    // TODO: w division?
    return r1.dx * v2.x + r1.dy * v2.y + r1.dz * v2.z;
}

inline float dot(Ray& r1, Ray& r2){
    return r1.dx * r2.dx + r1.dy * r2.dy + r1.dz * r2.dz;
}

inline Vertex cross(Vertex &v1, Vertex &v2){
    // TODO: w division?
    return Vertex{
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x,
        1.0f};
}

inline Vertex cross(Vertex &v, Ray &r){
    // TODO: w division?
    return Vertex{
         v.y * r.dz - v.z * r.dy,
        v.z * r.dx - v.x * r.dz,
        v.x * r.dy - v.y * r.dx,
        1.0f};
}

inline Vertex cross(Ray &r, Vertex &v){
    // TODO: w division?
    return Vertex{
        r.dy * v.z - r.dz * v.y,
        r.dz * v.x - r.dx * v.z,
        r.dx * v.y - r.dy * v.x,
        1.0f};
}

#endif