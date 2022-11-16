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
    {
    ret.matrix[0] = matrix[0]*m1.matrix[0] + matrix[1]*m1.matrix[4] + matrix[2]*m1.matrix[8] + matrix[3]*m1.matrix[12];
    ret.matrix[1] = matrix[0]*m1.matrix[1] + matrix[1]*m1.matrix[5] + matrix[2]*m1.matrix[9] + matrix[3]*m1.matrix[13];
    ret.matrix[2] = matrix[0]*m1.matrix[2] + matrix[1]*m1.matrix[6] + matrix[2]*m1.matrix[10] + matrix[3]*m1.matrix[14];
    ret.matrix[3] = matrix[0]*m1.matrix[3] + matrix[1]*m1.matrix[7] + matrix[2]*m1.matrix[11] + matrix[3]*m1.matrix[15];

    ret.matrix[4] = matrix[4]*m1.matrix[0] + matrix[5]*m1.matrix[4] + matrix[6]*m1.matrix[8] + matrix[7]*m1.matrix[12];
    ret.matrix[5] = matrix[4]*m1.matrix[1] + matrix[5]*m1.matrix[5] + matrix[6]*m1.matrix[9] + matrix[7]*m1.matrix[13];
    ret.matrix[6] = matrix[4]*m1.matrix[2] + matrix[5]*m1.matrix[6] + matrix[6]*m1.matrix[10] + matrix[7]*m1.matrix[14];
    ret.matrix[7] = matrix[4]*m1.matrix[3] + matrix[5]*m1.matrix[7] + matrix[6]*m1.matrix[11] + matrix[7]*m1.matrix[15];


    ret.matrix[8] = matrix[8]*m1.matrix[0] + matrix[9]*m1.matrix[4] + matrix[10]*m1.matrix[8] + matrix[11]*m1.matrix[12];
    ret.matrix[9] = matrix[8]*m1.matrix[1] + matrix[9]*m1.matrix[5] + matrix[10]*m1.matrix[9] + matrix[11]*m1.matrix[13];
    ret.matrix[10] = matrix[8]*m1.matrix[2] + matrix[9]*m1.matrix[6] + matrix[10]*m1.matrix[10] + matrix[11]*m1.matrix[14];
    ret.matrix[11] = matrix[8]*m1.matrix[3] + matrix[9]*m1.matrix[7] + matrix[10]*m1.matrix[11] + matrix[11]*m1.matrix[15];


    ret.matrix[12] = matrix[12]*m1.matrix[0] + matrix[13]*m1.matrix[4] + matrix[14]*m1.matrix[8] + matrix[15]*m1.matrix[12];
    ret.matrix[13] = matrix[12]*m1.matrix[1] + matrix[13]*m1.matrix[5] + matrix[14]*m1.matrix[9] + matrix[15]*m1.matrix[13];
    ret.matrix[14] = matrix[12]*m1.matrix[2] + matrix[13]*m1.matrix[6] + matrix[14]*m1.matrix[10] + matrix[15]*m1.matrix[14];
    ret.matrix[15] = matrix[12]*m1.matrix[3] + matrix[13]*m1.matrix[7] + matrix[14]*m1.matrix[11] + matrix[15]*m1.matrix[15];
    }
    return ret;
}


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



MatrixLinkedList::MatrixLinkedList(){
    Matrix4f *m = new Matrix4f();
    top = m;

    size = 1;
}

MatrixLinkedList::~MatrixLinkedList(){
    while(size != 0)Pop();
}



