#include "Primitive.h"


Color Primitive::getColorFromLightSource(PointLight &light, Vertex &position, Vertex &lookDirection){
    Color color{.0f, .0f, .0f};
    Vertex normal = normalAt(position);

    Vertex L = light.position - position;  // light direction
    L.normalize();
    float cosA = dot(L, normal);
    color += light.color * (material.color * material.kd) * std::max(cosA, 0.0f);
    // specular
    Vertex R = 2 * dot(L, normal) * normal - L; // reflected
    R.normalize();
    float cosB = dot(lookDirection, R);
    color += light.color  * material.ks * powf(std::max(cosB, 0.0f), material.shine);

    return color;
}

Ray Primitive::getReflectedRay(Ray &ray, Vertex &point){
    Vertex reverseRayDir = ray.direction * (-1);
    Vertex normal = normalAt(point);
    Vertex R = 2 * dot(reverseRayDir, normal) * normal - reverseRayDir; 
    return Ray(point, R, 0, INFINITY);
}

Ray Primitive::getRefractedRay(Ray &ray, Vertex &point){
  
// The following code fragment computes the direction of refracted ray (dir)
// given the primary ray direction (hitInfo.GetRay()->GetDir()) and the surface normal (normal)
// Note: hitInfo.GetDotProd()  =  DotProd(hitInfo.GetRay()->GetDir(), hitInfo.GetNormal())

  // get value of index of refraction  coefficient for intersection point

  // currently, rays are not bended according to spectral wavelegth

  // here is the point to be modified for spectral-dependent effects

  Vertex normal = normalAt(point);
  Vertex dir;
  float gamma, sqrterm;

  float dotProd = dot(ray.direction, normal);

  if (dotProd < 0.0) {
    // from outside into the inside of object
    gamma = 1.0 / material.ior;
  }
  else {
    // from the inside to outside of object
    gamma = material.ior;
    dotProd = -dotProd;
    normal = normal * (-1);
  }
  sqrterm = 1.0 - gamma * gamma * (1.0 - dotProd * dotProd);

  // Check for total internal reflection, do nothing if it applies.
  if (sqrterm > 0.0) {
    sqrterm = dotProd * gamma + sqrt(sqrterm);
    Vertex dir = -sqrterm * normal + ray.direction * gamma;
  }
  else  {}
  return Ray(point, dir, 0, INFINITY);

}



bool TriangleP::facesVector(Vertex &v){
    return dot(v - v0, normal) > 0.0f;
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
	if (divisor == 0.0001)
		return false;
	float invDivisor = 1.f / divisor;
	// Compute first barycentric coordinate
	Vertex d = ray.origin - v0;
	float b1 = dot(d, s1) * invDivisor;
	if (b1 < 0.001 || b1 > 1.001)
		return false;
	// Compute second barycentric coordinate
	Vertex s2 = cross(d, e1);
	float b2 = dot(ray.direction, s2) * invDivisor;
	if (b2 < 0.001 || b1 + b2 > 1.001)
		return false;
	// Compute _t_ to intersection point
	float t = dot(e2, s2) * invDivisor;
	if (t < ray.tMin || t > ray.tMax)
		return false;
	*tHit = t;
	return true;
}

void TriangleP::setMinDistFromCamera(Vertex &camera){
    Vertex& distance = v0 - camera;
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

void SphereP::setMinDistFromCamera(Vertex &camera){
    minDistFromCamera = (center - camera).length() - radius;
    minDistFromCamera = std::max(minDistFromCamera, 0.0f);
}





