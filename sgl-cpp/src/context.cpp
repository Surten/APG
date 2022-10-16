#include "context.h"

Context::Context(int idd, int width, int heigth) : id(idd), frameWidth(width), frameHeight(heigth)
{
	colorBufferSize = heigth*width*3;
 	color_buffer = new float[colorBufferSize];
}

Context::~Context(){}


void Context::MatrixMultVector(Matrix4f& m, float& x, float& y, float& z, float& w){

	float xx,yy,zz,ww;
	xx = m.matrix[0]*x + m.matrix[1]*y + m.matrix[2]*z + m.matrix[3]*w;
	yy = m.matrix[4]*x + m.matrix[5]*y + m.matrix[6]*z + m.matrix[7]*w;
	zz = m.matrix[8]*x + m.matrix[9]*y + m.matrix[10]*z + m.matrix[11]*w;
	ww = m.matrix[12]*x + m.matrix[13]*y + m.matrix[14]*z + m.matrix[15]*w;
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
	z = z / w;
}

void Context::ViewPortTransform(float& x, float& y){
	float xx, yy;
	xx = (x + 1) * viewport.width/2  + viewport.x;
  	yy = (y + 1) * viewport.height/2 + viewport.y;
	x=xx;
	y=yy;

}


