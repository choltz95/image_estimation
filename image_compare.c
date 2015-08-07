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

png_structp png_ptr;
png_infop info_ptr;
int number_of_passes;
png_bytep * img1_row_pointers;
png_bytep * img2_row_pointers;
png_bytep * row_pointers;

void read_png_file(char* file_name) {
    char header[8]; // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp) {
        cberror("[read_png_file] File %s could not be opened for reading", file_name);
    }
    fread(header, 1, 8, fp);
    if (png_sig_cmp(header, 0, 8)) {
        cberror("[read_png_file] File %s is not recognized as a PNG file", file_name);
    }


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
}

png_bytep* process_file(void) {
    png_bytep * pixels;
    if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
        cberror("[process_file] input file is PNG_COLOR_TYPE_RGBA but must be PNG_COLOR_TYPE_RGB "
                "(lacks the alpha channel)");
    }
    if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGB) {
        cberror("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGB (%d) (is %d)",
                PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));
    }
    pixels = row_pointers;
/*    for (y=0; y<height; y++) {
        row = row_pointers[y];
        for (x=0; x<width; x++) {
            png_byte* ptr = &(row[x*3]);
            printf("Pixel at position [ %d - %d ] has RGB values: %d - %d - %d\n",
            x, y, ptr[0], ptr[1], ptr[2]);
        }
    }
*/
    return pixels;
}

int compare_images(char* img1, char* img2) { // take monalisa as second arg
    int fitness;
    int pixel_fitness;
    png_bytep * image1;
    png_bytep * image2;
    read_png_file(img1);
    image1 = process_file();
    read_png_file(img2);
    image2 = process_file();

    png_byte * row1;
    png_byte * row2;

    for (y=0; y<height; y++) {
        row1 = image1[y];
        row2 = image2[y];
        for (x=0; x<width; x++) {
            png_byte* ptr1 = &(row1[x*3]);
            png_byte* ptr2 = &(row2[x*3]);
            printf("Difference in pixel color at position [ %d - %d ] is: %d - %d - %d\n",
            x, y, ptr1[0]-ptr2[0], ptr1[1]-ptr2[1], ptr1[2]-ptr2[2]);
            pixel_fitness = (ptr1[0]-ptr2[0])*(ptr1[0]-ptr2[0])+(ptr1[1]-ptr2[1])*(ptr1[1]-ptr2[1])+(ptr1[2]-ptr2[2])*(ptr1[2]-ptr2[2]);
            fitness += pixel_fitness;
        }
    }

    return fitness;
}