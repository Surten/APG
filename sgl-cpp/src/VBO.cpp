#include "VBO.h"

void VBO::InsertVertex(float x, float y, float z, float w)
{
    vertex_buffer.push_back(x);
    vertex_buffer.push_back(y);
    vertex_buffer.push_back(z);
    vertex_buffer.push_back(w);
    currIndex += 4;

}

void VBO::ClearVBO(){
    currIndex = 0;
    //do I need to erase everything??
    // feels like I can just override it
}

size_t VBO::GetSize(){
    return currIndex;
}

VBO::VBO(){}
VBO::~VBO(){}