void MatrixLinkedList::MultiplyFromRight(Matrix4f m2){
    float m1[16];
    std::copy(top->matrix,top->matrix+16,m1);
    {
    top->matrix[0] = m1[0]*m2.matrix[0] + m1[1]*m2.matrix[4] + m1[2]*m2.matrix[8] + m1[3]*m2.matrix[12];
    top->matrix[1] = m1[0]*m2.matrix[1] + m1[1]*m2.matrix[5] + m1[2]*m2.matrix[9] + m1[3]*m2.matrix[13];
    top->matrix[2] = m1[0]*m2.matrix[2] + m1[1]*m2.matrix[6] + m1[2]*m2.matrix[10] + m1[3]*m2.matrix[14];
    top->matrix[3] = m1[0]*m2.matrix[3] + m1[1]*m2.matrix[7] + m1[2]*m2.matrix[11] + m1[3]*m2.matrix[15];

    top->matrix[4] = m1[4]*m2.matrix[0] + m1[5]*m2.matrix[4] + m1[6]*m2.matrix[8] + m1[7]*m2.matrix[12];
    top->matrix[5] = m1[4]*m2.matrix[1] + m1[5]*m2.matrix[5] + m1[6]*m2.matrix[9] + m1[7]*m2.matrix[13];
    top->matrix[6] = m1[4]*m2.matrix[2] + m1[5]*m2.matrix[6] + m1[6]*m2.matrix[10] + m1[7]*m2.matrix[14];
    top->matrix[7] = m1[4]*m2.matrix[3] + m1[5]*m2.matrix[7] + m1[6]*m2.matrix[11] + m1[7]*m2.matrix[15];


    top->matrix[8] = m1[8]*m2.matrix[0] + m1[9]*m2.matrix[4] + m1[10]*m2.matrix[8] + m1[11]*m2.matrix[12];
    top->matrix[9] = m1[8]*m2.matrix[1] + m1[9]*m2.matrix[5] + m1[10]*m2.matrix[9] + m1[11]*m2.matrix[13];
    top->matrix[10] = m1[8]*m2.matrix[2] + m1[9]*m2.matrix[6] + m1[10]*m2.matrix[10] + m1[11]*m2.matrix[14];
    top->matrix[11] = m1[8]*m2.matrix[3] + m1[9]*m2.matrix[7] + m1[10]*m2.matrix[11] + m1[11]*m2.matrix[15];


    top->matrix[12] = m1[12]*m2.matrix[0] + m1[13]*m2.matrix[4] + m1[14]*m2.matrix[8] + m1[15]*m2.matrix[12];
    top->matrix[13] = m1[12]*m2.matrix[1] + m1[13]*m2.matrix[5] + m1[14]*m2.matrix[9] + m1[15]*m2.matrix[13];
    top->matrix[14] = m1[12]*m2.matrix[2] + m1[13]*m2.matrix[6] + m1[14]*m2.matrix[10] + m1[15]*m2.matrix[14];
    top->matrix[15] = m1[12]*m2.matrix[3] + m1[13]*m2.matrix[7] + m1[14]*m2.matrix[11] + m1[15]*m2.matrix[15];
    }
}

void MatrixLinkedList::MultiplyFromLeft(Matrix4f m1){
    float m2[16];
    std::copy(top->matrix,top->matrix+16,m2);
{
    top->matrix[0] = m1.matrix[0]*m2[0] + m1.matrix[1]*m2[4] + m1.matrix[2]*m2[8] + m1.matrix[3]*m2[12];
    top->matrix[1] = m1.matrix[0]*m2[1] + m1.matrix[1]*m2[5] + m1.matrix[2]*m2[9] + m1.matrix[3]*m2[13];
    top->matrix[2] = m1.matrix[0]*m2[2] + m1.matrix[1]*m2[6] + m1.matrix[2]*m2[10] + m1.matrix[3]*m2[14];
    top->matrix[3] = m1.matrix[0]*m2[3] + m1.matrix[1]*m2[7] + m1.matrix[2]*m2[11] + m1.matrix[3]*m2[15];

    top->matrix[4] = m1.matrix[4]*m2[0] + m1.matrix[5]*m2[4] + m1.matrix[6]*m2[8] + m1.matrix[7]*m2[12];
    top->matrix[5] = m1.matrix[4]*m2[1] + m1.matrix[5]*m2[5] + m1.matrix[6]*m2[9] + m1.matrix[7]*m2[13];
    top->matrix[6] = m1.matrix[4]*m2[2] + m1.matrix[5]*m2[6] + m1.matrix[6]*m2[10] + m1.matrix[7]*m2[14];
    top->matrix[7] = m1.matrix[4]*m2[3] + m1.matrix[5]*m2[7] + m1.matrix[6]*m2[11] + m1.matrix[7]*m2[15];


    top->matrix[8] = m1.matrix[8]*m2[0] + m1.matrix[9]*m2[4] + m1.matrix[10]*m2[8] + m1.matrix[11]*m2[12];
    top->matrix[9] = m1.matrix[8]*m2[1] + m1.matrix[9]*m2[5] + m1.matrix[10]*m2[9] + m1.matrix[11]*m2[13];
    top->matrix[10] = m1.matrix[8]*m2[2] + m1.matrix[9]*m2[6] + m1.matrix[10]*m2[10] + m1.matrix[11]*m2[14];
    top->matrix[11] = m1.matrix[8]*m2[3] + m1.matrix[9]*m2[7] + m1.matrix[10]*m2[11] + m1.matrix[11]*m2[15];


    top->matrix[12] = m1.matrix[12]*m2[0] + m1.matrix[13]*m2[4] + m1.matrix[14]*m2[8] + m1.matrix[15]*m2[12];
    top->matrix[13] = m1.matrix[12]*m2[1] + m1.matrix[13]*m2[5] + m1.matrix[14]*m2[9] + m1.matrix[15]*m2[13];
    top->matrix[14] = m1.matrix[12]*m2[2] + m1.matrix[13]*m2[6] + m1.matrix[14]*m2[10] + m1.matrix[15]*m2[14];
    top->matrix[15] = m1.matrix[12]*m2[3] + m1.matrix[13]*m2[7] + m1.matrix[14]*m2[11] + m1.matrix[15]*m2[15];
}


}


