//---------------------------------------------------------------------------
// nffwrite.cpp
// An example implementation of NFF callbacks that writes NFF data back to a file.
// Date:  2007/10/04
// Author: Jaroslav Krivanek, CTU Prague
//---------------------------------------------------------------------------

#ifndef __NFFWRITE_H__
#define __NFFWRITE_H__

// standard headers
#include <cstdio>

// project headers
#include "nffread.h"

/// An example implementation of NFF callbacks that writes NFF data back to a file.
class CNFFWrite:
  public NFFCallbacks
{
  /// stream to write the NFF data to
  FILE *_fout;

public:

  CNFFWrite(FILE *fout) : _fout(fout) {}

  virtual void Background(const nff_col3 &col)
  { fprintf(_fout, "b %g %g %g\n\n",col.r,col.g,col.b); }

  virtual void HDRBackground(const char *hdr_name)
  { fprintf(_fout, "B %s\n\n",hdr_name); }

  virtual void Camera(
    const nff_vec3 &from,
    const nff_vec3 &at,
    const nff_vec3 &up,
    const float angle,
    const float hither,
    int   width,
    int   height)
  {
    fprintf(_fout,
      "v\n"
      "from %g %g %g\n"
      "at %g %g %g\n"
      "up %g %g %g\n"
      "angle %g\n"
      "hither %g\n"
      "resolution %d %d\n\n",
      from.x, from.y, from.z,
      at.x, at.y, at.z,
      up.x, up.y, up.z,
      angle,
      hither,
      width,
      height );
  }

  virtual void PointLight(const nff_vec3 &pos, nff_col3 &intensity)
  {
    fprintf(_fout, "l %g %g %g  %g %g %g\n\n",
      pos.x,pos.y,pos.z, intensity.r,intensity.g,intensity.b);
  }

  virtual void SetMaterial(
    const nff_col3 &col,
    float kd,
    float ks,
    float shine,
    float T,
    float ior)
  {
    fprintf(_fout,"f %g %g %g %g %g %g %g %g\n\n",
      col.r,col.g,col.b,kd,ks,shine,T,ior);
  }

  virtual void Sphere(const nff_vec3 &c,float r)
  {
    fprintf(_fout,"s %g %g %g %g\n\n",c.x,c.y,c.z,r);
  }

  virtual void PolyPatch(const std::vector<nff_vec3> &vertices, const std::vector<nff_vec3> &normals)
  {
    int n=(int)vertices.size();
    fprintf(_fout,"%s %d\n",normals.size()==n?"pp":"p",n);
    if(normals.size()==n)
      for(int i=0; i<n; i++)
        fprintf(_fout,"%g %g %g   %g %g %g\n",
        vertices[i].x,vertices[i].y,vertices[i].z,
        normals[i].x, normals[i].y, normals[i].z);
    else
      for(int i=0; i<n; i++)
        fprintf(_fout,"%g %g %g\n",
        vertices[i].x,vertices[i].y,vertices[i].z);
    fprintf(_fout,"\n");
  }

  virtual void AreaLightBegin(const nff_col3 &intensity, const nff_vec3 &atten)
  {
    float m = intensity.max();
    if(m==0) m=1;
    fprintf(_fout, "L %g %g %g %g atten %g %g %g\n\n",
      intensity.r/m,intensity.g/m,intensity.b/m,m,
      atten.x,atten.y,atten.z);
  }

  virtual void AreaLightEnd()
  { fprintf(_fout, "L 0 0 0 0\n\n"); }
};



#endif // __NFFWRITE_H__
