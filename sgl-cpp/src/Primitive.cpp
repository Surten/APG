#include "Primitive.h"

bool TraingleP::traceRay(Ray ray, float* tHit){
    // uses pbrt from courseware
    // https://cent.felk.cvut.cz/courses/APG/triangle-pbrt.cpp

    Vertex s1 = cross(ray, e2);
    float divisor = dot(s1, e1);
    if (divisor == 0){
        return false;
    }
    float invDivisor = 1.0f / divisor;

    Vertex d = ray - v1; 
    float b1 = dot(d, s1) * invDivisor;
    if (b1 < 0.0f || b1 > 1.0f){
        return false;
    }

    Vertex s2 = cross(d, e1);
    float b2 = dot(ray, s2) * invDivisor;

    if (b2 < 0.0f || b1 + b2 > 1.0f){
        return false;
    }
    float t = dot(e2, s2) * invDivisor;
    if (t < ray.tMin || t > ray.tMax){
        return false;
    }
    *tHit = t;
    return true;
}


bool SphereP::traceRay(Ray ray, float* tHit){
    Vertex distance = ray - position;
    float b = dot(distance, ray);
    float c = dot(distance, distance) - radius * radius;
    float d = b * b - c;

    float t;
    if (d > 0){
        t = -b - sqrtf(d);
        if (t < 0.0f){
            *tHit = -b + sqrtf(d);
        }
        return true;
    }
	return false;
}



