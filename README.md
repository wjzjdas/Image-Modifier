# Description:

Including source code for a brightness modifier, as well as a seam remover. 
> Seam removing is a content-aware image resizing technique written in C, where the image is reduced in size by one pixel of width (or height) at a time.

# How to Use:

 || Changing Brightness ||

1. Import image.bin
2. In change_brightness.c, Modify char filename[] = "image.bin";
3. Modify change_brightness(im, brightness_by_percentage);
4. convert image_new.bin at https://constructor-s.github.io/esc190_bin_image/

 || Seam Removal ||

 1. Import image.bin
 2. In seamcarving.c, Modify read_in_img(&im, "image.bin");
 3. Modify i value in for(int i = 1; i <= 1000; i++) to remove i number of seams
 4. convert image_new.bin at https://constructor-s.github.io/esc190_bin_image/
