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


bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    float discr = b * b - 4 * a * c; 
    if (discr < 0) return false; 
    else if (discr == 0) x0 = x1 = - 0.5 * b / a; 
    else { 
        float q = (b > 0) ? 
            -0.5 * (b + sqrt(discr)) : 
            -0.5 * (b - sqrt(discr)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
    if (x0 > x1) std::swap(x0, x1); 
 
    return true; 
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool SphereP::traceRay(Ray ray, float* tHit){
    float t0, t1;

    Vertex L = ray - center;
    float a = dot(ray, ray);
    float b = 2 * dot(ray, L);
    float c = dot(L, L) - radius *  radius;

    if (!solveQuadratic(a, b, c, t0, t1)) return false; 

    if (t0 > t1){
        std::swap(t0, t1);
    }

    if (t0 < 0){
        t0 = t1;
        if (t0 < 0){
            return false;
        }
    }

    *tHit = t0;

	return true;
}





