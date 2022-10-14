struct Viewport{
    int x;
    int y;
    int width;
    int height;
};



class VBO{

    size_t currIndex = 0;

public:
    std::vector<float> vertex_buffer;

    VBO();
    ~VBO();
    void InsertVertex(float x, float y, float z, float w);
    void ClearVBO();
    size_t GetSize();

};
VBO::VBO(){
}
VBO::~VBO(){}

void VBO::InsertVertex(float x, float y, float z, float w)
{
    vertex_buffer.push_back(x);
    vertex_buffer.push_back(y);
    vertex_buffer.push_back(z);
    vertex_buffer.push_back(w);
    currIndex += 4;

}

void VBO::ClearVBO(){
    currIndex = 0;
    //do I need to erase everything??
    // feels like I can just override it
}

size_t VBO::GetSize(){
    return currIndex;
}


