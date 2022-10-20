//---------------------------------------------------------------------------
// sgl.cpp
// Empty implementation of the SGL (Simple Graphics Library)
// Date:  2016/10/24
// Author: Jaroslav Sloup
// Author: Jaroslav Krivanek, Jiri Bittner, CTU Prague
// Edited: Jakub Hendrich, Daniel Meister, CTU Prague
//---------------------------------------------------------------------------

#include "sgl.h"
#include "context.h"
#include "Rasterizer.h"

#include <cmath>

Context* ConActive = nullptr;
std::vector<Context> ContextArray;
/// Current error code.
static sglEErrorCode _libStatus = SGL_NO_ERROR;

static inline void setErrCode(sglEErrorCode c)
{
  if (_libStatus == SGL_NO_ERROR)
    _libStatus = c;
}

//---------------------------------------------------------------------------
// sglGetError()
//---------------------------------------------------------------------------
sglEErrorCode sglGetError(void)
{
  sglEErrorCode ret = _libStatus;
  _libStatus = SGL_NO_ERROR;
  return ret;
}

//---------------------------------------------------------------------------
// sglGetErrorString()
//---------------------------------------------------------------------------
const char* sglGetErrorString(sglEErrorCode error)
{
  static const char *errStrigTable[] =
  {
    "Operation succeeded",
    "Invalid argument(s) to a call",
    "Invalid enumeration argument(s) to a call",
    "Invalid call",
    "Quota of internal resources exceeded",
    "Internal library error",
    "Matrix stack overflow",
    "Matrix stack underflow",
    "Insufficient memory to finish the requested operation"
  };

  if ((int)error < (int)SGL_NO_ERROR || (int)error > (int)SGL_OUT_OF_MEMORY)
    return "Invalid value passed to sglGetErrorString()";

  return errStrigTable[(int)error];
}

//---------------------------------------------------------------------------
// Initialization functions
//---------------------------------------------------------------------------

void sglInit(void) {
  //init
}

void sglFinish(void) {
  //~init
  //free memory
  for(Context c : ContextArray){
    delete c.color_buffer;
  }
  
}

int sglCreateContext(int width, int height) {
  Context c(static_cast<int>(ContextArray.size()), width, height);
  ContextArray.push_back(c);
  return c.id;
}

void sglDestroyContext(int id) {
  for (size_t i = 0; i < ContextArray.size(); i++)
  {
    if(ContextArray.at(i).id == id) ContextArray.erase(ContextArray.begin() + i);
  }
}

void sglSetContext(int id) {
  
  for (size_t i = 0; i < ContextArray.size(); i++)
  {
    if(ContextArray.at(i).id == id) ConActive = &ContextArray.at(i);
  }
}

int sglGetContext(void) {
  return ConActive->id;
}

float *sglGetColorBufferPointer(void) {
  return ConActive->color_buffer;
}

//---------------------------------------------------------------------------
// Drawing functions
//---------------------------------------------------------------------------

void sglClearColor(float r, float g, float b, float alpha) {
  ConActive->clearColor[0] = r;
  ConActive->clearColor[1] = g;
  ConActive->clearColor[2] = b;
  //ConActive->clearColor[3] = alpha;
}

void sglClear(unsigned what) {
    switch (what)
    {
    case SGL_COLOR_BUFFER_BIT:
      for (size_t i = 0; i < ConActive->colorBufferSize; i+=3)
      {
        ConActive->color_buffer[i] = ConActive->clearColor[0];
        ConActive->color_buffer[i+1] = ConActive->clearColor[1];
        ConActive->color_buffer[i+2] = ConActive->clearColor[2];
      }
      break;
    
    case SGL_DEPTH_BUFFER_BIT:
      break;
    }
}

