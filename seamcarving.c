#include "seamcarving.h"

#include <stdio.h>
#include <math.h>

//The function will compute the dual-gradient energy function, and place it in the struct rgb_img *grad.
void calc_energy(struct rgb_img *im, struct rgb_img **grad){
    
    int height = im -> height;
    int width = im -> width;
    create_img(grad, height, width);

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

            int energy =  (int)fmax(fmin(sqrt(delta_x + delta_y)/10, 255), 0);
            
            set_pixel(*grad, y, x, energy, energy, energy);
        }
    }
}

//Define the function dynamic_seam(struct rgb_img *grad, double **best_arr) which allocates and computes the dynamic array *best_arr.
//(*best_arr)[i*width+j] contains the minimum cost of a seam from the top of grad to the point (i,j)
void dynamic_seam(struct rgb_img *grad, double **best_arr){

    int height = grad->height;
    int width = grad->width;
    *best_arr = (double *)malloc(height * width * sizeof(double));

    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if (i == 0) { //First row
                (*best_arr)[i*width + j] = (double)get_pixel(grad, i, j, 0);
            } else {
                (*best_arr)[i*width + j] = (double)get_pixel(grad, i, j, 0);
                if (j == 0) { //Leftmost column
                    (*best_arr)[i*width + j] += fmin((*best_arr)[(i-1)*width + j],(*best_arr)[(i-1)*width + j+1]);
                } else if(j == width-1) { //Rightmost column
                    (*best_arr)[i*width + j] += fmin((*best_arr)[(i-1)*width + j],(*best_arr)[(i-1)*width + j-1]);
                } else {
                    (*best_arr)[i*width + j] += fmin(fmin((*best_arr)[(i-1)*width + j],(*best_arr)[(i-1)*width + j-1]),(*best_arr)[(i-1)*width + j+1]);
                }
            }
        }   
    }
    
}

//This function allocates a path through the minimum seam as defined by the array best.
void recover_path(double *best, int height, int width, int **path){

    *path = (int *)malloc(height*sizeof(int));
    double min;

    for(int i = 0; i < height; i++){
        min = best[i*width];
        for(int j = 0; j < width; j++){
            if (best[i*width + j] < min) {
                min = best[i*width + j];
                (*path)[i] = j;
            }
        }
    }

}


void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    
    int height = src -> height;
    int width = src -> width;
    create_img(dest, height, width-1);

    for (int y = 0; y < height; ++y){
        for (int x = 0; x < width; ++x){
            if (path[y] > x){
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x, r, g, b);
            } else if (path[y] < x){
                int r = get_pixel(src, y, x, 0);
                int g = get_pixel(src, y, x, 1);
                int b = get_pixel(src, y, x, 2);
                set_pixel(*dest, y, x-1, r, g, b);
            }
        }
    }

}

int main(){
    
   
    struct rgb_img *im;
    struct rgb_img *cur_im;
    struct rgb_img *grad;
    double *best;
    int *path;

    read_in_img(&im, "IMG_8358.bin");
    
    for(int i = 1; i <= 1000; i++){
        calc_energy(im,  &grad);
        dynamic_seam(grad, &best);
        recover_path(best, grad->height, grad->width, &path);
        remove_seam(im, &cur_im, path);

        char filename[200] = "IMG_8358_CUR.bin"; 
        printf("Removing Seam #%d\n",i);
        write_img(cur_im, filename);


        destroy_image(im);
        destroy_image(grad);
        free(best);
        free(path);
        im = cur_im;
    }

    destroy_image(im);
        
    return 0;
}