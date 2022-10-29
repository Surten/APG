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
	unsigned int depthBufferSize;
	float currentColor[3];
	float clearColor[3];
	sglEElementType EleType;
	VBO vbo;
	sglEMatrixMode MatrixMode;
	bool depthActive = false;
	Viewport viewport;
	int pointSize = 1;
	sglEAreaMode areaMode = SGL_FILL;

	MatrixLinkedList modelViewStack;
	MatrixLinkedList projectionStack;

	Context(int idd, int width, int heigth);
	~Context();

	/**
	 * Multiplies Vector by Matrix
	 * Vector is on the right of the multiplication sign
	*/

	Matrix4f GetPVMMatrix();

	void MatrixMultVector(Matrix4f& m, Vertex &v);


	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void VertexShader(Matrix4f& pvm, Vertex &v);

	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void PerspectiveDivision(Vertex &v);
	
	/**
	 * Part of transformation matrix
	 * TODO merge all transforms together
	*/
	void ViewPortTransform(Vertex &v);


};


#endif
