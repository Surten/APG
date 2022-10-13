#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <vector>
#include "sgl.h"
#include "VBO.h"
#include "Matrix4f.h"


///Pixel structure, float triplet r,g,b and it's index (after 2D->1D transformation)


///Context structure, context_index, color_buffer and vertex_buffer
class Context{
public:
	int id;
	float* color_buffer;
	int frameWidth;
	int frameHeight;
	float currentColor[4];
	float clearColor[4];
	sglEElementType EleType; 
	VBO vbo;
	sglEMatrixMode MatrixMode;
	sglEEnableFlags flags;

	//Matrix4f * modelViewStack;
	//Matrix4f * projectionStack;

	//void MatrixPush();
	//void MatrixPop();

	Context();
	~Context();
};


Context* ConActive = nullptr;
std::vector<Context> ContextArray;


#endif