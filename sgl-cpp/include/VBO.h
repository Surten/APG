#ifndef _VBO_H_
#define _VBO_H_

#include <vector>


struct Viewport{
    int x;
    int y;
    int width;
    int height;
};



class VBO{
    size_t allocated = 5000;
    size_t currIndex = 0;

public:
    std::vector<float> vertex_buffer;

    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);
    void ClearVBO();
    size_t GetSize();

};

#endif