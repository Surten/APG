#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>

/// Variables
int new_index=0;
int current_color[3];

struct Contexts C;


///Pixel structure, float triplet r,g,b and it's index (after 2D->1D transformation)
typedef struct {
	float r;
	float g;
	float b;
}Pixel;



///Context structure, context_index, color_buffer and vertex_buffer
class Context{
	int context_index;
	ColorBuffer color_buff;
	Vertex_buffer vertex_buff;
	int width;
	int height;
	std::vector<Pixel> pixel;
};

///Contexts structure, an array of context (max 32) and the current context index
typedef struct {
	struct Context * contexts;
	int current_context;
}Contexts;

void set_color(Pixel p);


#endif