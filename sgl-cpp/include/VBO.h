class VBO{
    std::vector<float> vertex_buffer;
    int currIndex = 0;

public:
    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);
    void ClearVBO();

};
VBO::VBO(){
}
VBO::~VBO(){}

void VBO::InsertVertex(float x, float y, float z, float w)
{
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


