#include "Primitive.h"

bool TriangleP::facesVector(Vertex &v){
    return dot(v0 - v, normal) > 0.0f;
}

void TriangleP::transform(Matrix4f &mat){
    mat.MultiplyVector(v0);
    mat.MultiplyVector(v1);
    mat.MultiplyVector(v2);
    mat.MultiplyVector(e0);
    mat.MultiplyVector(e1);
    mat.MultiplyVector(normal);
    normal.normalize();

}

Vertex TriangleP::normalAt(Vertex &v){
    normal.normalize();
    return normal;
}

// Moller
// https://sci-hub.se/10.1080/10867651.1997.10487468
bool TriangleP::traceRay(Ray &ray, float* tHit){
    // normal.normalize();
    if (dot(ray.direction, normal) < 0.0f){
        return false;
    }

    Vertex pvec = cross(ray.direction, e2);
    float det, inv_det;
    det = dot(e1, pvec);
    if (det < 0.00001f){
        return false;
    }
    Vertex tvec = ray.origin - v0;
    float u = dot(tvec, pvec);
    if (u < 0.0f || u > det){
        return false;
    }

    Vertex qvec = cross(tvec, e1);

    float v = dot(ray.direction, qvec);
    if (v < 0.0 || u + v > det){
        return false;
    }
    float t = dot(e2, qvec);
    inv_det = 1.0f / det;
    t *= inv_det;

    *tHit = t;

    if (t < ray.tMin || t > ray.tMax){
        return false;
    }
    return true;
}




bool solveQuadratic(const float &a, const float &b, const float &c, float &x0, float &x1) 
{ 
    float discr = b * b - 4.0f * a * c; 
    if (discr < 0.0f) return false; 
    else if (discr == 0.0f) x0 = x1 = - 0.5f * b / a; 
    else { 
        float q = (b > 0.0f) ? 
            -0.5f * (b + sqrt(discr)) : 
            -0.5f * (b - sqrt(discr)); 
        x0 = q / a; 
        x1 = c / q; 
    } 
    if (x0 > x1) std::swap(x0, x1); 
 
    return true; 
}


bool SphereP::facesVector(Vertex &v){
    return true;
}

void SphereP::transform(Matrix4f &mat){
    mat.MultiplyVector(center);
}


Vertex SphereP::normalAt(Vertex &v){
    Vertex normal = center - v;
    normal.normalize();
    return normal;
}

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool SphereP::traceRay(Ray &ray, float* tHit){
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

    if (t0 < ray.tMin || t0 > ray.tMax){
        return false;
    }

	return true;
}





