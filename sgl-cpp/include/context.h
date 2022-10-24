#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <vector>
#include "sgl.h"
#include "VBO.h"
#include "Matrix4f.h"

/**
 * Class containing all info about one drawing
*/
class Context{
public:
	int id;
	float* color_buffer;
	float* depth_buffer;
	int frameWidth;
	int frameHeight;
	unsigned int colorBufferSize;
	float currentColor[3];
	float currentDepth;
	float clearColor[3];
	float clearDepth;
	sglEElementType EleType;
	VBO vbo;
	sglEMatrixMode MatrixMode;
	bool depthActive;
	Viewport viewport;
	int pointSize = 1;

	MatrixLinkedList modelViewStack;
	MatrixLinkedList projectionStack;

	Context(int idd, int width, int heigth);
	~Context();

	/**
	 * Multiplies Vector by Matrix
	 * Vector is on the right of the multiplication sign
	*/
	void MatrixMultVector(Matrix4f& m, float& x, float& y, float& z, float& w);


	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void VertexShader(float& x, float& y, float& z, float& w);

	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void PerspectiveDivision(float& x, float& y, float& z, float& w);
	
	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void ViewPortTransform(float& x, float& y);


};


#endif
