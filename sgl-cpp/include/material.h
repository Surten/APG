#ifndef _MATERIAL_H
#define _MATERIAL_H

#include "pointLight.h"

class Material{
public:
    Color color;
    float kd, ks;
    float shine;
    float T;
    float ior;

    Material(){
        color.r = color.g = color.b = kd = ks = shine = T = ior = 0;
    }

    Material(float r, float g,  float b, float kd, float ks, float shine, float T, float ior):
        kd(kd), ks(ks), shine(shine), T(T), ior(ior){
            color.r = r;
            color.g = g;
            color.b = b;
        }

    Material(Color &col, float kd, float ks, float shine, float T, float ior):
        kd(kd), ks(ks), shine(shine), T(T), ior(ior){
            color.r = col.r;
            color.g = col.g;
            color.b = col.b;
        }

    Material (Material const &m){
        color.r = m.color.r;
        color.g = m.color.g;
        color.b = m.color.b;
        kd = m.kd;
        ks = m.ks;
        shine = m.shine;
        T = m.T;
        ior = m.ior;
    }

    void setProperties(float r, float g,  float b, float kd, float ks, float shine, float T, float ior){
        color.r = r;
        color.g = g;
        color.b = b;
        this->kd = kd;
        this->ks = ks;
        this->shine = shine;
        this->T = T;
        this->ior = ior;
    }
};

#endif