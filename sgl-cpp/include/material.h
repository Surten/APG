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

    bool isEmissive = false;
    float c0, c1, c2;

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
        isEmissive = m.isEmissive;
        c0 = m.c0;
        c1 = m.c1;
        c2 = m.c2;
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

    void setEmissiveProperties(float r, float g,  float b, float c0, float c1, float c2){
        color.r = r;
        color.g = g;
        color.b = b;
        this->c0 = c0;
        this->c1 = c1;
        this->c2 = c2;
    }
};

#endif