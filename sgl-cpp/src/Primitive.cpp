#include "Primitive.h"


Color Primitive::getColorFromLightSource(PointLight &light, Vertex &position, Vertex &lookDirection){
    Color color{.0f, .0f, .0f};

    if (material.isEmissive){
        color = material.color;
        return color;    
    }

    Vertex normal = normalAt(position);

    Color lightColor = light.color;
    if (light.useAttenuation){
        float distance = (light.position - position).length();
        float atten = (light.c0 + light.c1 * distance + light.c2 * distance * distance);
        Vertex invRayDirection = (position - light.position);
        invRayDirection.normalize();
        float normRayDot = dot(light.triangleNormal, invRayDirection);
        float weight = light.triangleArea * normRayDot / (AREA_LIGHT_NUM_SAMPLES * atten);
        lightColor = lightColor * weight;
    }

    Vertex L = light.position - position;  // light direction
    L.normalize();
    float cosA = dot(L, normal);
    color += lightColor * (material.color * material.kd) * std::max(cosA, 0.0f);
    // specular
    Vertex R = 2 * dot(L, normal) * normal - L; // reflected
    R.normalize();
    float cosB = dot(lookDirection, R);
    color += lightColor  * material.ks * powf(std::max(cosB, 0.0f), material.shine);

    return color;
}

Ray Primitive::getReflectedRay(Ray &ray, Vertex &point){
    Vertex reverseRayDir = ray.direction * (-1);
    Vertex normal = normalAt(point);
    Vertex R = 2 * dot(reverseRayDir, normal) * normal - reverseRayDir; 
    Vertex originR = point + (normal * 0.01f);
    return Ray(originR, R, 0.0001f, INFINITY);
}

Ray Primitive::getRefractedRay(Ray &ray, Vertex &point){

  Vertex normal = normalAt(point);
  Vertex dir;
  float gamma, sqrterm;

  float dotProd = dot(ray.direction, normal);

  if (dotProd < 0.0f) {
    // from outside into the inside of object
    gamma = 1.0f / material.ior;
  }
  else {
    // from the inside to outside of object
    gamma = material.ior;
    dotProd = -dotProd;
    normal = normal * (-1);
  }
  sqrterm = 1.0f - gamma * gamma * (1.0f - dotProd * dotProd);

  // Check for total internal reflection, do nothing if it applies.
  if (sqrterm > 0.0f) {
    sqrterm = dotProd * gamma + sqrt(sqrterm);
    dir = (-sqrterm * normal) + (ray.direction * gamma);
  }
  else {}
  Vertex originR = point - (normal * 0.01f);
  return Ray(originR, dir, 0.0f, INFINITY);

}



bool TriangleP::facesVector(Vertex &v){
    Vertex vec = v - v0;
    return dot(vec, normal) > 0.0f;
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

/*
bool TriangleP::traceRay(Ray &ray, float* tHit){
    // normal.normalize();
    *tHit = -1;
    if (dot(ray.direction, normal) < 0.0f){
        return false;
    }
    Vertex pvec = cross(e2, ray.direction);
    float det, inv_det;
    det = dot(e1, pvec);
    if (det < 0.00001f){
        return false;
    }
    //printf("2\n");
    Vertex tvec = ray.origin - v0;
    float u = dot(tvec, pvec);
    if (u < 0.0f || u > det){
        return false;
    }
    //printf("3\n");
    Vertex qvec = cross(tvec, e1);

    float v = dot(ray.direction, qvec);
    if (v < 0.0 || u + v > det){
        return false;
    }
    //printf("4\n");
    float t = dot(e2, qvec);
    inv_det = 1.0f / det;
    t *= inv_det;

    *tHit = t;

    if (t < ray.tMin || t > ray.tMax){
        return false;
    }
    //printf("5\n");
    return true;
}
*/

// Find intersection point - from PBRT - www.pbrt.org
bool TriangleP::traceRay(Ray &ray, float *tHit) {
    *tHit = -1;
	Vertex s1 = cross(ray.direction, e2);
	float divisor = dot(s1, e1);
	if (divisor == 0.00001)
		return false;
	float invDivisor = 1.f / divisor;
	// Compute first barycentric coordinate
	Vertex d = ray.origin - v0;
	float b1 = dot(d, s1) * invDivisor;
	if (b1 < 0.0001 || b1 > 1.0001)
		return false;
	// Compute second barycentric coordinate
	Vertex s2 = cross(d, e1);
	float b2 = dot(ray.direction, s2) * invDivisor;
	if (b2 < 0.0001 || b1 + b2 > 1.0001)
		return false;
	// Compute _t_ to intersection point
	float t = dot(e2, s2) * invDivisor;
	if (t < ray.tMin || t > ray.tMax)
		return false;
	*tHit = t;
	return true;
}

void TriangleP::setMinDistFromCamera(Vertex &camera){
    Vertex distance = v0 - camera;
    minDistFromCamera = distance.length();
    distance = v1 - camera;
    minDistFromCamera = std::min(distance.length(), minDistFromCamera);
    distance = v2 - camera;
    minDistFromCamera = std::min(distance.length(), minDistFromCamera);
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
    Vertex normal = v - center;
    normal.normalize();
    return normal;
}
/*
bool SphereP::traceRay(Ray &ray, float* t) {

Vertex dst = ray.origin - center;
const float b = dot(dst, ray.direction);
const float c = dot(dst, dst) - radius*radius;
const float d = b*b - c;

if(d > 0) {
    *t = -b - sqrtf(d);
    if (*t < 0.0f)
    *t = -b + sqrtf(d);

    return true;
}
return false;
}
*/

// https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
bool SphereP::traceRay(Ray &ray, float* tHit){
    float t0, t1;
    *tHit = -1;

    Vertex L = ray - center;
    float a = dot(ray, ray);
    float b = 2 * dot(ray, L);
    float c = dot(L, L) - radius *  radius;

    if (!solveQuadratic(a, b, c, t0, t1)) return false; 

    if (t0 > t1){
        std::swap(t0, t1);
    }

    if (t0 < 0.0f){
        t0 = t1;
        if (t0 < 0.0f){
            return false;
        }
    }

    *tHit = t0;

    if (t0 < ray.tMin || t0 > ray.tMax){
        return false;
    }

	return true;
}

void SphereP::setMinDistFromCamera(Vertex &camera){
    minDistFromCamera = (center - camera).length() - radius;
    minDistFromCamera = std::max(minDistFromCamera, 0.0f);
}





