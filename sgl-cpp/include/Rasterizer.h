#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "context.h"


/**
 * Responsible for drawing into the frame buffer
 * Uses Verticies in screen coordinates
 * TODO make a separate fragment shader
*/
class Rasterizer{
public:
    /**
     * Insert point in the frame buffer using the coordinates
    */
    void DrawPoint(int x, int y);

    /**
     * Insert line beetween 2 points (x1,y1) and (x2,y2) into the frame buffer using Bresenham algorithm
    */
    void DrawLine(int x1, int y1, int x2, int y2);

    /**
     * Does nothing yet
     * TODO
    */
    void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3);

    /**
     * Inserts pixels at (x,y) and around in an area pointSize by pointSize
    */
    void setPixelBig(int x, int y);

    /**
     * Insert point in the frame buffer using the coordinates
    */
    void setPixel(int x, int y);
    
    Context* Con;
    Rasterizer(Context* ContextActive);
    
};


#endif