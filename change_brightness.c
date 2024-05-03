#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>
#include "change_brightness.h"

void change_brightness(struct rgb_img *im, float f){
    int height = im->height;
    int width = im->width;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Get pixel values
            uint8_t r = get_pixel(im, y, x, 0);
            uint8_t g = get_pixel(im, y, x, 1);
            uint8_t b = get_pixel(im, y, x, 2);

            // Adjust brightness for each color component
            r = (uint8_t)fmax(fmin(r * f, 255), 0);
            g = (uint8_t)fmax(fmin(g * f, 255), 0);
            b = (uint8_t)fmax(fmin(b * f, 255), 0);

            // Set pixel values
            set_pixel(im, y, x, r, g, b);
        }
    }

}


int main(){
    struct rgb_img *im;
    char filename[] = "cannon.bin";

    //read_in_img already allocates space for the image, 
    //therefore, read all rgb data from "cannon.jpg" into im
    read_in_img(&im, filename);

    change_brightness(im, 0.5);

    char filename_new[] = "image_new.bin";
    write_img(im, filename_new);


    return 0;
}
