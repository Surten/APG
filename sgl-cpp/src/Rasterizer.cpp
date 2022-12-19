#include "Rasterizer.h"
#include <cmath>
#include "Primitive.h"


Rasterizer::Rasterizer(Context* ContextActive){
    Con = ContextActive;
}

void Rasterizer::setPixel(SCVertex v){
    if(v.x < Con->frameWidth && v.x >= 0 && v.y < Con->frameHeight && v.y >= 0){
        if(Con->depthActive)
        {
            if(Con->depth_buffer[v.y*Con->frameWidth + v.x] > v.z)
            {
 /*                if(Con->depth_buffer[v.y*Con->frameWidth + v.x] != INFINITY){
                    std::cout << v.z << "     " << Con->depth_buffer[v.y*Con->frameWidth + v.x] << std::endl;
                    std::cout << Con->currentColor[0] << " " << Con->currentColor[1] << " " << Con->currentColor[2] << " " <<std::endl;
                } */
                Con->depth_buffer[v.y*Con->frameWidth + v.x] = v.z;
                FragmentShader(v);
            }
        }
        else{FragmentShader(v);}
    }

}

void Rasterizer::setPixelBig(SCVertex v){
    SCVertex point(v);

    for (int i = 0; i < Con->pointSize; i++)
    {
        for (int j = 0; j < Con->pointSize; j++)
        {
            point.x = v.x-((Con->pointSize-1)/2)+j;
            point.y = v.y-((Con->pointSize-1)/2)+i;
            setPixel(point);
        }
        
    }
    
}

void Rasterizer::DrawPoint(SCVertex v){
    //use color buffer in Con->color_buffer as an output
    //in the correct place put Con->currentColor
    //if(Con->depthActive && Con->depth_buffer[v.y*Con->frameWidth + v.x] > v.z){
        //Con->depth_buffer[v.y*Con->frameWidth + v.x] = v.z;
        if(Con->pointSize>1)setPixelBig(v);
        else setPixel(v);
    //}
}



void Rasterizer::DrawLine(SCVertex v1, SCVertex v2){

Bresenham3D(v1,v2);

}

// Bresenham3D
//
// A slightly modified version of the source found at
// http://www.ict.griffith.edu.au/anthony/info/graphics/bresenham.procs
// Provided by Anthony Thyssen, though he does not take credit for the original implementation
//
// It is highly likely that the original Author was Bob Pendelton, as referenced here
//
// ftp://ftp.isc.org/pub/usenet/comp.sources.unix/volume26/line3d
//
// line3d was dervied from DigitalLine.c published as "Digital Line Drawing"
// by Paul Heckbert from "Graphics Gems", Academic Press, 1990
//
// 3D modifications by Bob Pendleton. The original source code was in the public
// domain, the author of the 3D version places his modifications in the
// public domain as well.
//
// line3d uses Bresenham's algorithm to generate the 3 dimensional points on a
// line from (x1, y1, z1) to (x2, y2, z2)
//


