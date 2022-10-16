#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <iostream>
class Matrix4f{

public:
    float matrix[16];
    Matrix4f* next = nullptr;
    Matrix4f();
    ~Matrix4f();
    
    void InsertNumber(int posX, int posY, float value);
    void InsertMainDiagonal(float a, float b, float c, float d);
    void InsertColumn(int column, float a, float b, float c, float d);
    void InsertRow(int row, float a, float b, float c, float d);
    void PrintMatrix();
    
    void InsertMatrix(float m[]);
    Matrix4f Matrix4f::operator* (const Matrix4f& m1);
    //Matrix4f Matrix4f::operator= (const Matrix4f& m1);
    Matrix4f(const Matrix4f&);
    

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