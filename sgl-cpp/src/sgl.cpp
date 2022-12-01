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
#include "Ray.h"
#include "Primitive.h"

#include <omp.h>
#include <thread>

#include <cmath>

#define TWO_PI 6.2831853f

Context* ConActive = nullptr;
std::vector<Context*> ContextArray;
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
  const auto processor_count = std::thread::hardware_concurrency();
  std::cout << "processor count " << processor_count << std::endl;
  //init
}

void sglFinish(void) {
  //~init
  //free memory
  for(auto c : ContextArray){
    delete c;
  }
  ContextArray.clear();
  ConActive = nullptr;
  
}

int sglCreateContext(int width, int height) {
  Context *c = new Context(static_cast<int>(ContextArray.size()), width, height);
  ContextArray.push_back(c);
  return c->id;
}

void sglDestroyContext(int id) {
  for (size_t i = 0; i < ContextArray.size(); i++)
  {
    if(ContextArray.at(i)->id == id){ 
      delete ContextArray.at(i);
      ContextArray.erase(ContextArray.begin() + i);
      }
  }
}

void sglSetContext(int id) {
  
  for (size_t i = 0; i < ContextArray.size(); i++)
  {
    if(ContextArray.at(i)->id == id) ConActive = ContextArray.at(i);
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

  if ((what & SGL_COLOR_BUFFER_BIT) == SGL_COLOR_BUFFER_BIT){
    for (size_t i = 0; i < ConActive->colorBufferSize; i+=3)
    {
      ConActive->color_buffer[i] = ConActive->clearColor[0];
      ConActive->color_buffer[i+1] = ConActive->clearColor[1];
      ConActive->color_buffer[i+2] = ConActive->clearColor[2];
    }
  }
  if ((what & SGL_DEPTH_BUFFER_BIT) == SGL_DEPTH_BUFFER_BIT){
    for (size_t i = 0; i < ConActive->depthBufferSize; i++)
    {
      ConActive->depth_buffer[i] = INFINITY;
    }
  }
}

void sglBegin(sglEElementType mode) {
  ConActive->EleType = mode;

}
void sglEnd(void) {
  VBO *v = &ConActive->vbo;

  if (ConActive->beginSceneActive){
    Rasterizer rasterizer(ConActive);
    rasterizer.vboToPrimitives();
    v->ClearVBO();
    return;
  }


  if (v->GetSize() == 0){
    v->ClearVBO();
    return;
  }

  Matrix4f pvm = ConActive->GetPVMMatrix();
  for (size_t i = 0; i < v->GetSize(); i++)
  {
    ConActive->VertexShader(pvm, v->vertex_buffer.at(i));
    ConActive->PerspectiveDivision(v->vertex_buffer.at(i));
    ConActive->ViewPortTransform(v->vertex_buffer.at(i));
  }

  Rasterizer rasterizer(ConActive);

  switch (ConActive->EleType)
  {
  case SGL_POINTS:
    for (size_t i = 0; i < v->GetSize(); i++)
    {
      if(v->vertex_buffer.at(i).w == 0) continue;
      rasterizer.DrawPoint(v->vertex_buffer.at(i));
    }
    break;

  case SGL_LINES:

    for (size_t i = 0; i < v->GetSize()-1; i+=2)
    {
      if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0) continue;
      rasterizer.DrawLine(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
    }
    break;

  case SGL_LINE_STRIP:

      for (size_t i = 0; i < v->GetSize()-1; i++)
      {
        if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0) continue;
        rasterizer.DrawLine(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
      }
    break;

  case SGL_LINE_LOOP:
    for (size_t i = 0; i < v->GetSize()-1; i++)
    {
      if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0) continue;
      rasterizer.DrawLine(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
    }
    if(v->vertex_buffer.at(static_cast<int>(v->GetSize())-1).w == 0 || v->vertex_buffer.at(0).w == 0) break;
    rasterizer.DrawLine(v->vertex_buffer.at(static_cast<int>(v->GetSize())-1), v->vertex_buffer.at(0));
    break;

  case SGL_TRIANGLES:
    switch (ConActive->areaMode)
    {
    case SGL_POINT:
      for (size_t i = 0; i < v->GetSize()-2; i+=3)
      {
        if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0 || v->vertex_buffer.at(i+2).w == 0) continue;
        rasterizer.DrawPoint(v->vertex_buffer.at(i));
        rasterizer.DrawPoint(v->vertex_buffer.at(i+1));
        rasterizer.DrawPoint(v->vertex_buffer.at(i+2));
      }
      break;
    
    case SGL_LINE:
      for (size_t i = 0; i < v->GetSize()-2; i += 3)
      {
        if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0 || v->vertex_buffer.at(i+2).w == 0) continue;
        rasterizer.DrawLine(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
        rasterizer.DrawLine(v->vertex_buffer.at(i+1), v->vertex_buffer.at(i+2));
        rasterizer.DrawLine(v->vertex_buffer.at(i+2), v->vertex_buffer.at(i));
      }
      break;

    case SGL_FILL:
      for (size_t i = 0; i < v->GetSize()-2; i += 3)
      {
        if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0 || v->vertex_buffer.at(i+2).w == 0) continue;
        int yMax = -1, yMin = INT32_MAX;
        std::vector<SLFEdge> edges = rasterizer.CreateEdges(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1), v->vertex_buffer.at(i+2), yMax, yMin);
        rasterizer.ScanLineFill(edges, yMax, yMin);
      }
    
      
      break;
    }
    break;

  case SGL_POLYGON:
    switch (ConActive->areaMode)
    {
    case SGL_POINT:
      for (size_t i = 0; i < v->GetSize(); i++)
      {
        if(v->vertex_buffer.at(i).w == 0) break;
        rasterizer.DrawPoint(v->vertex_buffer.at(i));
      }
      break;
    
    case SGL_LINE:
      for (size_t i = 0; i < v->GetSize()-1; i++)
      {
        if(v->vertex_buffer.at(i).w == 0 || v->vertex_buffer.at(i+1).w == 0){v->ClearVBO(); return;}
        rasterizer.DrawLine(v->vertex_buffer.at(i), v->vertex_buffer.at(i+1));
      }
      if(v->vertex_buffer.at(static_cast<int>(v->GetSize())-1).w == 0 || v->vertex_buffer.at(0).w == 0)
      rasterizer.DrawLine(v->vertex_buffer.at(static_cast<int>(v->GetSize())-1), v->vertex_buffer.at(0));
      break;

    case SGL_FILL:
      for (size_t i = 0; i < v->GetSize(); i++)
      {
        if(v->vertex_buffer.at(i).w == 0) {v->ClearVBO(); return;}
      }
      int yMax = -1, yMin = INT32_MAX;
      std::vector<SLFEdge> edges = rasterizer.CreateEdges(*v, yMax, yMin);
      rasterizer.ScanLineFill(edges, yMax, yMin);
      break;
    }
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
  Vertex v(x,y,z,1);
  Matrix4f pvm = ConActive->GetPVMMatrix();
  ConActive->VertexShader(pvm,v);
  ConActive->PerspectiveDivision(v);
  ConActive->ViewPortTransform(v);

  Matrix4f scalingMatrix = *ConActive->projectionStack.top * *ConActive->modelViewStack.top;
  scalingMatrix = ConActive->viewport.viewportMatrix * scalingMatrix;

  //ConActive->viewport.viewportMatrix.PrintMatrix();

  float scalingFactor = sqrt((scalingMatrix.matrix[0] * scalingMatrix.matrix[5]) - (scalingMatrix.matrix[1] * scalingMatrix.matrix[4]));

  Rasterizer rasterizer(ConActive);

  float Sradius = radius * scalingFactor;
  int xs, ys, p, fourX, fourY;
  int ix = static_cast<int>(v.x);
  int iy = static_cast<int>(v.y);
  xs = 0; ys = static_cast<int>(Sradius);
  p = 3 - 2 * ys;
  fourX = 0; fourY = 4 * ys;

  switch (ConActive->areaMode)
  {
  case SGL_POINT:
    rasterizer.setPixel(SCVertex(ix,iy,v.z));
    break;
  
  case SGL_LINE:
    while (xs <= ys) {
      rasterizer.setPixel(SCVertex(ix+xs,iy+ys, v.z));
      rasterizer.setPixel(SCVertex(ix-xs,iy+ys, v.z)); 

      rasterizer.setPixel(SCVertex(ix+ys,iy+xs, v.z));
      rasterizer.setPixel(SCVertex(ix-ys,iy+xs, v.z));

      rasterizer.setPixel(SCVertex(ix+ys,iy-xs, v.z));
      rasterizer.setPixel(SCVertex(ix-ys,iy-xs, v.z));

      rasterizer.setPixel(SCVertex(ix+xs,iy-ys, v.z));
      rasterizer.setPixel(SCVertex(ix-xs,iy-ys, v.z));

      if (p > 0) {
        p = p - fourY + 4;
        fourY = fourY - 4;
        ys = ys - 1;
      }
      p = p + fourX + 6;
      fourX = fourX + 4;
      xs = xs + 1;
    }
    break;

  case SGL_FILL:
    while (xs <= ys) {

      for (int i = ix-xs; i < ix+xs; i++)
      {
        rasterizer.setPixel(SCVertex(i,iy+ys, v.z));
      }
      for (int i = ix-ys; i < ix+ys; i++)
      {
        rasterizer.setPixel(SCVertex(i,iy+xs, v.z));
      }
      for (int i = ix-ys; i < ix+ys; i++)
      {
        rasterizer.setPixel(SCVertex(i,iy-xs, v.z));
      }
      for (int i = ix-xs; i < ix+xs; i++)
      {
        rasterizer.setPixel(SCVertex(i,iy-ys, v.z));
      }

      if (p > 0) {
        p = p - fourY + 4;
        fourY = fourY - 4;
        ys = ys - 1;
      }
      p = p + fourX + 6;
      fourX = fourX + 4;
      xs = xs + 1;
    }
    break;

  }
  
}

