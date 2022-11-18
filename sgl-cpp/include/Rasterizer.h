#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include "context.h"
#include "pointLight.h"


/**
 * Edge structure for the scanline fill as described in the lecture
*/
struct SLFEdge{
    int yUp;
    int yLow;
    int xIntersection;
    float xUp;
    float xStep;
    float zIntersection;
    float zUp;
    float zStep;

    void SetXIntersection();



    SLFEdge(float x1, float y1, float z1, float x2, float y2, float z2);
};

/**
 * Screen space coordinates vertex... ususally used right before rasterization
*/
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
        x = static_cast<int>(v.x+0.5f);
        y = static_cast<int>(v.y+0.5f);
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
    void setPixel(SCVertex v);

    /**
     * the line drawing
     * TODO : make my own
    */
    void Bresenham3D(SCVertex v1, const SCVertex v2);

    void FragmentShader(SCVertex &v);
    void Rasterizer::FragmentShader(SCVertex &v, Vertex &position, Vertex &lookDirection, Vertex &normal, Material &mat);


    /**
     * Creates edge structure for scanline algorithm
    */
    std::vector<SLFEdge> CreateEdges(VBO &vbo, int &yMax, int &yMin);
    std::vector<SLFEdge> CreateEdges(Vertex &v1, Vertex &v2, Vertex &v3, int &yMax, int &yMin);
    std::vector<SLFEdge> CreateEdges(std::vector<Vertex> &vec, int &yMax, int &yMin);

    /**
     * Filling algorithm
    */
    void ScanLineFill(std::vector<SLFEdge> &edges, int yMax, int yMin);
    
    //active context of the current scene
    Context* Con;
    Rasterizer(Context* ContextActive);
    
};


#endif