#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <GL/glut.h>

#include "render.h"
#include "image_compare.h"

GLint windW = 300, windH = 300;
GLuint selectBuf[MAXSELECT];
GLfloat feedBuf[MAXFEED];
GLint vp[4];
float zRotation = 90.0;
float zoom = 1.0;
GLint objectCount;
GLint numObjects;
GLenum linePoly = GL_FALSE;
int j = 0;

struct object parent_dna;

void init(void) {
  numObjects = 50;
  init_polygons(numObjects);
}

void init_polygons(GLint num) {
  GLint i;
  float x, y;

  if (num > MAXOBJS) {
    num = MAXOBJS;
  }
  if (num < 1) {
    num = 1;
  }
  objectCount = num;

  srand((unsigned int) time(NULL));
  for (i = 0; i < num; i++) {
    x = (rand() % 300) - 150;
    y = (rand() % 300) - 150;

    objects[i].v1[0] = x + (rand() % 50) - 25;
    objects[i].v2[0] = x + (rand() % 50) - 25;
    objects[i].v3[0] = x + (rand() % 50) - 25;
    objects[i].v1[1] = y + (rand() % 50) - 25;
    objects[i].v2[1] = y + (rand() % 50) - 25;
    objects[i].v3[1] = y + (rand() % 50) - 25;
    objects[i].color[0] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[1] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[2] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[3] = (double)rand() / (double)RAND_MAX; // alpha value
  }
}

void reshape(int width, int height) {
  windW = width;
  windH = height;
  // multisampling
  //glEnable(GL_MULTISAMPLE);
  //glutSetOption(GLUT_MULTISAMPLE, 8);
  //glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_MULTISAMPLE);
  // transparency
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  glViewport(0, 0, windW, windH);
  glGetIntegerv(GL_VIEWPORT, vp);
}

static void Render(GLenum mode) {
  GLint i;

  for (i = 0; i < objectCount; i++) {
    if (mode == GL_SELECT) {
      glLoadName(i);
    }
    glColor4fv(objects[i].color);
    glBegin(GL_POLYGON);
    glVertex2fv(objects[i].v1);
    glVertex2fv(objects[i].v2);
    glVertex2fv(objects[i].v3);
    glEnd();
  }
}

static GLint DoSelect(GLint x, GLint y) {
  GLint hits;

  glSelectBuffer(MAXSELECT, selectBuf);
  glRenderMode(GL_SELECT);
  glInitNames();
  glPushName(~0);

  glPushMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPickMatrix(x, windH - y, 4, 4, vp);
  gluOrtho2D(-175, 175, -175, 175);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glScalef(zoom, zoom, zoom);
  glRotatef(zRotation, 0, 0, 1);

  Render(GL_SELECT);

  glPopMatrix();

  hits = glRenderMode(GL_RENDER);
  if (hits <= 0) {
    return -1;
  }
  return selectBuf[(hits - 1) * 4 + 3];
}

static void recolor_poly(GLint h) {
  objects[h].color[0] = ((rand() % 100) + 50) / 150.0;
  objects[h].color[1] = ((rand() % 100) + 50) / 150.0;
  objects[h].color[2] = ((rand() % 100) + 50) / 150.0;
  objects[h].color[3] = (double)rand() / (double)RAND_MAX; // alpha value
}

static void resize_poly(GLint h) {
  float x = (rand() % 300) - 150;
  float y = (rand() % 300) - 150;
  objects[h].v1[0] = x + (rand() % 50) - 25;
  objects[h].v2[0] = x + (rand() % 50) - 25;
  objects[h].v3[0] = x + (rand() % 50) - 25;
  objects[h].v1[1] = y + (rand() % 50) - 25;
  objects[h].v2[1] = y + (rand() % 50) - 25;
  objects[h].v3[1] = y + (rand() % 50) - 25;
}

void redraw() {
  GLint i;
  float x, y;
  for (i = 0; i < numObjects; i++) {
    x = (rand() % 300) - 150;
    y = (rand() % 300) - 150;

    objects[i].v1[0] = x + (rand() % 50) - 25;
    objects[i].v2[0] = x + (rand() % 50) - 25;
    objects[i].v3[0] = x + (rand() % 50) - 25;
    objects[i].v1[1] = y + (rand() % 50) - 25;
    objects[i].v2[1] = y + (rand() % 50) - 25;
    objects[i].v3[1] = y + (rand() % 50) - 25;
    objects[i].color[0] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[1] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[2] = ((rand() % 100) + 50) / 150.0;
    objects[i].color[3] = (double)rand() / (double)RAND_MAX; // alpha value
  }
}

static void delete_poly(GLint h) {
  objects[h] = objects[objectCount - 1];
  objectCount--;
}

