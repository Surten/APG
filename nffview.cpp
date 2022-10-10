//---------------------------------------------------------------------------
// nffread.cpp
// Simple OpenGL application to display contents of a NFF file.
// Date:  2007/10/04
// Author: Jaroslav Krivanek, CTU Prague
//---------------------------------------------------------------------------

// standard headers
#include <cstdio>
#include <list>
#include <iostream>
// OpenGL headers
#include <GL/glut.h>

// project headers
#include "nffread.h"
#include "nffwrite.h"
#include "nffstore.h"

using namespace std;

NFFStore nffstore;

// ==========================================================

/// rotation around x and y
GLfloat xr=0,yr=0;
/// how much mouse dragging rotates
GLfloat rfactor=180;

/// what is going on with the mouse right now
enum EDraggingState {EE_DragNothing,EE_DragRotate};
static EDraggingState dragging = EE_DragNothing;
/// original mouse position when dragging started
static int x_mouse_orig,y_mouse_orig;
/// original rotation when draggig started
static GLfloat rot_x_orig,rot_y_orig;

/// Set the projection matrix and viewport
void myReshape(int width, int height)
{
  glViewport(0, 0, width, height);
  // projection transformation
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective (nffstore.angle, (GLdouble)width/(GLdouble)height, 1.0, 1800.0);

  // modelview transformation
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(
    nffstore.from.x,
    nffstore.from.y,
    nffstore.from.z,
    nffstore.at.x,
    nffstore.at.y,
    nffstore.at.z,
    nffstore.up.x,
    nffstore.up.y,
    nffstore.up.z
    );
}

/// mouse dragging - rotate
void myMouseDrag(int x,int y)
{
  if ( dragging == EE_DragRotate )
  {
    int dx=x-x_mouse_orig;
    int dy=y-y_mouse_orig;
    int viewport_w=glutGet((GLenum)GLUT_WINDOW_WIDTH);
    int viewport_h=glutGet((GLenum)GLUT_WINDOW_HEIGHT);
    xr=rot_x_orig+((GLfloat)dx/(GLfloat)viewport_w)*rfactor;
    yr=rot_y_orig+((GLfloat)dy/(GLfloat)viewport_h)*rfactor;
  }
  glutPostRedisplay();
}

/// button press - start rotation
void myMouse(int button, int state, int x, int y)
{
  if( state == GLUT_UP )
  {
    dragging = EE_DragNothing;
    return;
  }

  x_mouse_orig=x;
  y_mouse_orig=y;

  if ( button == GLUT_LEFT_BUTTON )
  {
    dragging = EE_DragRotate;
    rot_x_orig = xr;
    rot_y_orig = yr;
  }
}

/// draw coordinate axes from -1 to 1
void drawAxes()
{
  glBegin(GL_LINES);
  glVertex3f(-1.0,0.0,0.0); glVertex3f(1.0,0.0,0.0);
  glVertex3f(0.0,-1.0,0.0); glVertex3f(0.0,1.0,0.0);
  glVertex3f(0.0,0.0,-1.0); glVertex3f(0.0,0.0,1.0);
  glEnd();
}

/// set up OpenGL
void myInit(void)
{
  GLint two_side[]={1};
  /// background color
  glClearColor (0, 0, 0, 1.0);

  /// light applied from both sides
  glLightModeliv(GL_LIGHT_MODEL_TWO_SIDE, two_side);

//  glPolygonMode( GL_FRONT, GL_LINE );
//  glPolygonMode( GL_BACK,  GL_POINT );

  /// turn on lighting
  glDisable(GL_LIGHTING);
  /// turn on z-buffering
  glDepthFunc(GL_LEQUAL);
  glEnable(GL_DEPTH_TEST);
  /// gouraud shading
  glShadeModel(GL_SMOOTH);
}

/// draw the scene
void myDraw()
{
  /// clear window and z-buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /// set the modelview tranform
  glPushMatrix();

  glRotatef(yr, 1.,0.,0.);
  glRotatef(xr, 0.,1.,0.);

  glColor3f(1.0,0.0,1.0);
  drawAxes();

  //float s = 0.3;
  //glScalef(s,s,s);

  /// render the NFF geometry
  glBegin(GL_TRIANGLES);
  NFFStore::TMatGroupList::const_iterator giter = nffstore.matgroups.begin();
  for ( ; giter != nffstore.matgroups.end(); ++giter) {
    const NFFStore::Material &m = giter->material;
    glColor3f(m.col.r*m.kd, m.col.g*m.kd, m.col.b*m.kd );
    const NFFStore::TriangleList &tlist = giter->geometry;
    NFFStore::TriangleList::const_iterator titer = tlist.begin();
    for ( ; titer != tlist.end(); ++titer ) {
      const NFFStore::Triangle &t = *titer;
      for(int i=0; i<3; i++)
        glVertex3f(t.vertices[i].x,t.vertices[i].y,t.vertices[i].z);
    }
  }
  glEnd();

  glPopMatrix();

  glFlush();
  glutSwapBuffers ();
}


void myKeyboardFunc (unsigned char key, int x, int y)
{
   switch (key) {
    case 27: case 'q':  case 'Q':  exit(0);
   }
   glutPostRedisplay();
}

int main (int argc,char **argv)
{
  if(argc<2) {
    fprintf(stderr,"Usage: nffview scene.nff\n");
    return 2;
  }

  // read the NFF file specified on the cmmand line
  FILE *f = fopen(argv[1],"rt");
  if(!f) {
    fprintf(stderr,"Could not open %s for reading.\n",argv[1]);
    return 2;
  }

  char errstring[4000];
  if( ReadNFF(f,errstring,&nffstore) < 0 ) {
    fprintf(stderr,"Error in NFF file %s:\n",argv[1],errstring);
    return 2;
  }

  fprintf(stderr,"NFF file %s successfully parsed.\n",argv[1]);

  // init glut
  GLuint window;
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
  glutInitWindowSize(nffstore.width,nffstore.height);
  glutInitWindowPosition(50,50);
  window=glutCreateWindow("nff view");
  myInit();
  glutReshapeFunc(myReshape);
  glutDisplayFunc(myDraw);
  glutMotionFunc (myMouseDrag);
  glutMouseFunc  (myMouse);
  glutKeyboardFunc(myKeyboardFunc);

  // run the app
  glutMainLoop();
}
