//---------------------------------------------------------------------------
// nffstore.cpp
// Simple storage of data from a NFF file.
// Date:  2007/10/04
// Author: Jaroslav Krivanek, CTU Prague
//---------------------------------------------------------------------------

#ifndef __NFFSTORE_H__
#define __NFFSTORE_H__

// standard headers
#include <cstdio>
#define _USE_MATH_DEFINES
#include <string.h>
#include <cmath>
#include <stack>
#include <list>
#include <iostream>
using namespace std;

// project headers
#include "nffread.h"
#include "hdrloader.h"


/// Simple storage of data from a NFF file.
class NFFStore:
  public NFFCallbacks
{
public: // data

  /// material specification
  class Material {
  public: // data
    nff_col3 col;
    float kd, ks, shine, T, ior;
  public: // methods
    Material(const nff_col3 &xcol, float xkd, float xks, float xshine, float xT, float xior):
            col(xcol), kd(xkd), ks(xks), shine(xshine), T(xT), ior(xior) {}
  };

  /// triangle geometry
  class Triangle {
  public: // data
    nff_vec3 vertices[3];
  public: // methods
    Triangle(const nff_vec3 &v0, const nff_vec3 &v1, const nff_vec3 &v2)
    { vertices[0] = v0; vertices[1] = v1; vertices[2] = v2; }
    Triangle(const std::vector<nff_vec3>& v)
    { memcpy(vertices,&v[0],3*sizeof(nff_vec3)); }
  };

  class Sphere {
  public: // data
    nff_vec3 center;
    float radius;
  public: // methods
    Sphere() {}
    Sphere(const nff_vec3 &c, const float r):center(c), radius(r) {}
  };

  class PointLight {
  public:
    nff_vec3 position;
    nff_col3 intensity;
  public:
    PointLight():position(0,0,0), intensity(1,1,1) {}
    PointLight(const nff_vec3 &pos, const nff_col3 &inten):position(pos), intensity(inten) {}
  };

  typedef std::list<Triangle> TriangleList;
  typedef std::list<Sphere> SphereList;


  /// one material with a list of geometry that has the meteria assigned to it
  class LightGroup {
  public: // data
    nff_col3 intensity;
    nff_vec3 atten;

    TriangleList geometry;
    SphereList spheres;
  public: // methods
    LightGroup(const nff_col3 &i, const nff_vec3 &a):intensity(i), atten(a) {}
  };


  /// one material with a list of geometry that has the meteria assigned to it
  class MaterialGroup {
  public: // data
    Material material;
    TriangleList geometry;
    SphereList spheres;
  public: // methods
    MaterialGroup(const nff_col3 &xcol, float xkd, float xks, float xshine, float xT, float xior):
                  material(xcol,xkd,xks,xshine,xT,xior) {}
  };

  /// true if speheres should be tesselated into triangles
  bool tesselateSpheres;

  bool areaLightMode;

  /// background
  nff_col3 bg_col;

  HDRLoaderResult envMap;

  /// camera
  nff_vec3 from, at, up;
  float angle, hither;
  int width, height;
  /// geometry & materials
  typedef std::list<MaterialGroup> TMatGroupList;
  TMatGroupList matgroups;

  typedef std::list<LightGroup> TLightGroupList;
  TLightGroupList lightgroups;

  std::list<PointLight> pointLights;

public: // methods

  NFFStore(const bool ts = true):tesselateSpheres(ts) {
    // set default mode to "normal" surfaces
    areaLightMode = false;
    // empty env map for a start
    envMap.cols = NULL;
  }

  virtual ~NFFStore() {}
  virtual void Background(const nff_col3 &col)
  { bg_col = col; }

  // not implemented yet
  virtual void HDRBackground(const char *hdr_name) {
    HDRLoader loader;

    if (!loader.load(hdr_name, envMap)) {
      cout<<"Cound not read hdr env map !"<<hdr_name<<endl;
      envMap.cols = NULL;
    }
  }

  virtual void Camera(
    const nff_vec3 &f,const nff_vec3 &a,const nff_vec3 &u,
    const float ang,const float hit,
    int   w, int h)
  {
    from = f;  at = a;   up = u;
    angle = ang;  hither = hit;
    width  = w;   height = h;
  }

  // not implemented
  virtual void AddPointLight(const nff_vec3 &pos, const nff_col3 &intensity) {
    pointLights.push_back(PointLight(pos, intensity));
  }

  virtual void SetMaterial(
    const nff_col3 &col, float kd, float ks, float shine, float T, float ior)
  {
    matgroups.push_back(MaterialGroup(col,kd,ks,shine,T,ior));
  }

  /// convert spherical coordinates to a 3D vector in cartesian coordinates
  inline nff_vec3 sph2cart(float theta,float phi)
  {
    float sin_theta = sin(theta);
    return nff_vec3(sin_theta*cos(phi), sin_theta*sin(phi), cos(theta));
  }

  /// helper class for sphere triangulation
  class TD {
  public: // data
    Triangle t;
    int depth;
  public: // methods
    TD(const nff_vec3& v0,const nff_vec3& v1,const nff_vec3& v2,int d):
      t(v0,v1,v2), depth(d) {}
  };

  /// truangulate and store a sphere
  virtual void AddSphere(const nff_vec3 &ctr,float r)
  {
    // default materials (in case material not specified)
    if(matgroups.empty())
      matgroups.push_back(MaterialGroup(nff_col3(1,1,1),0.7,0.0,32,0,1));

    if (!tesselateSpheres) {
      matgroups.back().spheres.push_back(Sphere(ctr,r));
    } else {
      std::list<Triangle> &dest = matgroups.back().geometry;

      std::stack<TD> stack;

      // generate octahedron faces
      nff_vec3 v[7] = {
        nff_vec3( 0, 0, 0),
        nff_vec3( 1, 0, 0),
        nff_vec3(-1, 0, 0),
        nff_vec3( 0, 1, 0),
        nff_vec3( 0,-1, 0),
        nff_vec3( 0, 0, 1),
        nff_vec3( 0, 0,-1)
      };

      stack.push(TD(v[1],v[3],v[5],0));
      stack.push(TD(v[3],v[1],v[6],0));
      stack.push(TD(v[4],v[1],v[5],0));
      stack.push(TD(v[1],v[4],v[6],0));
      stack.push(TD(v[3],v[2],v[5],0));
      stack.push(TD(v[2],v[3],v[6],0));
      stack.push(TD(v[2],v[4],v[5],0));
      stack.push(TD(v[4],v[2],v[6],0));

      while(!stack.empty()){
        TD c = stack.top();
        stack.pop();
        nff_vec3 *v = c.t.vertices;

        if(c.depth>2) {
          // generate triangle
          for(int i=0; i<3; i++)
            v[i] = v[i]*r + ctr;
          dest.push_back(c.t);
        }
        else {
          // subdivide
          nff_vec3 v01 = normalize(v[0]+v[1]);
          nff_vec3 v12 = normalize(v[1]+v[2]);
          nff_vec3 v20 = normalize(v[2]+v[0]);
          stack.push(TD(v[0],v01,v20,c.depth+1));
          stack.push(TD(v[1],v12,v01,c.depth+1));
          stack.push(TD(v[2],v20,v12,c.depth+1));
          stack.push(TD(v01,v12,v20,c.depth+1));
        }
      }
    }
  }

  virtual void PolyPatch(const std::vector<nff_vec3> &vertices, const std::vector<nff_vec3> &normals)
  {
    // sanity check
    if(vertices.size()<3) return;

    if (areaLightMode) {
      // add to the last material group
      _triangulateInto(lightgroups.back().geometry, vertices);
    } else {
      // default materials (in case material not specified)
      if(matgroups.empty())
        matgroups.push_back(MaterialGroup(nff_col3(1,1,1),0.7,0.0,32,0,1));

      // add to the last material group
      _triangulateInto(matgroups.back().geometry,vertices);
    }
  }

  /// not implemented
  virtual void AreaLightBegin(const nff_col3 &intensity, const nff_vec3 &atten) {
    lightgroups.push_back(LightGroup(intensity, atten));
    areaLightMode = true;
  }
  // not implemented
  virtual void AreaLightEnd() {
    areaLightMode = false;
  }

protected:
  /// triangulate a quad
  void _triangulateInto(std::list<Triangle> &dest, const std::vector<nff_vec3> &vertices)
  {
    if(vertices.size()>4) {
      fprintf(stdout,"Cannot triangulate polygons with more than 4 vertices.\n");
      exit(2);
    }

    if(vertices.size()==3) {
      dest.push_back(Triangle(vertices));
      return;
    }

    // split along the shorter diagonal
    float d1 = sqrdist(vertices[0],vertices[2]);
    float d2 = sqrdist(vertices[1],vertices[3]);
    if(d1<d2) {
      dest.push_back(Triangle(vertices[0],vertices[1],vertices[2]));
      dest.push_back(Triangle(vertices[0],vertices[2],vertices[3]));
    }
    else {
      dest.push_back(Triangle(vertices[0],vertices[1],vertices[3]));
      dest.push_back(Triangle(vertices[1],vertices[2],vertices[3]));
    }
  }
};

#endif // __NFFSTORE_H__