void mouse(int button, int state, int mouseX, int mouseY) {
  GLint hit;

  if (state == GLUT_DOWN) {
    hit = DoSelect((GLint) mouseX, (GLint) mouseY);
    if (hit != -1) {
      if (button == GLUT_LEFT_BUTTON) {
        recolor_poly(hit);
      } else if (button == GLUT_MIDDLE_BUTTON) {
        resize_poly(hit);
      } else if (button == GLUT_RIGHT_BUTTON) {
        delete_poly(hit);
      }
      glutPostRedisplay();
    }
    else {
      for (j=0; j<25; j++) { // temporary for debugging
        int random_poly = rand() % numObjects;
        //memcpy(&objects[random_poly], &parent_dna, sizeof(objects[random_poly]));
        parent_dna.v1[0] =  objects[random_poly].v1[0];
        parent_dna.v2[0] =  objects[random_poly].v2[0];
        parent_dna.v3[0] =  objects[random_poly].v3[0];
        parent_dna.v1[1] =  objects[random_poly].v1[1];
        parent_dna.v2[1] =  objects[random_poly].v2[1];
        parent_dna.v3[1] =  objects[random_poly].v3[1];
        parent_dna.color[0] =  objects[random_poly].color[0];
        parent_dna.color[1] =  objects[random_poly].color[1];
        parent_dna.color[2] =  objects[random_poly].color[2];
        parent_dna.color[3] =  objects[random_poly].color[3];

        recolor_poly(random_poly);
        resize_poly(random_poly);

        draw();
        system("scrot -u current_out.png"); // screenshot current image
        if (compare_images("current_out.png","mona.png") == 0) {
          //memcpy(&parent_dna, &objects[random_poly], sizeof(parent_dna));
          objects[random_poly].v1[0] = parent_dna.v1[0]; 
          objects[random_poly].v2[0] = parent_dna.v2[0];   
          objects[random_poly].v3[0] = parent_dna.v3[0];   
          objects[random_poly].v1[1] = parent_dna.v1[1];   
          objects[random_poly].v2[1] = parent_dna.v2[1];   
          objects[random_poly].v3[1] = parent_dna.v3[1];   
          objects[random_poly].color[0] = parent_dna.color[0];   
          objects[random_poly].color[1] = parent_dna.color[1];   
          objects[random_poly].color[2] = parent_dna.color[2];   
          objects[random_poly].color[3] = parent_dna.color[3];   
        }
        draw();
      }
    }
  }
}

void draw(void) {
  glPushMatrix();
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-175, 175, -175, 175);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glScalef(zoom, zoom, zoom);
  glRotatef(zRotation, 0, 0, 1);
  Render(GL_RENDER);
  glPopMatrix();
  glutSwapBuffers();
}

static void DumpFeedbackVert(GLint * i, GLint n) {
  GLint index;

  index = *i;
  if (index + 7 > n) {
    *i = n;
    printf("  ???\n");
    return;
  }
  printf("  (%g %g %g), color = (%4.2f %4.2f %4.2f)\n",
    feedBuf[index],
    feedBuf[index + 1],
    feedBuf[index + 2],
    feedBuf[index + 3],
    feedBuf[index + 4],
    feedBuf[index + 5]);
  index += 7;
  *i = index;
}

static void DrawFeedback(GLint n) {
  GLint i;
  GLint verts;

  printf("Feedback results (%d floats):\n", n);
  for (i = 0; i < n; i++) {
    switch ((GLint) feedBuf[i]) {
    case GL_POLYGON_TOKEN:
      printf("Polygon");
      i++;
      if (i < n) {
        verts = (GLint) feedBuf[i];
        i++;
        printf(": %d vertices", verts);
      } else {
        verts = 0;
      }
      printf("\n");
      while (verts) {
        DumpFeedbackVert(&i, n);
        verts--;
      }
      i--;
      break;
    case GL_LINE_TOKEN:
      printf("Line:\n");
      i++;
      DumpFeedbackVert(&i, n);
      DumpFeedbackVert(&i, n);
      i--;
      break;
    case GL_LINE_RESET_TOKEN:
      printf("Line Reset:\n");
      i++;
      DumpFeedbackVert(&i, n);
      DumpFeedbackVert(&i, n);
      i--;
      break;
    default:
      printf("%9.2f\n", feedBuf[i]);
      break;
    }
  }
  if (i == MAXFEED) {
    printf("...\n");
  }
  printf("\n");
}

void DoFeedback(void) {
  GLint x;

  glFeedbackBuffer(MAXFEED, GL_3D_COLOR, feedBuf);
  (void) glRenderMode(GL_FEEDBACK);

  glPushMatrix();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(-175, 175, -175, 175);
  glMatrixMode(GL_MODELVIEW);

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);

  glScalef(zoom, zoom, zoom);
  glRotatef(zRotation, 0, 0, 1);

  Render(GL_FEEDBACK);

  glPopMatrix();

  x = glRenderMode(GL_RENDER);
  if (x == -1) {
    x = MAXFEED;
  }
  DrawFeedback((GLint) x);
}