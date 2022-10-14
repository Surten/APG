#include "Rasterizer.h"

Rasterizer::Rasterizer(Context* ContextActive){
    Con = ContextActive;
}

void Rasterizer::DrawPoint(int x, int y){
    //use color buffer in ConActive->color_buffer as an output
    //in the correct place put ConActive->currentColor
    
    Con->color_buffer[3*(y*Con->frameWidth + x)] = Con->currentColor[0];
    Con->color_buffer[3*(y*Con->frameWidth + x) + 1] = Con->currentColor[1];
    Con->color_buffer[3*(y*Con->frameWidth + x) + 2] = Con->currentColor[2];
    
}

void Rasterizer::DrawLine(int x1, int y1, int x2, int y2){
    //use color buffer in ConActive->color_buffer as an output
    //in the correct place put ConActive->currentColor
}

void Rasterizer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    //use color buffer in ConActive->color_buffer as an output
}