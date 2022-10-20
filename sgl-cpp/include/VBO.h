#ifndef _VBO_H_
#define _VBO_H_

#include <vector>
#include <algorithm>
#include "Matrix4f.h"


struct Viewport{
    int x;
    int y;
    int width;
    int height;

    float near;
    float far;

    Matrix4f viewportMatrix;

    void CreateViewportMatrix();
};



class VBO{
    size_t allocated = 1000;
    size_t currIndex = 0;

public:
    std::vector<float> vertex_buffer;

    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);
    void InsertVertexAt(float x, float y, float z, float w, size_t index);
    void ClearVBO();
    size_t GetSize();

};

#endif