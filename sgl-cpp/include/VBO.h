#ifndef _VBO_H_
#define _VBO_H_

#include <vector>
#include <algorithm>
#include "Matrix4f.h"

/**
 * Viewport holds info about transforming Normalized device coordinates to Screen coordinates 
*/
struct Viewport{
    float x;
    float y;
    float width;
    float height;


    Matrix4f viewportMatrix;

    void CreateViewportMatrix();
};

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


/**
 * Vertex Buffer Object holds all the verticies in the current drawing 
 * until they are passed to rasterizer
 * meaning that verticies are updated in place during Vertex buffer transform and viewport
*/
class VBO{
    size_t allocated = 0;
    size_t currIndex = 0;

public:
    std::vector<Vertex> vertex_buffer;

    VBO();
    ~VBO();
    /**
     * Inserts vertex at the end of VBO
    */
    void InsertVertex(float x, float y, float z, float w);
    void InsertVertex(Vertex v);


    /**
     * Sets VBO Index to 0, does not actually set values in the vertex_buffer array
    */
    void ClearVBO();

    size_t GetSize();

};

#endif