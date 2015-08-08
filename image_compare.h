#ifndef _IMAGE_COMPARE_H_
#define _IMAGE_COMPARE_H_
#include <png.h>

extern float fitness;

void cberror(const char * s, ...);
png_bytep*  read_png_file(char* file_name);
int compare_images(char* img1, char* img2);

#endif