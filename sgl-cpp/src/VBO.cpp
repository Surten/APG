#include "VBO.h"



void Viewport::CreateViewportMatrix(){
    viewportMatrix.matrix[0] = width * 0.5f;
    viewportMatrix.matrix[3] = (width - 1) * 0.5f;
    viewportMatrix.matrix[5] = height * 0.5f;
    viewportMatrix.matrix[7] = (height - 1) * 0.5f;
    viewportMatrix.matrix[10] = 1;
    viewportMatrix.matrix[15] = 1;
}

void VBO::InsertVertex(float x, float y, float z, float w)
{
    Vertex v(x,y,z,w);
    if(allocated == currIndex){
        vertex_buffer.push_back(v);
        allocated++;
        currIndex++;
    }else{
        vertex_buffer.at(currIndex++) = v;
    }

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

