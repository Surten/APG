#include "VBO.h"



void Viewport::CreateViewportMatrix(){
    viewportMatrix.matrix[0] = width / 2;
    viewportMatrix.matrix[3] = (width - 1) / 2;
    viewportMatrix.matrix[5] = height / 2;
    viewportMatrix.matrix[7] = (height - 1) / 2;
    viewportMatrix.matrix[10] = (far - near) / 2;
    viewportMatrix.matrix[10] = (far + near) / 2;
    viewportMatrix.matrix[15] = 1;
}

void VBO::InsertVertex(float x, float y, float z, float w)
{
    if(allocated < currIndex+8){
        allocated = allocated*5;
        vertex_buffer.resize(allocated, 0);
    }
    vertex_buffer.at(currIndex++) = x;
    vertex_buffer.at(currIndex++) = y;
    vertex_buffer.at(currIndex++) = z;
    vertex_buffer.at(currIndex++) = w;
    //currIndex += 4;

}

void VBO::InsertVertexAt(float x, float y, float z, float w, size_t index){
    index *=4;
    currIndex = std::max(currIndex, index+4);

    vertex_buffer.at(index) = x;
    vertex_buffer.at(index+1) = y;
    vertex_buffer.at(index+2) = z;
    vertex_buffer.at(index+3) = w;

}

void VBO::ClearVBO(){
    currIndex = 0;
    //do I need to erase everything??
    // feels like I can just override it
}

size_t VBO::GetSize(){
    return currIndex;
}

VBO::VBO(){
    vertex_buffer.resize(allocated, 0);
}
VBO::~VBO(){}

