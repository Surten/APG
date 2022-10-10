//---------------------------------------------------------------------------
// nffread.cpp
// Simple NFF reader.
// Date:  2007/10/04
// Author: Jaroslav Krivanek, CTU Prague
//---------------------------------------------------------------------------

#include "nffread.h"

// standard headers
#include <cassert>
#include <cstring>
#include <vector>

static void
_getNextToken(FILE* fin,char *nexttok)
{
  if (feof(fin))
    goto end_of_file;

  while(1) {
    if( fscanf(fin, " %s", nexttok ) != 1 ) {
      // we could have reached the end of file while looking for non-white chars
      if(feof(fin))
        goto end_of_file;
      else
        strcpy(nexttok,"ERROR: could no get next token");
    }
    else {
      // let's see if it is not a comment
      if (nexttok[0] == '#') {
        char c;
        do { c = fgetc(fin); } while (!feof(fin) && c!='\n');
      }
      else
        return;
    }
  }

end_of_file:
  strcpy(nexttok,"_");

  return;
}

int
ReadNFF(FILE *fin, char *errstring, NFFCallbacks *callbacks)
{
  char nexttok[4000];
  int ret;

  while (1) {
    _getNextToken(fin, nexttok);

    switch (nexttok[0]) {

    case 'v':
      // ------------------------------
      //  'v' camera
      // ------------------------------
      {
        nff_vec3 from;
        nff_vec3 at;
        nff_vec3 up;
        float    angle, hither;
        int      width, height;

        ret = fscanf(fin,
          " from %g %g %g\n"
          "at %g %g %g\n"
          "up %g %g %g\n"
          "angle %g\n"
          "hither %g\n"
          "resolution %d %d",
          &(from.x), &(from.y), &(from.z),
          &(at.x), &(at.y), &(at.z),
          &(up.x), &(up.y), &(up.z),
          &(angle), &(hither), &(width), &(height));

        if ( ret != 13 ) {
          sprintf(errstring, "Could not read camera.");
          return -1;
        }

        callbacks->Camera(from,at,up,angle,hither,width,height);
      }
      break;

    case 'b':
      // ------------------------------
      //  'b' background
      // ------------------------------
      {
        nff_col3 col;

        ret = fscanf(fin, " %g %g %g\n", &(col.r), &(col.g), &(col.b) );

        if ( ret != 3 ) {
          sprintf(errstring, "Could not read background.");
          return -1;
        }

        callbacks->Background(col);
      }
      break;

    case 'l':
      // ------------------------------
      //  'l' point light
      // ------------------------------
      {
        nff_vec3 pos;
        nff_col3 i;

        ret = fscanf(fin," %g %g %g %g %g %g\n",
          &(pos.x),&(pos.y),&(pos.z),&(i.r),&(i.g),&(i.b));

        if ( ret != 6 && ret != 3 ) {
          sprintf(errstring, "Could not read point light.");
          return -1;
        }

        if(ret == 3)
          i.r = i.g = i.b = 1.0;

        callbacks->AddPointLight(pos,i);
      }
      break;

  case 'B':
      // ------------------------------
      //  'B' HDR env map
      // ------------------------------
      {
        char s[256];
        ret = fscanf(fin," %s\n",
           s);
        if ( ret != 1 ) {
          sprintf(errstring, "Could not read env map name.");
          return -1;
        }

        callbacks->HDRBackground(s);
      }
      break;

  case 'L':
      // ------------------------------
      //  'L' emmisive material (area lights)
      // ------------------------------
      {
        nff_col3 i;
        nff_vec3 atten;
        float m;
        ret = fscanf(fin," %g %g %g %g atten %g %g %g\n",
           &(i.r),&(i.g),&(i.b),
           &m,
           &atten.x, &atten.y, &atten.z
           );

        if ( ret != 4 && ret != 7 ) {
          sprintf(errstring, "Could not read area light.");
          return -1;
        }

        if ( ret == 4 ) {
          atten.x = 0;
          atten.y = 0;
          atten.z = 0;
        }

        i.r *= m/M_PI;
        i.g *= m/M_PI;
        i.b *= m/M_PI;

        if (ret == 4 && i.r == 0 && i.g == 0 && i.b == 0 && m == 0)
          callbacks->AreaLightEnd();
        else
          callbacks->AreaLightBegin(i, atten);
      }
      break;

    case 'f':
      // ------------------------------
      //  'f' phong material
      // ------------------------------
      {
        nff_col3 col;
        float kd,ks,shine,T,ior;

        ret = fscanf(fin," %g %g %g %g %g %g %g %g",
          &(col.r),&(col.g),&(col.b),
          &(kd),&(ks),&(shine),&(T),&(ior));

        if( ret != 8 ) {
          sprintf(errstring, "Could not read material.");
          return -1;
        }

        callbacks->SetMaterial(col,kd,ks,shine,T,ior);
      }
      break;

    case 's':
      // ------------------------------
      //  's' sphere
      // ------------------------------
      {
        nff_vec3 c;
        float    r;

        ret = fscanf(fin, " %g %g %g %g\n", &(c.x), &(c.y), &(c.z), &r);

        if ( ret !=  4 ) {
          sprintf(errstring, "Could not read sphere.");
          return -1;
        }
        callbacks->AddSphere(c,r);
      }
      break;

    case 'p':
      {
        // ------------------------------
        //  'p'/'pp' polygon/patch
        // ------------------------------
        bool patch = nexttok[1]=='p';
        int num;
        nff_vec3  vertex, normal;
        std::vector<nff_vec3> vertices, normals;

        ret = fscanf(fin, " %d\n", &num);

        if ( ret !=  1 ) {
          sprintf(errstring, "Could not read number of polygon / patch vertices.");
          return -1;
        }

        for(int i=0; i<num; i++) {
          ret = fscanf(fin, " %g %g %g", &(vertex.x),&(vertex.y),&(vertex.z) );
          if(ret != 3 ) {
            sprintf(errstring, "Could not read polygon / patch vertex #%d.",i);
            return -1;
          }
          vertices.push_back(vertex);

          if(!patch) continue;

          ret = fscanf(fin, " %g %g %g", &(normal.x),&(normal.y),&(normal.z) );
          if(ret != 3 ) {
            sprintf(errstring, "Could not read polygon / patch normal #%d.",i);
            return -1;
          }
          normals.push_back(normal);
        }

        if(num>0)
          callbacks->PolyPatch(vertices, normals);
      }
      break;

    case '_':
      // ------------------------------
      //  '_' end of file
      // ------------------------------
      return 0;
      break;

    default:
      sprintf(errstring, "Unknown nff command: %s ",nexttok);
      return -1;
      break;

    }
 }
}
