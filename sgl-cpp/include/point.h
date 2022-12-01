
#include <algorithm>
#include <cmath>
#include "Vertex.h"

class Point{

public:

    float x;
    float y;
    float z;
    Point(){x = y = z = 0.0f;}
    Point(float xx, float yy, float zz) : x(xx), y(yy), z(zz){}
    Point(Point &p){x=p.x; y=p.y, z=p.z;}
    Point(Vertex &v){x=v.x; y=v.y, z=v.z;}

    Point operator-(Point const &v){
        return Point(x - v.x, y - v.y, z - v.z);
    }

    Point operator+(Point const&v){
        
        return Point(x + v.x, y + v.y, z + v.z);
    }
    friend Point operator*(float c, Point& v);
    friend Point operator*(Point& v, float c);

    void normalize(){
        float len = length();
        x /= len;
        y /= len;
        z /= len;
    }

    float length(){
        return sqrtf(x * x + y * y + z * z);
    }
};


