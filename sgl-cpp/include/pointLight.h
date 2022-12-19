#ifndef _LIGHT_H
#define _LIGHT_H

#include "VBO.h"

#define AREA_LIGHT_NUM_SAMPLES 16

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

    // used for area lights
    bool useAttenuation = false;
    float c0, c1, c2; // constant, linear, quadratic attenuation coeficients
    float triangleArea;
    Vertex triangleNormal;

    PointLight(Vertex &position, Color &color):
        position(position), color(color){}

    PointLight(Vertex &position, Color &color, float c0, float c1, float c2, float triangleArea, Vertex triangleNormal):
        position(position), color(color),
        c0(c0), c1(c1), c2(c2),
        triangleArea(triangleArea), triangleNormal(triangleNormal){
            useAttenuation = true;
        }
};











#endif