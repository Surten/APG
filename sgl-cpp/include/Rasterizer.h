#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "context.h"

class Rasterizer{
public:
    Context* ConActive;
    void DrawPoint(int x, int y);
    void DrawLine(int x1, int y1, int x2, int y2);
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    Rasterizer(Context* ContextActive){ConActive = ContextActive;}
};


#endif