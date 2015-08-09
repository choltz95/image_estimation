#ifndef _IMAGE_COMPARE_H_
#define _IMAGE_COMPARE_H_
#include <png.h>

#define UNUSED(expr) do { (void)(expr); } while (0) // silence unused glut variables

extern float fitness;

void abort_(const char * s, ...);
png_bytep* read_png_file(char* file_name, png_bytep* row_pointers);
int compare_images(char* img1, char* img2);

#endif