void sglBegin(sglEElementType mode) {
  ConActive->EleType = mode;

}
void sglEnd(void) {
  VBO *v = &ConActive->vbo;

  for (size_t i = 0; i < v->GetSize(); i+=4)
  {
    ConActive->VertexShader(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1),
          v->vertex_buffer.at(i+2), v->vertex_buffer.at(i+3));
    ConActive->PerspectiveDivision(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1),
          v->vertex_buffer.at(i+2), v->vertex_buffer.at(i+3));
    ConActive->ViewPortTransform(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
  }

  Rasterizer rasterizer(ConActive);

  switch (ConActive->EleType)
  {
  case SGL_POINTS:
    for (size_t i = 0; i < v->GetSize(); i += 4)
    {
      rasterizer.DrawPoint(static_cast<int>(v->vertex_buffer.at(i)), static_cast<int>(v->vertex_buffer.at(i+1)));
    }
    break;

  case SGL_LINES:

    for (size_t i = 0; i < v->GetSize(); i += 8)
    {
      rasterizer.DrawLine(static_cast<int>(v->vertex_buffer.at(i)), static_cast<int>(v->vertex_buffer.at(i+1)),
          static_cast<int>(v->vertex_buffer.at(i+4)), static_cast<int>(v->vertex_buffer.at(i+5)));
    }
    break;

  case SGL_LINE_STRIP:

      for (size_t i = 0; i < v->GetSize()-4; i += 4)
      {
        rasterizer.DrawLine(static_cast<int>(v->vertex_buffer.at(i)), static_cast<int>(v->vertex_buffer.at(i+1)),
            static_cast<int>(v->vertex_buffer.at(i+4)), static_cast<int>(v->vertex_buffer.at(i+5)));
      }
    break;

  case SGL_LINE_LOOP:
    for (size_t i = 0; i < v->GetSize()-4; i += 4)
    {
      rasterizer.DrawLine(static_cast<int>(v->vertex_buffer.at(i)), static_cast<int>(v->vertex_buffer.at(i+1)),
          static_cast<int>(v->vertex_buffer.at(i+4)), static_cast<int>(v->vertex_buffer.at(i+5)));
    }
    rasterizer.DrawLine(static_cast<int>(v->vertex_buffer.at(v->GetSize()-4)), static_cast<int>(v->vertex_buffer.at(v->GetSize()-3)),
          static_cast<int>(v->vertex_buffer.at(0)), static_cast<int>(v->vertex_buffer.at(1)));
    break;

  case SGL_TRIANGLES:
      for (size_t i = 0; i < v->GetSize(); i += 12)
      {
        rasterizer.DrawTriangle(
            static_cast<int>(v->vertex_buffer.at(i)), static_cast<int>(v->vertex_buffer.at(i+1)),
            static_cast<int>(v->vertex_buffer.at(i+4)), static_cast<int>(v->vertex_buffer.at(i+5)),
            static_cast<int>(v->vertex_buffer.at(i+8)), static_cast<int>(v->vertex_buffer.at(i+9)));
      }
    break;

  case SGL_POLYGON:
    /* code */
    break;

  case SGL_AREA_LIGHT:
    /* code */
    break;

  case SGL_LAST_ELEMENT_TYPE:
    /* code */
    break;
  
  default:
    break;
  }
  v->ClearVBO();
}

void sglVertex4f(float x, float y, float z, float w) {
  ConActive->vbo.InsertVertex(x,y,z,w);
}

void sglVertex3f(float x, float y, float z) {
  sglVertex4f(x,y,z,1);
}

void sglVertex2f(float x, float y) {
  sglVertex4f(x,y,0,1);
}

