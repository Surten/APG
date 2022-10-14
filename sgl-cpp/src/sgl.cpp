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
}

int sglCreateContext(int width, int height) {
  Context c;
  c.id = ContextArray.size();
  c.frameWidth = width;
  c.frameHeight = height;

  c.color_buffer = (float*)malloc(3*sizeof(float)*width*height);
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
  ConActive = &ContextArray.at(id);
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
  ConActive->clearColor[3] = alpha;
}

void sglClear(unsigned what) {
//  ContextArray[manager.currentContext].color_buff
}

void sglBegin(sglEElementType mode) {
  ConActive->EleType = mode;
}

void sglEnd(void) {




  switch (ConActive->EleType)
  {
  case SGL_POINTS:
    /* code */
    break;

  case SGL_LINES:
    /* code */
    break;

  case SGL_LINE_STRIP:
    /* code */
    break;

  case SGL_LINE_LOOP:
    /* code */
    break;

  case SGL_TRIANGLES:
    /* code */
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

void sglCircle(float x, float y, float z, float radius) {}

void sglEllipse(float x, float y, float z, float a, float b) {}

void sglArc(float x, float y, float z, float radius, float from, float to) {}

//---------------------------------------------------------------------------
// Transform functions
//---------------------------------------------------------------------------

void sglMatrixMode(sglEMatrixMode mode) {
  ConActive->MatrixMode = mode;
}

void sglPushMatrix(void) {
  float m[16];
  Matrix4f mat;
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    memcpy(m,ConActive->modelViewStack.top->matrix,sizeof(float)*16);
    mat.InsertMatrix(m);
    ConActive->modelViewStack.Push(mat);
    break;

  case SGL_PROJECTION:
    memcpy(m,ConActive->projectionStack.top->matrix,sizeof(float)*16);
    mat.InsertMatrix(m);
    ConActive->projectionStack.Push(mat);
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
    memcpy(ConActive->modelViewStack.top->matrix,mat,sizeof(float)*16);
    break;

  case SGL_PROJECTION:
    memcpy(ConActive->modelViewStack.top->matrix,mat,sizeof(float)*16);
    break;
  }
}

void sglLoadMatrix(const float *matrix) {

  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    memcpy(ConActive->modelViewStack.top->matrix,matrix,sizeof(float)*16);
    break;

  case SGL_PROJECTION:
    memcpy(ConActive->modelViewStack.top->matrix,matrix,sizeof(float)*16);
    break;
  }
}

void sglMultMatrix(const float *matrix) {
  Matrix4f m;
  float temp[16];
  memcpy(temp,matrix,sizeof(float)*16);
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
    ConActive->modelViewStack.MultiplyFromLeft(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromLeft(m);
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
    ConActive->modelViewStack.MultiplyFromLeft(m);
    break;

  case SGL_PROJECTION:
    ConActive->projectionStack.MultiplyFromLeft(m);
    break;
  }
}

void sglRotate2D(float angle, float centerx, float centery) {}

void sglRotateY(float angle) {}

void sglOrtho(float left, float right, float bottom, float top, float near, float far) {

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

void sglFrustum(float left, float right, float bottom, float top, float near, float far) {}

void sglViewport(int x, int y, int width, int height) {

  //x_w = (x_nd + 1) * width/2  + x
  //y_w = (y_nd + 1) * height/2 + y
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

void sglPointSize(float size) {}

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
