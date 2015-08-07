#ifndef _IMAGE_COMPARE_H_
#define _IMAGE_COMPARE_H_
#include <png.h>

void cberror(const char * s, ...);
void read_png_file(char* file_name);
png_bytep* process_file(void);
int compare_images(char* img1, char* img2);

#endif