void sglEllipse(float x, float y, float z, float a, float b) {

  switch (ConActive->areaMode)
  {
  case SGL_POINT:
    sglBegin(SGL_POINTS);
    ConActive->vbo.InsertVertex(x, y, z, 1);
    sglEnd();
    return;

  case SGL_LINE:
    sglBegin(SGL_LINE_LOOP);
    break;

  case SGL_FILL:
    sglBegin(SGL_POLYGON);
    break;
  }

    double pi = static_cast<float>(2*acos(0.0));
    double t = pi / 20;
    for (double i = 0; i < 2*pi; i+=t)
    {
        ConActive->vbo.InsertVertex(static_cast<float>(x+(a*cos(i)))  , static_cast<float>(y+(b*sin(i))), z, 1);
    }

  sglEnd();
}

void sglArc(float x, float y, float z, float radius, float from, float to) {
  switch (ConActive->areaMode)
  {
  case SGL_POINT:
    sglBegin(SGL_POINTS);
    ConActive->vbo.InsertVertex(x, y, z, 1);
    sglEnd();
    return;
    break;

  case SGL_LINE:
    sglBegin(SGL_LINE_STRIP);

    break;

  case SGL_FILL:
    ConActive->vbo.InsertVertex(x, y, z, 1);
    sglBegin(SGL_POLYGON);
    break;
  }

    // add two pi if from > to
    float realAngle;
    if (to > from){
        realAngle = fabs(to - from);
    } else {
        realAngle = fabs(TWO_PI + to - from);
    }

    int numSegments = static_cast<int>(roundf(40 * realAngle / TWO_PI));
    float angleDiff = realAngle / numSegments;
    float angle = from;
    // need n+1 vertices for n segments
    for (double i = 0; i < numSegments + 1; i++)
    {
      ConActive->vbo.InsertVertex(static_cast<float>(x+(radius*cos(angle)))  , static_cast<float>(y+(radius*sin(angle))), z, 1);
      angle += angleDiff;
    }

  sglEnd();




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
  float temp[16];

  temp[0] = matrix[0];
  temp[1] = matrix[4];
  temp[2] = matrix[8];
  temp[3] = matrix[12];
  temp[4] = matrix[1];
  temp[5] = matrix[5];
  temp[6] = matrix[9];
  temp[7] = matrix[13];
  temp[8] = matrix[2];
  temp[9] = matrix[6];
  temp[10] = matrix[10];
  temp[11] = matrix[14];
  temp[12] = matrix[3];
  temp[13] = matrix[7];
  temp[14] = matrix[11];
  temp[15] = matrix[15];
  switch (ConActive->MatrixMode)
  {
  case SGL_MODELVIEW:
    std::copy(temp,temp+16,ConActive->modelViewStack.top->matrix);
    break;

  case SGL_PROJECTION:
    std::copy(temp,temp+16,ConActive->projectionStack.top->matrix);
    break;
  }
}

