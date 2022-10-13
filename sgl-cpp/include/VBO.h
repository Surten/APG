class VBO{
    float* vertex_buffer;
    int currIndex = 0;
    int allocatedSize = 0;

public:
    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);
    void ClearVBO();

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
    if(allocatedSize <= currIndex+5){
        //reallocate to a higher size
    }
    vertex_buffer[currIndex++] = x;
    vertex_buffer[currIndex++] = y;
    vertex_buffer[currIndex++] = z;
    vertex_buffer[currIndex++] = w;
}

void VBO::ClearVBO(){
    currIndex = 0;
    //do I need to erase everything??
    // feels like I can just override it
}