void Rasterizer::Bresenham3D(SCVertex v1, const SCVertex v2){
    
    int i, dx, dy, dz, l, m, n, x_inc, y_inc, z_inc, err_1, err_2, dx2, dy2, dz2;
    SCVertex point(v1);

    dx = v2.x - v1.x;
    dy = v2.y - v1.y;
    dz = static_cast<int>(v2.z - v1.z);
    x_inc = (dx < 0) ? -1 : 1;
    l = abs(dx);
    y_inc = (dy < 0) ? -1 : 1;
    m = abs(dy);
    z_inc = (dz < 0) ? -1 : 1;
    n = abs(dz);
    dx2 = l << 1;
    dy2 = m << 1;
    dz2 = n << 1;
    
    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (i = 0; i < l; i++) {
            setPixel(point);
            if (err_1 > 0) {
                point.y += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point.z += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point.x += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (i = 0; i < m; i++) {
            setPixel(point);
            if (err_1 > 0) {
                point.x += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point.z += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point.y += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (i = 0; i < n; i++) {
            setPixel(point);
            if (err_1 > 0) {
                point.y += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point.x += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point.z += z_inc;
        }
    }
    setPixel(point);
}

void Rasterizer::DrawTriangle(SCVertex v1, SCVertex v2, SCVertex v3){
    //use color buffer in Con->color_buffer as an output
}



void SLFEdge::SetXIntersection(){
    xUp += xStep; 
    xIntersection = static_cast<int>(xUp);
    zUp += zStep;
    zIntersection = zUp;
    
}

SLFEdge::SLFEdge(float x1, float y1, float z1, float x2, float y2, float z2){
    if(y1 > y2){
        yUp = static_cast<int>(y1);
        xUp = x1;
        yLow = static_cast<int>(y2)+1;
        xStep = (x2 - x1) / static_cast<float>(y1 - y2+1.0f);
        xIntersection = static_cast<int>(x1-xStep);
        zUp = z1;
        zStep = (z2 - z1) / static_cast<float>(y1 - y2+1.0f);
        zIntersection = z1-zStep;
    }else{
        yUp = static_cast<int>(y2);
        xUp = x2;
        yLow = static_cast<int>(y1)+1;
        xStep = (x1 - x2) / static_cast<float>(y2 - y1+1.0f);
        xIntersection = static_cast<int>(x2-xStep);
        zUp = z2;
        zStep = (z1 - z2) / static_cast<float>(y2 - y1+1.0f);
        zIntersection = z2-zStep;
    }
}

bool predicate(SLFEdge e1, SLFEdge e2){return e1.yUp > e2.yUp;}

void ShakerSort(std::vector<SLFEdge> &edges, int start, int stop){
    int i, j, k;
    for (i = start; i < stop; i++)
    {
        for (j = i+1; j < stop; j++)
        {
            if(edges[j].xIntersection < edges[j-1].xIntersection){
                std::swap(edges[j], edges[j-1]);
            }
        }
        for (k = stop-1; k > i; k--)
        {
            if(edges[k].xIntersection < edges[k-1].xIntersection){
                std::swap(edges[k], edges[k-1]);
            }
        }
    }
}

void MySwap(SLFEdge &e1, SLFEdge &e2){
    SLFEdge e = e1;
    e1 = e2;
    e2 = e;
}

std::vector<SLFEdge> Rasterizer::CreateEdges(VBO &vbo, int &yMax, int &yMin){
    std::vector<SLFEdge> edges;

    //make edges from VBO
    for (int i = 0; i < static_cast<int>(vbo.GetSize())-1; i++)
    {
        
        if(static_cast<int>(vbo.vertex_buffer.at(i).y) != static_cast<int>(vbo.vertex_buffer.at(i+1).y)){
            SLFEdge e(vbo.vertex_buffer.at(i).x, vbo.vertex_buffer.at(i).y, vbo.vertex_buffer.at(i).z, vbo.vertex_buffer.at(i+1).x, vbo.vertex_buffer.at(i+1).y, vbo.vertex_buffer.at(i+1).z);
            edges.push_back(e);
            if(yMax < e.yUp) yMax = e.yUp;
            if(yMin > e.yLow) yMin = e.yLow;
        }
    }
    //add last edge to make full polygon
    if(static_cast<int>(vbo.vertex_buffer.at(vbo.GetSize()-1).y) != static_cast<int>(vbo.vertex_buffer.at(0).y)){
            SLFEdge e(vbo.vertex_buffer.at(vbo.GetSize()-1).x, vbo.vertex_buffer.at(vbo.GetSize()-1).y, vbo.vertex_buffer.at(vbo.GetSize()-1).z, vbo.vertex_buffer.at(0).x, vbo.vertex_buffer.at(0).y, vbo.vertex_buffer.at(0).z);
            edges.push_back(e);
            if(yMax < e.yUp) yMax = e.yUp;
            if(yMin > e.yLow) yMin = e.yLow;
    }

    return edges;
}
std::vector<SLFEdge> Rasterizer::CreateEdges(Vertex &v1, Vertex &v2, Vertex &v3, int &yMax, int &yMin){
    std::vector<SLFEdge> edges;

    if(static_cast<int>(v1.y) != static_cast<int>(v2.y)){
        SLFEdge e(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z);
        edges.push_back(e);
        if(yMax < e.yUp) yMax = e.yUp;
        if(yMin > e.yLow) yMin = e.yLow;
    }
    if(static_cast<int>(v2.y) != static_cast<int>(v3.y)){
        SLFEdge e(v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
        edges.push_back(e);
        if(yMax < e.yUp) yMax = e.yUp;
        if(yMin > e.yLow) yMin = e.yLow;
    }
    if(static_cast<int>(v3.y) != static_cast<int>(v1.y)){
        SLFEdge e(v3.x, v3.y, v3.z, v1.x, v1.y, v1.z);
        edges.push_back(e);
        if(yMax < e.yUp) yMax = e.yUp;
        if(yMin > e.yLow) yMin = e.yLow;
    }
    return edges;
}
std::vector<SLFEdge> Rasterizer::CreateEdges(std::vector<Vertex> &vec, int &yMax, int &yMin){
    std::vector<SLFEdge> edges;

    //make edges from VBO
    for (int i = 0; i < static_cast<int>(vec.size())-1; i++)
    {
        if(static_cast<int>(vec.at(i).y) != static_cast<int>(vec.at(i+1).y)){
            SLFEdge e(vec.at(i).x, vec.at(i).y, vec.at(i).z, vec.at(i+1).x, vec.at(i+1).y, vec.at(i+1).z);
            edges.push_back(e);
            if(yMax < e.yUp) yMax = e.yUp;
            if(yMin > e.yLow) yMin = e.yLow;
        }
    }
    //add last edge to make full polygon
    if(static_cast<int>(vec.at(vec.size()-1).y) != static_cast<int>(vec.at(0).y)){
            SLFEdge e(vec.at(vec.size()-1).x, vec.at(vec.size()-1).y, vec.at(vec.size()-1).z, vec.at(0).x, vec.at(0).y, vec.at(0).z);
            edges.push_back(e);
            if(yMax < e.yUp) yMax = e.yUp;
            if(yMin > e.yLow) yMin = e.yLow;
    }
    return edges;
}

void Rasterizer::ScanLineFill(std::vector<SLFEdge> &edges, int yMax, int yMin){
    int activeIndexEnd = 0;
    int activeIndexBegin = 0;
    int edgesSize;
    float z = 0, zStep = 0;

    
    //sort array by yUp
    edgesSize = static_cast<int>(edges.size());
    std::sort(edges.begin(), edges.end(), predicate);

    //for each y beetween top and bottom point of polygon
    for (int i = yMax; i >= yMin; i--)
    {
        //add next element to active array
        while (activeIndexEnd != edgesSize && i <= edges.at(activeIndexEnd).yUp)
        {
            activeIndexEnd++;
        }
        
        // remove inactive elements from active array
        for (int j = activeIndexBegin; j < activeIndexEnd; j++)
        {
            if(i < edges.at(j).yLow){
                MySwap(edges[activeIndexBegin], edges[j]);
                activeIndexBegin++;
            }else{
                //if not removing, calculate xIntersetion
                edges.at(j).SetXIntersection();
            }
        }
        //shake sort the active list
        ShakerSort(edges, activeIndexBegin, activeIndexEnd);

        //for each 2 edges do:
        for (int j = activeIndexBegin; j < activeIndexEnd; j+=2)
        {
            if(Con->depthActive){
                z = edges[j].zIntersection;
                zStep = (edges[j+1].zIntersection - edges[j].zIntersection) / (edges[j+1].xIntersection - edges[j].xIntersection + 1);
            }
            //for each x between the 2 edges : render
            for (int k = edges[j].xIntersection; k <= edges[j+1].xIntersection; k++)
            {
                //if(Con->depthActive && edges[j].zIntersection < edges[j+1].zIntersection && edges[j+1].zIntersection < z) std::cout << k << " " << z << std::endl;
                setPixel(SCVertex(k, i, z));
                z+=zStep;
            }
            
        }     

    }

}



void Rasterizer::vboToPrimitives(){
    std::vector<Vertex> &vbo = Con->vbo.vertex_buffer;
    size_t vboSize = Con->vbo.GetSize();
    if (vboSize < 3){
        return;
    }

    switch(Con->EleType){
        case SGL_POINTS:
        case SGL_LINES:
        case SGL_LINE_STRIP:
        case SGL_LINE_LOOP:
        case SGL_LAST_ELEMENT_TYPE:
        case SGL_AREA_LIGHT:
        default:
             // not supported
            break;
  
        case SGL_TRIANGLES:
            for (size_t i = 0; i < vboSize - 2; i++){
                TriangleP* tri = new TriangleP{vbo[i], vbo[i+1], vbo[i+2], Con->currentMaterial};
                //TriangleP* tri2 = new TriangleP{vbo[i], vbo[i+2], vbo[i+1], Con->currentMaterial};
                Con->primitiveList.push_back(tri);
                if (Con->currentMaterial.isEmissive){
                    Con->setPointlightsForEmissiveTriangle(*tri);
                }
                //Con->primitiveList.push_back(tri2);
            }
            break;
  
        case SGL_POLYGON:
            // Convert into triangle fan
            Vertex first = vbo[0];
            for (size_t i = 1; i < vboSize - 1; i++){
                Vertex second = vbo[i];
                Vertex third = vbo[i + 1];
                TriangleP* tri = new TriangleP{first, second, third, Con->currentMaterial};
                //TriangleP* tri2 = new TriangleP{first, third, second, Con->currentMaterial};
                Con->primitiveList.push_back(tri);
                if (Con->currentMaterial.isEmissive){
                    Con->setPointlightsForEmissiveTriangle(*tri);
                }
                //Con->primitiveList.push_back(tri2);
            }
            break;
    }

}



float clamp01(float num){
    return std::min(std::max(num, 0.0f), 1.0f);
}

void Rasterizer::FragmentShader(SCVertex &v){
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x)] = Con->currentColor[0];
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 1] = Con->currentColor[1];
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 2] = Con->currentColor[2];
}

void Rasterizer::FragmentShader(SCVertex &v, Color &c){
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x)] = c.r;
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 1] = c.g;
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 2] = c.b;
}
/*
void Rasterizer::FragmentShader(SCVertex &v, Vertex &position, Vertex &lookDirection, Vertex &normal, Material &mat){
    Color color{.0f, .0f, .0f};

    using std::max;
    using std::min;
    
    for (PointLight light : Con->pointLightList){
        // phong
        // ambient is neglected = 0

        // diffuse
        Vertex L = position - light.position;  // light direction
        L.normalize();
        float cosA = dot(L, normal);
        color += light.color * (mat.color * mat.kd) * max(cosA, 0.0f);
        // specular
        Vertex R = 2 * dot(L, normal) * normal - L; // reflected
        R.normalize();
        float cosB = dot(lookDirection, R);
        color += light.color  * mat.ks * powf(max(cosB, 0.0f), mat.shine);
    }
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x)    ] = color.r;
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 1] = color.g;
    Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 2] = color.b;
    // Con->color_buffer[3*(v.y*Con->frameWidth + v.x)    ] = mat.color.r;
    // Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 1] = mat.color.g;
    // Con->color_buffer[3*(v.y*Con->frameWidth + v.x) + 2] = mat.color.b;
}
*/