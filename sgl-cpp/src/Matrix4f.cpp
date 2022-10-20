#include "Matrix4f.h"
#include <algorithm>



Matrix4f::Matrix4f(){
    float mat[16] = 
             {1,0,0,0,
              0,1,0,0,
              0,0,1,0,
              0,0,0,1};
    std::copy(mat,mat+16,matrix);
}
Matrix4f::~Matrix4f(){}

Matrix4f Matrix4f::operator* (const Matrix4f& m1){
    Matrix4f ret;
    float sum;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
           sum = 0;
           for (size_t k = 0; k < 4; k++)
            {
                sum += this->matrix[i*4+k] * m1.matrix[j+k*4];
            }
            ret.matrix[i*4+j] = sum;
        }
    }
    return ret;
}
/*
Matrix4f Matrix4f::operator= (const Matrix4f& m1){
    Matrix4f m;
    std::copy(m1.matrix, m1.matrix+16, m.matrix);
    return m;
}
*/

void Matrix4f::PrintMatrix(){
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            std::cout << matrix[i*4+j] << " ";
        }
        std::cout << std::endl;
    }
    
}

Matrix4f::Matrix4f(const Matrix4f& m1){
    std::copy(m1.matrix,m1.matrix+16,matrix);
}




void Matrix4f::InsertMatrix(float* m){
    std::copy(m, m+16, matrix);
}


void Matrix4f::InsertNumber(int posX, int posY, float value)
{
    matrix[posX + posY*4] = value;
}

void Matrix4f::InsertMainDiagonal(float a, float b, float c, float d)
{
    matrix[0] = a;
    matrix[5] = b;
    matrix[10] = c;
    matrix[15] = d;
}

void Matrix4f::InsertColumn(int column, float a, float b, float c, float d)
{
    matrix[column] = a;
    matrix[column+4] = b;
    matrix[column+8] = c;
    matrix[column+12] = d;

}

void Matrix4f::InsertRow(int row, float a, float b, float c, float d)
{
    matrix[row*4] = a;
    matrix[row*4 +1] = b;
    matrix[row*4 +2] = c;
    matrix[row*4 +3] = d;
}





MatrixLinkedList::MatrixLinkedList(){
    Matrix4f *m = new Matrix4f();
    top = m;

    size = 1;
}

MatrixLinkedList::~MatrixLinkedList(){
    while (top->next != nullptr)
    {
        Matrix4f * t = top;
        top = top->next;
        delete t;
    }
    delete top;
    
}



void MatrixLinkedList::MultiplyFromRight(Matrix4f m1){
    float m2[16];
    std::copy(top->matrix,top->matrix+16,m2);
    float sum;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            sum = 0;
            for (size_t k = 0; k < 4; k++)
            {
                sum += m2[i*4+k] * m1.matrix[j+k*4];
            }
            top->matrix[i*4+j] = sum;
        }   
    }
}

void MatrixLinkedList::MultiplyFromLeft(Matrix4f m1){
    float m2[16];
    std::copy(top->matrix,top->matrix+16,m2);
    float sum;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            sum = 0;
            for (size_t k = 0; k < 4; k++)
            {
                sum += m1.matrix[i*4+k] *m2[j+k*4];
            }
            top->matrix[i*4+j] = sum;
        }   
    }
}



void MatrixLinkedList::Push(Matrix4f& m){
    Matrix4f *mat = new Matrix4f(m);
    mat->next = top;
    top = mat;
    size++;
}

void MatrixLinkedList::Pop(){
    if(top->next == nullptr) return;
    Matrix4f* m = top;
    top = top->next;
    delete m;
    size--;
}