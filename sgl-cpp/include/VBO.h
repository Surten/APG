class VBO{
    float* vertex_buffer;
    int currIndex = 0;
    int allocatedSize = 0;

public:
    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);


};
VBO::VBO(){
    allocatedSize = 1000;
    vertex_buffer = new float[allocatedSize];
}
VBO::~VBO(){
    delete vertex_buffer;
}

void VBO::InsertVertex(float x, float y, float z, float w)
{
    vertex_buffer
}



