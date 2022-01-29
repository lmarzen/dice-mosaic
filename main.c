#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"

// debug
#include <sys/time.h>

/*
  struct timeval start, end;
  double time_taken = 0;
  gettimeofday(&start, NULL); // start timer

  gettimeofday(&end, NULL); // stop timer
  time_taken = end.tv_sec + end.tv_usec / 1e6 -
               start.tv_sec - start.tv_usec / 1e6; // in seconds
  printf("program took an average of %f seconds to execute\n", time_taken / 100);
*/

int main(int argc, char *argv[])
{
  //read_png(argv[1]);
  //write_png(argv[2]);

	unsigned char* input_pixels;
	unsigned char* output_pixels;
	int width, height, channels;
	int out_width, out_height;
  
  //debug
  //struct timeval start, end;
  //double time_taken = 0;

  // read input image to memory
	input_pixels = stbi_load(argv[1], &width, &height, &channels, 0);

  // allocate memory for resized image
	out_width = width/20;
	out_height = height/20;
	output_pixels = (unsigned char*) malloc(out_width*out_height*channels);
  if(output_pixels == NULL) {
      printf("Unable to allocate memory for the output image.\n");
      return 1;
  }

	stbir_resize_uint8(input_pixels, width, height, 0, output_pixels, out_width, out_height, 0, channels);

  // Convert the input image to grayscale
  size_t output_pixels_size = out_width * out_height * channels;
  int gray_channels = channels == 4 ? 2 : 1;
  size_t gray_img_size = out_width * out_height * gray_channels;


  unsigned char *grayscale_pixels = malloc(gray_img_size);
  if(grayscale_pixels == NULL) {
      printf("Unable to allocate memory for the grayscale image.\n");
      return 1;
  }

  for(unsigned char *p = output_pixels, *pg = grayscale_pixels; p != output_pixels + output_pixels_size; p += channels, pg += gray_channels) {
    *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    if(channels == 4) {
      *(pg + 1) = *(p + 3);
    }
  }

  stbi_write_jpg("output.jpg", out_width, out_height, gray_channels, grayscale_pixels, 85);

  stbi_image_free(input_pixels);
  stbi_image_free(output_pixels);

/*
  int width, height, channels;
  unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);
  if(img == NULL) {
    printf("Error in loading the image\n");
    exit(1);
  }
  printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

  // Convert the input image to grayscale
  size_t img_size = width * height * channels;
  int gray_channels = channels == 4 ? 2 : 1;
  size_t gray_img_size = width * height * gray_channels;

  unsigned char *gray_img = malloc(gray_img_size);
  if(gray_img == NULL) {
      printf("Unable to allocate memory for the gray image.\n");
      return 1;
  }

  for(unsigned char *p = img, *pg = gray_img; p != img + img_size; p += channels, pg += gray_channels) {
    *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
    if(channels == 4) {
      *(pg + 1) = *(p + 3);
    }
  }

  stbi_write_jpg("./photos/output1.jpg", width, height, gray_channels, gray_img, 80);


  size_t resize_img_size = width * height * gray_channels;
  unsigned char *resize_img = malloc(resize_img_size);

  stbir_resize_uint8(img,        width,   height,   0,
                     resize_img, width, height, 0, 0);
  stbi_write_jpg("./photos/output2.jpg", width/8, height/8, gray_channels, resize_img, 80);                

  stbi_image_free(img);
  stbi_image_free(gray_img);
  stbi_image_free(resize_img);
*/
return 0;
}

