class Matrix4f{

public:
    float matrix[16];
    Matrix4f* next;
    Matrix4f* previous;
    Matrix4f();
    ~Matrix4f();
    void InsertMatrix(float* m);
    void InsertNumber(int posX, int posY, float value);
    void InsertMainDiagonal(float a, float b, float c, float d);
    void InsertColumn(int column, float a, float b, float c, float d);
    void InsertRow(int row, float a, float b, float c, float d);


};

class MatrixLinkedList{
public:
    Matrix4f* top;
    int size = 0;
    
    void Push(Matrix4f m);
    void Pop();

    void MultiplyFromRight(Matrix4f mat);
    void MultiplyFromLeft(Matrix4f mat);

    MatrixLinkedList();
    ~MatrixLinkedList();

};

MatrixLinkedList::MatrixLinkedList(){
    Matrix4f mat;
    top = &mat;
}




Matrix4f::Matrix4f(){
    float mat[16] = 
             {1,0,0,0,
              0,1,0,0,
              0,0,1,0,
              0,0,0,1};
    memcpy(matrix,mat,sizeof(float)*16);
}
Matrix4f::~Matrix4f(){}

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






void MatrixLinkedList::MultiplyFromRight(Matrix4f m1){
    float m2[16];
    memcpy(m2,top->matrix,sizeof(float)*16);
    float sum;
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            sum = 0;
            for (size_t k = 0; k < 4; k++)
            {
                sum += m1.matrix[i+k*4] *m2[j*4+k];
            }
            top->matrix[i*4+j] = sum;
        }   
    }
}

void MatrixLinkedList::MultiplyFromLeft(Matrix4f m1){
    float m2[16];
    memcpy(m2,top->matrix,sizeof(float)*16);
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



void MatrixLinkedList::Push(Matrix4f m){
    top->previous = &m;
    m.next = top;
    top = &m;
    size++;
}

void MatrixLinkedList::Pop(){
    top = top->next;
    size--;
}