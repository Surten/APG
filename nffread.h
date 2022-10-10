//---------------------------------------------------------------------------
// nffread.cpp
// Simple NFF reader.
// Date:  2007/10/04
// Author: Jaroslav Krivanek, CTU Prague
//---------------------------------------------------------------------------

#ifndef __NFFREAD_H__
#define __NFFREAD_H__

// standard headers
#include <cstdio>
#include <vector>
#include <cmath>

#ifndef M_PI
#define M_PI 3.1415928
#endif

// return the square of 'a'
static float sqr(float a) {return a*a;}

/// 3d vector
struct nff_vec3
{
  float x, y, z;
  nff_vec3() {}
  nff_vec3(float xx,float yy,float zz): x(xx),y(yy),z(zz) {}
  friend float sqrdist(const nff_vec3& v1,const nff_vec3& v2)
  { return sqr(v1.x-v2.x)+sqr(v1.y-v2.y)+sqr(v1.z-v2.z); }
  friend nff_vec3 operator+(const nff_vec3& v1,const nff_vec3& v2)
  { return nff_vec3(v1.x+v2.x,v1.y+v2.y,v1.z+v2.z); }
  friend nff_vec3 operator-(const nff_vec3& v1,const nff_vec3& v2)
  { return nff_vec3(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z); }
  friend nff_vec3 operator*(const nff_vec3& v1,const float &w)
  { return nff_vec3(v1.x*w,v1.y*w,v1.z*w); }
  friend nff_vec3 operator/(const nff_vec3& v1,const float &w)
  { return operator*(v1,1.0f/w); }
  friend float sqrlen(const nff_vec3& v1)
  { return sqr(v1.x)+sqr(v1.y)+sqr(v1.z); }
  friend float length(const nff_vec3& v1)
  { return sqrtf(sqrlen(v1)); }
  friend nff_vec3 normalize(const nff_vec3& v1)
  { return v1/length(v1); }
};

/// rgb color
struct nff_col3
{
  float r, g, b;
  nff_col3() : r(0.0f), g(0.0f), b(0.0f) {}
  nff_col3(float rr,float gg,float bb): r(rr),g(gg),b(bb) {}
  float max() const { return (r>g) ? (r>b?r:b) : (g>b?r:b); }
};

/// abstract class with virtual methods (callbacks) called by the NFF reader
/** Implement the callbacks according to the needs of your application */
class NFFCallbacks {

public:

  /// background color
  virtual void Background(const nff_col3 &col) = 0;

  /// background HDR map
  virtual void HDRBackground(const char *hdr_name) = 0;

  /// camera specification
  virtual void Camera(
    const nff_vec3 &from,
    const nff_vec3 &at,
    const nff_vec3 &up,
    const float angle,
    const float hither,
    int   width,
    int   height) = 0;

  /// ppint light specification
  virtual void AddPointLight(const nff_vec3 &pos, const nff_col3 &intensity) = 0;

  /// matyerial specification - the following geometry should have this material assigned to it
  virtual void SetMaterial(
    const nff_col3 &col,
    float kd,
    float ks,
    float shine,
    float T,
    float ior) = 0;

  /// sphere
  virtual void AddSphere(const nff_vec3 &c,float r) = 0;

  /// polygonal patch. if 'normals' are empty, they were not specified in the input file
  virtual void PolyPatch(const std::vector<nff_vec3> &vertices, const std::vector<nff_vec3> &normals) = 0;

  /// area light specification. the following geometry emits light
  virtual void AreaLightBegin(const nff_col3 &intensity, const nff_vec3 &atten) = 0;

  /// the following geometry does not emit light
  virtual void AreaLightEnd() = 0;
};

/// parse the NFF file from 'fin'.
/**
  @param fin strem to read the NFF data from
  @param errstring upon error, this contais error description
  @param callbacks implementation of the CNFFCallbacks class, must nop be NULL
  @return negative value if an error occurs
*/
int ReadNFF(FILE *fin, char *errstring, NFFCallbacks *callbacks);

#endif // __NFFREAD_H__