void sglMultMatrix(const float *matrix) {
  Matrix4f m;
  float temp[16];
  temp[0] = matrix[0];
  temp[1] = matrix[4];
  temp[2] = matrix[8];
  temp[3] = matrix[12];
  temp[4] = matrix[1];
  temp[5] = matrix[5];
  temp[6] = matrix[9];
  temp[7] = matrix[13];
  temp[8] = matrix[2];
  temp[9] = matrix[6];
  temp[10] = matrix[10];
  temp[11] = matrix[14];
  temp[12] = matrix[3];
  temp[13] = matrix[7];
  temp[14] = matrix[11];
  temp[15] = matrix[15];
  
  

  //std::copy(matrix,matrix+16,temp);
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
  float mat[16] = {static_cast<float>(cos(angle)), static_cast<float>(-sin(angle)), 0, 0,
                   static_cast<float>(sin(angle)), static_cast<float>(cos(angle)) , 0, 0,
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
  float mat[16] = {static_cast<float>(cos(angle)), 0, static_cast<float>(-sin(angle)), 0,
                   0,          1, 0,           0,
                   static_cast<float>(sin(angle)), 0, static_cast<float>(cos(angle)) , 0,
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
  Matrix4f m;
  m.matrix[0] = (2.0f * near)/(right - left);
  m.matrix[5] = (2.0f * near)/(top - bottom);


  m.matrix[2] =((right + left) / (right - left));
  m.matrix[6] =((top + bottom) / (top - bottom));
  m.matrix[10] =(-(far + near) / (far - near));

  m.matrix[11] = -(2.0f * far * near)/(far - near);

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
  ConActive->viewport.x = (float)x;
  ConActive->viewport.y = (float)y;
  ConActive->viewport.width = (float)width;
  ConActive->viewport.height = (float)height;

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

void sglAreaMode(sglEAreaMode mode) {
  ConActive->areaMode = mode;
}

void sglPointSize(float size) {
  ConActive->pointSize = static_cast<int>(size);
}

void sglEnable(sglEEnableFlags cap) {
  if ((cap & SGL_DEPTH_TEST) == SGL_DEPTH_TEST){
    ConActive->depthActive = true;
  }
}

void sglDisable(sglEEnableFlags cap) {
  if ((cap & SGL_DEPTH_TEST) == SGL_DEPTH_TEST){
    ConActive->depthActive = false;
  }
}

//---------------------------------------------------------------------------
// RayTracing oriented functions
//---------------------------------------------------------------------------

void sglBeginScene() {
  ConActive->beginSceneActive = true;
  ConActive->discardPrimitives();
  // TODO: handle invalid calls
}

void sglEndScene() {
  ConActive->beginSceneActive = false;
  // TODO: handle invalid calls
}

void sglSphere(const float x,
               const float y,
               const float z,
               const float radius)
{
  // TODO: handle calls out of beginScene()
  Material mat{ConActive->currentMaterial};
  SphereP* sphere = new SphereP{x, y, z, radius, mat};
  ConActive->primitiveList.push_back(sphere);
}

void sglMaterial(const float r,
                 const float g,
                 const float b,
                 const float kd,
                 const float ks,
                 const float shine,
                 const float T,
                 const float ior)
{
  ConActive->currentMaterial.setProperties(r, g, b,
                                           kd, ks,
                                           shine, T, ior);
}

void sglPointLight(const float x,
                   const float y,
                   const float z,
                   const float r,
                   const float g,
                   const float b)
{
  Vertex position{x, y, z};
  Color color{r, g, b};
  PointLight light{position, color};
  ConActive->pointLightList.push_back(light);
}

/*void drawAxis(){
  Vertex origin{0.0f, 0.0f, 0.0f, 1.0f};
  Vertex      x{1.0f, 0.0f, 0.0f, 1.0f};
  Vertex      y{0.0f, 1.0f, 0.0f, 1.0f};
  Vertex      z{0.0f, 0.0f, 1.0f, 1.0f};

  Matrix4f pvm = ConActive->GetPVMMatrix();

  ConActive->VertexShader(pvm, origin);
  ConActive->PerspectiveDivision(origin);
  ConActive->ViewPortTransform(origin);
  ConActive->VertexShader(pvm, x);
  ConActive->PerspectiveDivision(x);
  ConActive->ViewPortTransform(x);
  ConActive->VertexShader(pvm, y);
  ConActive->PerspectiveDivision(y);
  ConActive->ViewPortTransform(y);
  ConActive->VertexShader(pvm, z);
  ConActive->PerspectiveDivision(z);
  ConActive->ViewPortTransform(z);

  Rasterizer rasterizer(ConActive);
  sglColor3f(1.0f, 0.0f, 0.0f);
  rasterizer.DrawLine(origin, x);
  sglColor3f(0.0f, 1.0f, 0.0f);
  rasterizer.DrawLine(origin, y);
  sglColor3f(0.0f, 0.0f, 1.0f);
  rasterizer.DrawLine(origin, z);
}*/


bool traceShadowRay(Ray &ray, PointLight &light){
  float hitT = -1;
  float distance = (light.position - ray.origin).length() - 0.01f;  // -0.01f is here to handle edge case where the light is too close to the wall
  ConActive->findFirstIntersection(ray, hitT);
  // if(!(hitT > distance || hitT == -1)){
  //   printf("here\n");
  // }
  return (hitT > distance || hitT == -1);
}



Color myRayTrace(Ray inRay, int depth, float coeficient){
  Color retColor;
  if(depth > 8){return retColor;}

  float hitT = -1;
  Primitive* currentPrimitive = ConActive->findFirstIntersection(inRay, hitT);
  //printf("%f, %f, %f\n", currentPrimitive->material.color.r, currentPrimitive->material.color.g, currentPrimitive->material.color.b);


  if(currentPrimitive == nullptr || hitT < 0){
    // todo for assignment 5 - insert enviroment map color here
    retColor.r = ConActive->clearColor[0];
    retColor.g = ConActive->clearColor[1];
    retColor.b = ConActive->clearColor[2];
    return retColor;
  }
  

  Vertex positionHit = inRay.origin + (inRay.direction * hitT);
  Vertex lookDir = inRay.direction * (-1);
  lookDir.normalize();

  if(coeficient * currentPrimitive->material.ks > 0.01f){
    Ray reflectedRay = currentPrimitive->getReflectedRay(inRay, positionHit);
    retColor += myRayTrace(reflectedRay, depth+1, coeficient * currentPrimitive->material.ks);
  }

  // if(coeficient * currentPrimitive->material.T > 0.01f){
  //  Ray refractedRay = currentPrimitive->getRefractedRay(inRay, positionHit);
  //  if(refractedRay.direction.length() > 0.0001f){   //perfect reflection check
  //   retColor += myRayTrace(refractedRay, depth+1, coeficient * currentPrimitive->material.T);
  //  }
  // }

  for(auto light : ConActive->pointLightList){
    Vertex lightDir(light.position - positionHit);
    lightDir.normalize();
    Ray shadowRay(positionHit, lightDir, 0, INFINITY);
    if(traceShadowRay(shadowRay, light))  retColor += currentPrimitive->getColorFromLightSource(light, positionHit, lookDir) * coeficient;
  }

  return retColor;
}



void sglRayTraceScene() {
  Matrix4f projectionInv = Matrix4f(*ConActive->projectionStack.top);
  projectionInv.invert();

  Matrix4f modelviewInv = Matrix4f(*ConActive->modelViewStack.top);
  modelviewInv.invert();

  Matrix4f viewportInv = Matrix4f(ConActive->viewport.viewportMatrix);
  viewportInv.invert();

  Vertex cameraPosition{0.0f, 0.0f, 0.0f, 1.0f};
  ConActive->MatrixMultVector(modelviewInv, cameraPosition);
  cameraPosition.perspDivide();

  using std::min;
  using std::max;

  // get near plane from projection matrix
  // https://forums.structure.io/t/near-far-value-from-projection-matrix/3757
  float m22 = - ConActive->projectionStack.top->matrix[10];
  float m32 = - ConActive->projectionStack.top->matrix[11];
  float far = (2.0f*m32)/(2.0f*m22-2.0f);
  float near = ((m22-1.0f)*far)/(m22+1.0f);


  Rasterizer rasterizer{ConActive};

  int width = ConActive->frameWidth;
  int height = ConActive->frameHeight;

  Matrix4f mvpv_inv = modelviewInv * projectionInv * viewportInv;
/*
  // backface culling
  std::vector<Primitive*> frontfacePrimitives;
  for (auto &p : ConActive->primitiveList){
    if (p->facesVector(cameraPosition)){
      frontfacePrimitives.push_back(p);
      p->setMinDistFromCamera(cameraPosition);
    }
  }*/

  //using std::thread;
  //auto threadFun = [&](int threadNum, int start, int chunkSize){

  // iterate over pixels in screen
  //#pragma omp parallel for schedule(static)
    for (int y = 0/*start*/; y < width/*start + chunkSize*/; y++){
      for (int x = 0; x < height; x++){
        if(y == 238 && x == 156){
         printf("%d %d \n", y, x);
        }
        // transform pixel into world space
        Vertex pxInWspc{static_cast<float>(y), static_cast<float>(x), -1.0f, 1.0f};
        mvpv_inv.MultiplyVector(pxInWspc);
        Vertex direction = pxInWspc - cameraPosition;
        direction.normalize(); // ray direction

        Ray ray(cameraPosition, direction, near, INFINITY);
        Color c = myRayTrace(ray, 0, 1.0f);


        // if(x > (float)height/4.0f && x < (float)height /2.0f && y > (float)width/4.0f && y < (float)width*1.0f /2.0f && c.r <= 0.01 && c.g <= 0.01 && c.b <= 0.01){
        //   printf("%d %d \n", y, x);
        //   c.r = 1;
        // }

        rasterizer.FragmentShader(SCVertex(y, x, 0), c);

        // iterate over primitives
        /*for (auto& p : frontfacePrimitives){
          float maxT = INFINITY;
          if (ConActive->depthActive){
            maxT = min(maxT, ConActive->depth_buffer[y * width + x]);
          }
          
          if (p->minDistFromCamera > maxT){
            continue;
          }

          ray.setProperties(cameraPosition, direction, near, maxT);
          float t; // distance at which the ray hit
          bool hit = p->traceRay(ray, &t);
          if (hit){
            if (dynamic_cast<TriangleP*>(p)){
              x = x;
            }
            Vertex point{cameraPosition + t * ray.direction};
            SCVertex screenVert{y, x, t};
            Vertex normal = p->normalAt(point);
            rasterizer.FragmentShader(screenVert, point, ray.direction, normal, p->material);
            if (ConActive->depthActive){
              ConActive->depth_buffer[y * width + x] = t;
            }
          } 
        }*/
      }
    }
  //};
  // const auto processor_count = std::max(thread::hardware_concurrency(), 1u);
  // std::vector<thread> threadPool;
  // for (unsigned int i = 0; i < processor_count; i++)
  // {
  //   int chunkSize = width / processor_count;
  //   int start = chunkSize * i;
  //   if (i == processor_count -  1){
  //     chunkSize = width - ((processor_count - 1) * chunkSize); // padding if the processor count is not factor of width
  //   }
  //   threadPool.push_back(thread(threadFun, i, start, chunkSize));
  // }
  // for (auto &t : threadPool){
  //   t.join();
  // }

  // drawAxis();
}

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
