#include "context.h"

Context::Context(int idd, int width, int heigth) : id(idd), frameWidth(width), frameHeight(heigth)
{
	colorBufferSize = heigth*width*3;
 	color_buffer = new float[colorBufferSize];
}

Context::~Context(){}


void Context::MatrixMultVector(Matrix4f& m, float& x, float& y, float& z, float& w){
	float xx,yy,zz,ww;
	xx = m.matrix[0]*x + m.matrix[1]*x + m.matrix[2]*x + m.matrix[3]*x;
	yy = m.matrix[4]*y + m.matrix[5]*y + m.matrix[6]*y + m.matrix[7]*y;
	zz = m.matrix[8]*z + m.matrix[9]*z + m.matrix[10]*z + m.matrix[11]*z;
	ww = m.matrix[12]*w + m.matrix[13]*w + m.matrix[14]*w + m.matrix[15]*w;
	x=xx;
	y=yy;
	z=zz;
	w=ww;
}


void Context::VertexShader(float& x, float& y, float& z, float& w){
	Matrix4f mat;
	mat = *projectionStack.top * *modelViewStack.top;
	
	MatrixMultVector(mat,x,y,z,w);
}

void Context::PerspectiveDivision(float& x, float& y, float& z, float& w){
	x = x / w;
	y = y / w;
	z = y / w;
}

void Context::ViewPortTransform(float& x, float& y){
	float xx, yy;
	xx = (x + 1) * viewport.width/2  + viewport.x;
  	yy = (y + 1) * viewport.height/2 + viewport.y;
	x=xx;
	y=yy;

}


