#ifndef _CONTEXT_H_
#define _CONTEXT_H_

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
	unsigned int colorBufferSize;
	float currentColor[3];
	float clearColor[3];
	sglEElementType EleType; 
	VBO vbo;
	sglEMatrixMode MatrixMode;
	bool depthActive;
	Viewport viewport;

	MatrixLinkedList modelViewStack;
	MatrixLinkedList projectionStack;

	Context(int idd, int width, int heigth);
	~Context();

	void MatrixMultVector(Matrix4f& m, float& x, float& y, float& z, float& w);

	void VertexShader(float& x, float& y, float& z, float& w);
	void PerspectiveDivision(float& x, float& y, float& z, float& w);
	void ViewPortTransform(float& x, float& y);


};


#endif
