#ifndef _RENDER_H_
#define _RENDER_H_
#include <GL/glut.h>

#define UNUSED(expr) do { (void)(expr); } while (0) // silence unused glut variables

#define MAXOBJS 10000
#define MAXSELECT 100
#define MAXFEED 300
#define	SOLID 1
#define	LINE 2
#define	POINT 3

extern float zRotation;
extern float zoom;
extern GLenum linePoly;
struct object {
  float v1[2];
  float v2[2];
  float v3[2];
  float color[4];
} objects[MAXOBJS];

void init(void);
void reshape(int width, int height);
void redraw();
void mouse(int button, int state, int mouseX, int mouseY);
void draw(void);
void DoFeedback(void);
void init_polygons(GLint num);
void mutate();

#endif