void sglCircle(float x, float y, float z, float radius) {

  //calculate the scale from these matricies
  float w=1;
  ConActive->VertexShader(x,y,z,w);
  ConActive->PerspectiveDivision(x,y,z,w);
  ConActive->ViewPortTransform(x,y);

  Matrix4f scalingMatrix = *ConActive->projectionStack.top * *ConActive->modelViewStack.top;
  scalingMatrix = ConActive->viewport.viewportMatrix * scalingMatrix;

  //ConActive->viewport.viewportMatrix.PrintMatrix();

  float scalingFactor = sqrt((scalingMatrix.matrix[0] * scalingMatrix.matrix[5]) - (scalingMatrix.matrix[1] * scalingMatrix.matrix[4]));

  Rasterizer rasterizer(ConActive);

  float Sradius = radius * scalingFactor;
  int xs, ys, p, fourX, fourY;
  int ix = static_cast<int>(x);
  int iy = static_cast<int>(y);
  xs = 0; ys = Sradius;
  p = 3 - 2 * Sradius;
  fourX = 0; fourY = 4 * Sradius;
  while (xs <= ys) {
    rasterizer.setPixel(ix+xs,iy+ys);
    rasterizer.setPixel(ix+ys,iy+xs);
    rasterizer.setPixel(ix+xs,iy-ys);
    rasterizer.setPixel(ix+ys,iy-xs);
    rasterizer.setPixel(ix-xs,iy+ys);
    rasterizer.setPixel(ix-ys,iy+xs);
    rasterizer.setPixel(ix-xs,iy-ys);
    rasterizer.setPixel(ix-ys,iy-xs);

    if (p > 0) {
      p = p - fourY + 4;
      fourY = fourY - 4;
      ys = ys - 1;
      }
  p = p + fourX + 6;
  fourX = fourX + 4;
  xs = xs + 1;
  }
  //use rasterizer.setPixel(x,y); to draw the pixel on screen
  //which pixels to draw you determine in the window coordinates which are ranging from 0 to Con->frameWidth/Height
}

void sglEllipse(float x, float y, float z, float a, float b) {
  sglBegin(SGL_LINE_LOOP);
  float pi = static_cast<float>(2*acos(0.0));
  float t = pi / 20;
  int j = 0;
  for (float i = 0; i < 2*pi; i+=t)
  {
      ConActive->vbo.InsertVertexAt(x+(a*cos(i))  , y+(b*sin(i)), z, 1, 0+j);
      j++;
  }
  sglEnd();
}

void sglArc(float x, float y, float z, float radius, float from, float to) {
  sglBegin(SGL_LINE_STRIP);
  //float pi = 2*acos(0.0);
  float t = (to-from)/40;
  int j = 0;
  for (float i = from; i < to; i+=t)
  {
      ConActive->vbo.InsertVertexAt(x+(radius*cos(i))  , y+(radius*sin(i)), z, 1, 0+j);
      j++;
  }
  sglEnd();
}

void sglLine(float x_beggin,float y_beggin,float x_end,float y_end){

}

void sglPoint(float x,float y){
  
}
//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

void sglMatrixMode(sglEMatrixMode mode) {
  ConActive->MatrixMode = mode;
}

void sglPushMatrix(void) {
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:{
    Matrix4f mat = *ConActive->modelViewStack.top;
    ConActive->modelViewStack.Push(mat);
  }
  break;

  case SGL_PROJECTION:{
    Matrix4f mat(*ConActive->modelViewStack.top);
    ConActive->projectionStack.Push(mat);
  }
  break;
  
  }
}

void sglPopMatrix(void) {
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.Pop();
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.Pop();
    break;
  }
}

void sglLoadIdentity(void) {
    float mat[16] = 
             {1,0,0,0,
              0,1,0,0,
              0,0,1,0,
              0,0,0,1};
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    std::copy(mat,mat+16,ConActive->modelViewStack.top->matrix);
    break;

  case SGL_PROJECTION:
    std::copy(mat,mat+16,ConActive->projectionStack.top->matrix);
    break;
  }
}

void sglLoadMatrix(const float *matrix) {

  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    std::copy(matrix,matrix+16,ConActive->modelViewStack.top->matrix);
    break;

  case SGL_PROJECTION:
    std::copy(matrix,matrix+16,ConActive->projectionStack.top->matrix);
    break;
  }
}

void sglMultMatrix(const float *matrix) {
  Matrix4f m;
  float temp[16];
  std::copy(matrix,matrix+16,temp);
  m.InsertMatrix(temp);
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }
}

void sglTranslate(float x, float y, float z) {

  Matrix4f m;
  float mat[16] = {1,0,0,x,
                   0,1,0,y,
                   0,0,1,z,
                   0,0,0,1};
  m.InsertMatrix(mat);

  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }
}

void sglScale(float scalex, float scaley, float scalez) {

  Matrix4f m;
  float mat[16] = {scalex,0,0,0,
                   0,scaley,0,0,
                   0,0,scalez,0,
                   0,0,0,1};
  m.InsertMatrix(mat);

  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }
}

