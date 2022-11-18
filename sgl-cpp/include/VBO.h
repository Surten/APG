#ifndef _VBO_H_
#define _VBO_H_

#include <vector>
#include <algorithm>
#include "Matrix4f.h"
#include "Vertex.h"

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