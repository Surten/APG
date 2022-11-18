#ifndef _VERTEX_H
#define _VERTEX_H

#include <algorithm>

/**
 * Verticies in VBO
*/
struct Vertex{
    float x;
    float y;
    float z;
    float w;
    Vertex(){x = y = z = 0.0f; w = 1.0f;}
    //Vertex(Vertex& v){x=v.x;y=v.y;z=v.z;w=v.w;}
    Vertex(float xx, float yy, float zz, float ww) : x(xx), y(yy), z(zz), w(ww){}
    Vertex(float xx, float yy, float zz) : x(xx), y(yy), z(zz), w(1.0f){}

    Vertex operator-(Vertex const &v){
        float wRatio = w / v.w;
        x /= wRatio;
        y /= wRatio;
        z /= wRatio;
        w /= wRatio;

        return Vertex(x - v.x, y - v.y, z - v.z, v.w);
    }

    Vertex operator+(Vertex const&v){
        float wRatio = w / v.w;
        x /= wRatio;
        y /= wRatio;
        z /= wRatio;
        w /= wRatio;
        
        return Vertex(x + v.x, y + v.y, z + v.z, v.w);
    }

    void perspDivide(){
        x /= w;
        y /= w;
        z /= w;
        w = 1.0f;
    }

    void normalize(){
        perspDivide();
        float len = sqrtf(x*x + y*y + z*z);
        x /= len;
        y /= len;
        z /= len;
    }
};

Vertex operator*(float c, Vertex& v);
Vertex operator*(Vertex& v, float c);












#endif