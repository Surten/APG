#pragma once

#ifndef _RAY_H
#define _RAY_H

#include "Vertex.h"

struct Ray{
    Vertex origin{};
    Vertex direction{};

    float &x = origin.x;
    float &y = origin.y;
    float &z = origin.z;
    float &dx = direction.x;
    float &dy = direction.y;
    float &dz = direction.z;
    float tMin = 0;
    float tMax = INFINITY;

    Ray(){}
    
    Ray(float x, float y, float z, float dx, float dy, float dz, float tMin, float tMax):
        x(x), y(y), z(z), dx(dx), dz(dz), tMin(tMin), tMax(tMax){}

    Ray(Vertex& position, Vertex& direction, float tMin, float tMax):
        tMin(tMin), tMax(tMax){
            origin.x = position.x;
            origin.y = position.y;
            origin.z = position.z;
            this->direction.x = direction.x;
            this->direction.y = direction.y;
            this->direction.z = direction.z;

        }

    void setProperties(Vertex& position, Vertex& direction, float tMin, float tMax){
            origin.x = position.x;
            origin.y = position.y;
            origin.z = position.z;
            this->direction.x = direction.x;
            this->direction.y = direction.y;
            this->direction.z = direction.z;
            this->tMax = tMax;
            this->tMin = tMin;

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