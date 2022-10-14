#include "context.h"

class Rasterizer{
public:
    void DrawPoint(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);
};

void Rasterizer::DrawPoint(int x, int y){
    //use color buffer in ConActive->color_buffer as an output
    //in the correct place put ConActive->currentColor
}

void DrawLine(int x1, int y1, int x2, int y2){
    //use color buffer in ConActive->color_buffer as an output
    //in the correct place put ConActive->currentColor
}

void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3){
    //use color buffer in ConActive->color_buffer as an output
}