// inverse matrix computation using gauss_jacobi method 
  // source: N.R. in C
  // if matrix is regular = computatation successfull = returns 0
  // in case of singular matrix returns 1
int Matrix4f::invert()
{
    int indxc[4],indxr[4],ipiv[4];
    int i,icol,irow,j,k,l,ll,n;
    float big,dum,pivinv,temp;
    // satisfy the compiler
    icol = irow = 0;

    float x[4][4];
    for (int _r = 0; _r < 4; _r++){
        for (int _c = 0; _c < 4; _c++){
            x[_c][_r] = matrix[_r * 4 + _c];
        }
    }
        
    // the size of the matrix
    n = 4;
        
    for ( j = 0 ; j < n ; j++) /* zero pivots */
        ipiv[j] = 0;
        
    for ( i = 0; i < n; i++)
        {
        big = 0.0;
        for (j = 0 ; j < n ; j++)
            if (ipiv[j] != 1)
            for ( k = 0 ; k<n ; k++)
                {
                if (ipiv[k] == 0)
                    {
                    if (fabs(x[k][j]) >= big)
                        {
                        big = fabs(x[k][j]);
                        irow = j;
                        icol = k;
                        }
                    }
                else
                    if (ipiv[k] > 1)
                    return 1; /* singular matrix */
                }
        ++(ipiv[icol]);
        if (irow != icol)
            {
            for ( l = 0 ; l<n ; l++)
                {
                temp = x[l][icol];
                x[l][icol] = x[l][irow];
                x[l][irow] = temp;
                }
            }
        indxr[i] = irow;
        indxc[i] = icol;
        if (x[icol][icol] == 0.0)
            return 1; /* singular matrix */
                
        pivinv = 1.0 / x[icol][icol];
        x[icol][icol] = 1.0 ;
        for ( l = 0 ; l<n ; l++)
            x[l][icol] = x[l][icol] * pivinv ;
                
        for (ll = 0 ; ll < n ; ll++)
            if (ll != icol)
            {
                dum = x[icol][ll];
                x[icol][ll] = 0.0;
                for ( l = 0 ; l<n ; l++)
                x[l][ll] = x[l][ll] - x[l][icol] * dum ;
            }
        }
    for ( l = n; l--; )
        {
        if (indxr[l] != indxc[l])
            for ( k = 0; k<n ; k++)
            {
                temp = x[indxr[l]][k];
                x[indxr[l]][k] = x[indxc[l]][k];
                x[indxc[l]][k] = temp;
            }
        }


    for (int _r = 0; _r < 4; _r++){
        for (int _c = 0; _c < 4; _c++){
            matrix[_r * 4 + _c] = x[_c][_r];
        }
    }
        
    return 0 ; // matrix is regular .. inversion has been succesfull
}


void MatrixLinkedList::Push(Matrix4f& m){
    Matrix4f *mat = new Matrix4f(m);
    mat->next = top;
    top = mat;
    size++;
}

void MatrixLinkedList::Pop(){
    if(top == nullptr) return;
    Matrix4f* m = top;
    if(top->next != nullptr){
        top = top->next;
    }
    delete m;
    size--;
}