#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

void cberror(const char * s, ...) {
    va_list args;
    va_start(args, s);
    vfprintf(stderr, s, args);
    fprintf(stderr, "\n");
    va_end(args);
    abort();
}

int x, y;
int width, height;
png_byte color_type;
png_byte bit_depth;
float fitness = 10000000000;

png_structp png_ptr;
png_infop info_ptr;
png_infop end_info;
int number_of_passes;
png_bytep * img1_row_pointers;
png_bytep * img2_row_pointers;
png_bytep * row_pointers;

png_bytep*  read_png_file(char* file_name) {
    char header[8]; // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        cberror("[read_png_file] File %s could not be opened for reading", file_name);
    }
    fread(header, 1, 8, fp);
/*    if (png_sig_cmp(header, 0, 8)) {
        cberror("[read_png_file] File %s is not recognized as a PNG file", file_name);
    }*/


    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr) {
        cberror("[read_png_file] png_create_read_struct failed");
    }
    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        cberror("[read_png_file] png_create_info_struct failed");
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        cberror("[read_png_file] Error during init_io");
    }
    end_info = png_create_info_struct(png_ptr);

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
    if (setjmp(png_jmpbuf(png_ptr))) {
        cberror("[read_png_file] Error during read_image");
    }

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++) {
        row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
    }

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
    return row_pointers;
}

int compare_images(char* img1, char* img2) { // take monalisa as second arg
    float image_fitness = 0;
    float pixel_fitness = 0;
    png_bytep * image1;
    png_bytep * image2;

    image1 =read_png_file(img1);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    /* cleanup heap allocation */
    for (y=0; y<height; y++) {
            free(row_pointers[y]);
    }
    free(row_pointers);

    image2=read_png_file(img2);
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
       /* cleanup heap allocation */
    for (y=0; y<height; y++) {
            free(row_pointers[y]);
    }
    free(row_pointers);


    png_byte * row1;
    png_byte * row2;


    for (y=0; y<height; y++) {
        row1 = image1[y];
        row2 = image2[y];
        for (x=0; x<width; x++) {
            png_byte* ptr1 = &(row1[x*3]);
            png_byte* ptr2 = &(row2[x*3]);
            //printf("Difference in pixel color at position [ %d - %d ] is: %d - %d - %d\n",
            //x, y, ptr1[0]-ptr2[0], ptr1[1]-ptr2[1], ptr1[2]-ptr2[2]);
            pixel_fitness = (ptr1[0]-ptr2[0])*(ptr1[0]-ptr2[0])+(ptr1[1]-ptr2[1])*(ptr1[1]-ptr2[1])+(ptr1[2]-ptr2[2])*(ptr1[2]-ptr2[2]);
            image_fitness += pixel_fitness;
        }
    }

    if(fitness > image_fitness) {
        printf("%f --- %f \n",image_fitness, fitness);
        fitness = image_fitness;
        return 1;
    } else {
        return 0;
    }
}