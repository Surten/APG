#ifndef _LIGHT_H
#define _LIGHT_H

#include "VBO.h"

struct Color{
    float r, g, b;

    Color(){
        r = g = b = 0;
    }

    Color(float r, float g, float b):
        r(r), g(g), b(b){}

    void operator+=(Color const& c){
        r += c.r;
        g += c.g;
        b += c.b;
    }

    Color operator+(Color const& c){
        return Color(r + c.r, g + c.g, b + c.b);
    }

    Color operator*(Color const& c){
        return Color(r * c.r, g * c.g, b * c.b);
    }

    Color operator*(float con){
        return Color(r * con, g * con, b * con);
    }
};

struct PointLight {
public:
    Vertex position;
    Color color;

    PointLight(Vertex &position, Color &color):
        position(position), color(color){}
};











#endif