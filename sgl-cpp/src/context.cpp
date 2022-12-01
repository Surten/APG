#include "context.h"

Context::Context(int idd, int width, int heigth) : id(idd), frameWidth(width), frameHeight(heigth)
{
	colorBufferSize = heigth*width*3;
 	color_buffer = new float[colorBufferSize];
 	depthBufferSize = heigth*width;
 	depth_buffer = new float[depthBufferSize];
	for (size_t i = 0; i < depthBufferSize; i++)
    {
      depth_buffer[i] = 1;
    }
}

Context::~Context(){
	delete[] color_buffer;
	delete[] depth_buffer;
	discardPrimitives();
}

Matrix4f Context::GetPVMMatrix(){
	Matrix4f pvm = *projectionStack.top * *modelViewStack.top;
	return pvm;
}


void Context::MatrixMultVector(Matrix4f& m, Vertex &v){

	float xx,yy,zz,ww;
	xx = m.matrix[0]*v.x + m.matrix[1]*v.y + m.matrix[2]*v.z + m.matrix[3]*v.w;
	yy = m.matrix[4]*v.x + m.matrix[5]*v.y + m.matrix[6]*v.z + m.matrix[7]*v.w;
	zz = m.matrix[8]*v.x + m.matrix[9]*v.y + m.matrix[10]*v.z + m.matrix[11]*v.w;
	ww = m.matrix[12]*v.x + m.matrix[13]*v.y + m.matrix[14]*v.z + m.matrix[15]*v.w;
	v.x=xx;
	v.y=yy;
	v.z=zz;
	v.w=ww;

}


void Context::VertexShader(Matrix4f& pvm, Vertex &v){
	
	MatrixMultVector(pvm,v);
}

void Context::PerspectiveDivision(Vertex &v){
	v.x = v.x / v.w;
	v.y = v.y / v.w;
	v.z = v.z / v.w;
}

void Context::ViewPortTransform(Vertex &v){
	float xx, yy;
	xx = (v.x + 1) * (viewport.width*0.5f)  + viewport.x;
  	yy = (v.y + 1) * (viewport.height*0.5f) + viewport.y;
	v.x=xx;
	v.y=yy;

}

void Context::discardPrimitives(){
	for (auto primitive : primitiveList){
		delete primitive;
	}
	primitiveList.clear();
}

Primitive* Context::findFirstIntersection(Ray &ray, float &t){

	Primitive* ret = nullptr;

	for (int i = 0; i < primitiveList.size(); i++)
	{
		float currentT = -1;
		bool hit = primitiveList[i]->traceRay(ray, &currentT);
		if(!hit) continue;
		//if (currentObject == primitiveList[i]) continue;
		if(currentT < 0.001) continue;
		if(/*!ray.refractiveRay && */!primitiveList[i]->facesVector(ray.origin)) continue;
		if (t < 0 || currentT < t){
			t = currentT;
			ret = primitiveList[i];
		}
	}
	return ret;
}
	

