#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "context.h"



struct SCVertex{
    int x;
    int y;
    float z;

    SCVertex(){}
    SCVertex(SCVertex &v){
        x = v.x;
        y = v.y;
        z = v.z;
    }

    SCVertex(int xx, int yy, float zz) : x(xx), y(yy), z(zz){}

    SCVertex(Vertex v){
        x = static_cast<int>(std::round(v.x));
        y = static_cast<int>(std::round(v.y));
        z = v.z;
    }
};

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
    void DrawPoint(SCVertex v);

    /**
     * Insert line beetween 2 points (x1,y1) and (x2,y2) into the frame buffer using Bresenham algorithm
    */
    void DrawLine(SCVertex v1, SCVertex v2);

    /**
     * Does nothing yet
     * TODO
    */
    void DrawTriangle(SCVertex v1, SCVertex v2, SCVertex v3);

    /**
     * Inserts pixels at (x,y) and around in an area pointSize by pointSize
    */
    void setPixelBig(SCVertex v);

    /**
     * Insert point in the frame buffer using the coordinates
    */
    void setPixel(SCVertex &v);

    void Bresenham3D(SCVertex v1, const SCVertex v2);

    void FragmentShader(SCVertex &v);

    void ScanLineFill(VBO &vbo);
    
    Context* Con;
    Rasterizer(Context* ContextActive);
    
};


#endif