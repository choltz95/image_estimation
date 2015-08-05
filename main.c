#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>
#include "render.h"
/* ARGSUSED1 */
static void Key(unsigned char key, int x, int y) {
  switch (key) {
  case 'z':
    zoom /= 0.75;
    glutPostRedisplay();
    break;
  case 'Z':
    zoom *= 0.75;
    glutPostRedisplay();
    break;
  case 'f':
    DoFeedback();
    glutPostRedisplay();
    break;
  case 'l':
    linePoly = !linePoly;
    if (linePoly) {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    glutPostRedisplay();
    break;
  case 27:
    exit(0);
  }
}

/* ARGSUSED1 */
static void SpecialKey(int key, int x, int y) {
  switch (key) {
  case GLUT_KEY_LEFT:
    zRotation += 0.5;
    glutPostRedisplay();
    break;
  case GLUT_KEY_RIGHT:
    zRotation -= 0.5;
    glutPostRedisplay();
    break;
  }
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutCreateWindow("Select Test");
  Init();
  glutReshapeFunc(Reshape);
  glutKeyboardFunc(Key);
  glutSpecialFunc(SpecialKey);
  glutMouseFunc(Mouse);
  glutDisplayFunc(Draw);
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}