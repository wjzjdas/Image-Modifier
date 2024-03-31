#include "seamcarving.h"

#include <stdio.h>
#include <math.h>

//The function will compute the dual-gradient energy function, and place it in the struct rgb_img *grad.
void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    int height = im -> height;
    int width = im -> width;


    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            //Acuquire the surrounding cell coordinates, lrud=[left, right, up, down]
            int lrud[] = {x-1, x+1, y-1, y+1};

            if (x == 0){ //Leftmost column
                lrud[0] = width-1;
            } 
            if (x == width-1){ //Rightmost column
                lrud[1] = 0;
            }
            if (y == 0){ //Highest Row
                lrud[2] = height-1;
            }
            if (y == height-1){ //Lowest Row
                lrud[3] = 0;
            }

            //Acquire the RGB Value of surrounding cells = [R, G, B]
            uint8_t up[] = {get_pixel(im, lrud[2], x, 0), get_pixel(im, lrud[2], x, 1), get_pixel(im, lrud[2], x, 2)};
            uint8_t down[] = {get_pixel(im, lrud[3], x, 0), get_pixel(im, lrud[3], x, 1), get_pixel(im, lrud[3], x, 2)};
            uint8_t left[] = {get_pixel(im, y, lrud[0], 0), get_pixel(im, y, lrud[0], 1), get_pixel(im, y, lrud[0], 2)};
            uint8_t right[] = {get_pixel(im, y, lrud[1], 0), get_pixel(im, y, lrud[1], 1), get_pixel(im, y, lrud[1], 2)};

            //Compute the dual-gradient energy function
            double delta_x = pow((double)right[0] - (double)left[0], 2) + pow((double)right[1] - (double)left[1], 2) + pow((double)right[2] - (double)left[2], 2);
            double delta_y = pow((double)up[0] - (double)down[0], 2) + pow((double)up[1] - (double)down[1], 2) + pow((double)up[2] - (double)down[2], 2);

            uint8_t energy =  (uint8_t)fmax(fmin(sqrt(delta_x + delta_y)/10, 255), 0);
            
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){

}

void recover_path(double *best, int height, int width, int **path){

}

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){

}

int main(){

   
    struct rgb_img *im;
    char filename[] = "3x4.bin";
    read_in_img(&im, filename);

    struct rgb_img *grad;
    create_img(&grad, im->height, im->width);
    
    calc_energy(im,  &grad);
    
    print_grad(grad, 0);
    return 0;
}