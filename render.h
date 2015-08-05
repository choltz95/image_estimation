#ifndef _RENDER_H_
#define _RENDER_H_

#include <GL/glut.h>


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

static void InitObjects(GLint num);
void Init(void);
void Reshape(int width, int height);
static void Render(GLenum mode);
static GLint DoSelect(GLint x, GLint y);
static void RecolorTri(GLint h);
static void DeleteTri(GLint h);
static void GrowTri(GLint h);
void Mouse(int button, int state, int mouseX, int mouseY);
void Draw(void);
static void DumpFeedbackVert(GLint * i, GLint n);
static void DrawFeedback(GLint n);
void DoFeedback(void);

#endif