void sglRotate2D(float angle, float centerx, float centery) {
  
  sglTranslate(centerx, centery, 0);
  Matrix4f m;
  float mat[16] = {cos(angle), -sin(angle), 0, 0,
                   sin(angle), cos(angle) , 0, 0,
                   0         , 0          , 1, 0,
                   0         , 0          , 0, 1 };

  std::copy(mat, mat+16, m.matrix);
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }

  sglTranslate(-centerx, -centery, 0);

}

void sglRotateY(float angle) {
  float mat[16] = {cos(angle), 0, -sin(angle), 0,
                   0,          1, 0,           0,
                   sin(angle), 0, cos(angle) , 0,
                   0,          0, 0,           1 };
  Matrix4f m;
  std::copy(mat,mat+16, m.matrix);
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }
}

void sglOrtho(float left, float right, float bottom, float top, float near, float far) {

  ConActive->viewport.far = far;
  ConActive->viewport.near = near;

  Matrix4f orthoMat;
  orthoMat.matrix[0] = 2 / (right-left);
  orthoMat.matrix[5] = 2 / (top - bottom);
  orthoMat.matrix[10] = -2 / (far - near);

  orthoMat.matrix[3] =-((right + left) / (right - left));
  orthoMat.matrix[7] =-((top + bottom) / (top - bottom));
  orthoMat.matrix[11] =-((far + near) / (far - near));

  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(orthoMat);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(orthoMat);
    break;
  }
}

void sglFrustum(float left, float right, float bottom, float top, float near, float far) {

  ConActive->viewport.far = far;
  ConActive->viewport.near = near;

  Matrix4f m;
  m.matrix[0] = (2 * near)/(right - left);
  m.matrix[5] = (2 * near)/(top - bottom);


  m.matrix[2] =((right + left) / (right - left));
  m.matrix[6] =((top + bottom) / (top - bottom));
  m.matrix[10] =-((far + near) / (far - near));

  m.matrix[11] = -(2 * far * near)/(far - near);

  m.matrix[14] = -1;
  m.matrix[15] = 0;


  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    ConActive->modelViewStack.MultiplyFromRight(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromRight(m);
    break;
  }
}

void sglViewport(int x, int y, int width, int height) {
  ConActive->viewport.x = x;
  ConActive->viewport.y = y;
  ConActive->viewport.width = width;
  ConActive->viewport.height = height;

  ConActive->viewport.CreateViewportMatrix();
}

//---------------------------------------------------------------------------
// Attribute functions
//---------------------------------------------------------------------------

void sglColor3f(float r, float g, float b) {
  ConActive->currentColor[0] = r;
  ConActive->currentColor[1] = g;
  ConActive->currentColor[2] = b;
}

void sglAreaMode(sglEAreaMode mode) {}

void sglPointSize(float size) {
  ConActive->pointSize = static_cast<int>(size);
}

void sglEnable(sglEEnableFlags cap) {
  if(cap = SGL_DEPTH_TEST){
    ConActive->depthActive = true;
  }
}

void sglDisable(sglEEnableFlags cap) {
  if(cap = SGL_DEPTH_TEST){
    ConActive->depthActive = false;
  }
}

//---------------------------------------------------------------------------
// RayTracing oriented functions
//---------------------------------------------------------------------------

void sglBeginScene() {}

void sglEndScene() {}

void sglSphere(const float x,
               const float y,
               const float z,
               const float radius)
{}

void sglMaterial(const float r,
                 const float g,
                 const float b,
                 const float kd,
                 const float ks,
                 const float shine,
                 const float T,
                 const float ior)
{}

void sglPointLight(const float x,
                   const float y,
                   const float z,
                   const float r,
                   const float g,
                   const float b)
{}

void sglRayTraceScene() {}

void sglRasterizeScene() {}

void sglEnvironmentMap(const int width,
                       const int height,
                       float *texels)
{}

void sglEmissiveMaterial(const float r,
                         const float g,
                         const float b,
                         const float c0,
                         const float c1,
                         const float c2)
{}
