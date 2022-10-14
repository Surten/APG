#include "Rasterizer.h"


Rasterizer::Rasterizer(Context* ContextActive){
    Con = ContextActive;
}

void Rasterizer::setPixel(int x, int y){
    
    if(x < Con->frameWidth && x >= 0 && y < Con->frameHeight && y >= 0){

        //std::cout << "GOODBYE" << std::endl;
        Con->color_buffer[3*(y*Con->frameWidth + x)] = Con->currentColor[0];
        Con->color_buffer[3*(y*Con->frameWidth + x) + 1] = Con->currentColor[1];
        Con->color_buffer[3*(y*Con->frameWidth + x) + 2] = Con->currentColor[2];
    }
}

void Rasterizer::DrawPoint(int x, int y){
    //use color buffer in Con->color_buffer as an output
    //in the correct place put Con->currentColor
    setPixel(x,y);
}

void Rasterizer::DrawLine(int x1, int y1, int x2, int y2){
    
    int c0, c1, p;
    c0 = 2 * (y2 - y1);
    c1 = c0 - 2 * (x2 - x1);
    p = c0 - (x2 - x1);
    setPixel (x1, y1);
    for (int i = x1 + 1; i <= x2; i++) {
        if (p < 0) {
            p += c0;
        } else {
            p += c1;
            y1++;
        }
        setPixel (i, y1);
    }

}

void Rasterizer::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    //use color buffer in Con->color_buffer as an output
}