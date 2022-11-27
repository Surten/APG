#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <iostream>
#include "Vertex.h"
#include <algorithm>
#include <cmath>

/**
 * Holds Matrix 4x4 and its operations
*/
class Matrix4f{

public:
    float matrix[16];
    Matrix4f* next = nullptr;
    Matrix4f();
    ~Matrix4f();
    
    /**
     * Prints Matrix to standart output
     * Used for debugging purposes
    */
    void PrintMatrix();
    
    /**
     * Copies matrix from float[16]
    */
    void InsertMatrix(float m[]);
    Matrix4f operator* (const Matrix4f& m1);
    Matrix4f(const Matrix4f&);

    void MultiplyVector(Vertex &v);

    int invert();
    

};

class MatrixLinkedList{
public:
    Matrix4f* top;
    int size = 0;
    
    void Push(Matrix4f& m);
    void Pop();

    void MultiplyFromRight(Matrix4f mat);
    void MultiplyFromLeft(Matrix4f mat);


    MatrixLinkedList();
    ~MatrixLinkedList();

};


#endif