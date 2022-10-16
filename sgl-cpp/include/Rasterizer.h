#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "context.h"

class Rasterizer{
public:
    void DrawPoint(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    void setPixelBig(int x, int y);
    void setPixel(int x, int y);
    
    Context* Con;
    Rasterizer(Context* ContextActive);
    
};


#endif