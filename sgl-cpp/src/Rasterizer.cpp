#include "Rasterizer.h"
#include <cmath>

Rasterizer::Rasterizer(Context* ContextActive){
    Con = ContextActive;
}

void Rasterizer::setPixel(int x, int y){
    
    if(x < Con->frameWidth && x >= 0 && y < Con->frameHeight && y >= 0){
        Con->color_buffer[3*(y*Con->frameWidth + x)] = Con->currentColor[0];
        Con->color_buffer[3*(y*Con->frameWidth + x) + 1] = Con->currentColor[1];
        Con->color_buffer[3*(y*Con->frameWidth + x) + 2] = Con->currentColor[2];
    }
}

void Rasterizer::setPixelBig(int x, int y){
    for (int i = 0; i < Con->pointSize; i++)
    {
        for (int j = 0; j < Con->pointSize; j++)
        {
            setPixel(x-((Con->pointSize-1)/2)+j,y-((Con->pointSize-1)/2)+i);
        }
        
    }
    
}

void Rasterizer::DrawPoint(int x, int y){
    //use color buffer in Con->color_buffer as an output
    //in the correct place put Con->currentColor
    if(Con->pointSize>1)setPixelBig(x,y);
    else setPixel(x,y);
}



void Rasterizer::DrawLine(int x1, int y1, int x2, int y2){

    int dx, dy, pk, decide = 0;
 
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
 
    if (dx < dy) {
        std::swap(dx,dy);
        std::swap(x1,y1);
        std::swap(x2,y2);
        decide = 1;
    }


    pk = 2 * dy - dx;
    for (int i = 0; i <= dx; i++) {
        x1 < x2 ? x1++ : x1--;
        if (pk < 0) {
            if (decide == 0) {
                 setPixel(x1, y1);
                pk = pk + 2 * dy;
            }
            else {
                setPixel(y1, x1);
                pk = pk + 2 * dy;
            }
        }
        else {
            y1 < y2 ? y1++ : y1--;
            if (decide == 0) {
 
                setPixel(x1, y1);
            }
            else {
                setPixel(y1, x1);
            }
            pk = pk + 2 * dy - 2 * dx;
        }
    }
    



}

void Rasterizer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    //use color buffer in Con->color_buffer as an output
}