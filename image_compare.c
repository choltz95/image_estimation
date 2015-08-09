/*
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}

int x, y;

png_structp png_ptr;
png_infop info_ptr;
png_infop end_info;
int number_of_passes;
int width, height;

//png_bytep* row_pointers;

png_bytep* row_pointers_1;
png_bytep* row_pointers_2;


png_bytep* read_png_file(char* file_name, png_bytep* row_pointers) {

    png_byte color_type = NULL;
    png_byte bit_depth = NULL;

    png_ptr = NULL;
    info_ptr = NULL;
    end_info = NULL;
    number_of_passes = 0;
    width = 0; 
    height = 0;

    char header[8]; // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp)
        abort_("[read_png_file] File %s could not be opened for reading", file_name);
    fread(header, 1, 8, fp);
//    if (png_sig_cmp(header, 0, 8))
//        abort_("[read_png_file] File %s is not recognized as a PNG file", file_name);


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
        abort_("[read_png_file] png_create_read_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
        abort_("[read_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during init_io");

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);

    number_of_passes = png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
        abort_("[read_png_file] Error during read_image");

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++)
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    png_read_image(png_ptr, row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr,
       &end_info);

    fclose(fp);
    return row_pointers;
}


int compare_images(char* fname1, char* fname2) {
    int h1, w1, h2, w2;
    row_pointers_1 = read_png_file(fname1, row_pointers_1);
    h1 = height;
    w1 = width;
    row_pointers_2 = read_png_file(fname2, row_pointers_2);
    h2 = height;
    w2 = width;

/*
    if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
        abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
               "(lacks the alpha channel)");

    if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
        abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
               PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));
*/
    for (y=0; y<h1; y++) {
        png_byte* row = row_pointers_1[y];
        for (x=0; x<w1; x++) {
            png_byte* ptr = &(row[x*3]);
            printf("Pixel at position [ %d - %d ] has RGB values: %d - %d - %d\n",
                   x, y, ptr[0], ptr[1], ptr[2]);
        }
    
    }

    for (y=0; y<h1; y++) {
        free(row_pointers_1[y]);
    }
    free(row_pointers_1);
    for (y=0; y<h2; y++) {
        free(row_pointers_2[y]);
    }
    free(row_pointers_2);

    return 0;
}
