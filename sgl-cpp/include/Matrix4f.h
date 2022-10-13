class Matrix4f{
    float matrix[16];

public:
    Matrix4f(float* mat);
    ~Matrix4f();
    void InsertNumber(int posX, int posY, float value);
    void InsertMainDiagonal(float a, float b, float c, float d);
    void InsertColumn(int column, float a, float b, float c, float d);
    void InsertRow(int row, float a, float b, float c, float d);
    void MultiplyBy(Matrix4f mat);
};

Matrix4f::Matrix4f(float* mat){
    memcpy(matrix,mat,sizeof(float)*16);
}
Matrix4f::~Matrix4f(){}

void Matrix4f::InsertNumber(int posX, int posY, float value)
{
    if(posX*posY<0 || posX*posY > 16){}
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
/*
void Matrix4f::MultiplyBy(Matrix4f mat){
    for (size_t i = 0; i < 4; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            float sum = 0;
            for (size_t k = 0; k < 4; k++)
            {
                sum += 
            }
            
        }
        
    }
    
}

*/