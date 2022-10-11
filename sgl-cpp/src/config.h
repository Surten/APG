#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>


///Pixel structure, float triplet r,g,b and it's index (after 2D->1D transformation)


///Context structure, context_index, color_buffer and vertex_buffer
class Context{
public:
	int id;
	float* color_buff;
	float* vertex_buff;
	int width;
	int height;
	float clearColor[4];
};


struct Manager{
	int currentContext = 0;
};
Manager manager;

std::vector<Context> ContextArray;

///Contexts structure, an array of context (max 32) and the current context index
/*
typedef struct {
	struct Context * contexts;
	int current_context;
}Contexts;
*/
//void set_color(Pixel p);


#endif