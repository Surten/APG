#include "context.h"
#include "cstdlib" // random

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
	if (isEnvMapInitialized) delete[] envMap;
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

	for (size_t i = 0; i < primitiveList.size(); i++)
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
	
void Context::setPointlightsForEmissiveTriangle(TriangleP &triangle){
	Vertex &baseVertex = triangle.v0;

	Vertex e1 = (triangle.v1 - triangle.v0);
	Vertex e2 = (triangle.v2 - triangle.v0);
	float triangleArea = cross(e1, e2).length() / 2.0f;

	Vertex normal = triangle.normal;

	Vertex normalOffset = triangle.normal * .1f; // to prevent z-fighting



	
	for (int i = 0; i < AREA_LIGHT_NUM_SAMPLES; i++) {
		float r1 = (float)rand()/(float)(RAND_MAX);
		float r2 = (float)rand()/(float)(RAND_MAX);
		float u = 1.0f - sqrtf(r1);
		float v = (1.0f - r2) * sqrt(r1);
		Vertex point = baseVertex 
						+ e1 * u 
						+ e2 * v
						+ normalOffset;

		Color color{currentMaterial.color};	
		Vertex position{point};
		PointLight light{position, color, 
			currentMaterial.c0, currentMaterial.c1, currentMaterial.c2,
			triangleArea, normal};
		pointLightList.push_back(light);
	}

	
}
