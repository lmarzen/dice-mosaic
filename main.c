#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

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
	unsigned char* input_img;
	unsigned char* resized_img;
  //unsigned char* output_img;
	int input_width, input_height, input_channels;
	int resized_width, resized_height;
  int Y;

  // default options
  char *input_filepath_ptr = "input.jpg";
  char *output_filepath_ptr = "output.jpg";

  int index;
  int c;

  // process option flags
  opterr = 0;
  while ((c = getopt (argc, argv, "i:o:s::")) != -1) {
    switch (c)
      {
      case 'i':
        input_filepath_ptr = optarg;
        break;
      case 'o':
        output_filepath_ptr = optarg;
        break;
      case 's':
        cvalue = optarg;
        break;
      case 'w':
        cvalue = optarg;
        break;
      case 'h':
        cvalue = optarg;
        break;
      case '?':
        if (optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        return 1;
      default:
        return 1;
      }
  }

  for (index = optind; index < argc; index++) {
    printf ("Non-option argument %s\n", argv[index]);
  }

  //debug
  printf ("input_filepath = %s\n", input_filepath_ptr);
  printf ("output_filepath = %s\n", output_filepath_ptr);
  

  //debug
  //struct timeval start, end;
  //double time_taken = 0;

  // read input image to memory
	input_img = stbi_load(input_filepath_ptr, &input_width, &input_height, &input_channels, 0);

  // allocate memory for resized image
	resized_width = input_width/20;
	resized_height = input_height/20;
  size_t resized_img_size = resized_width * resized_height * input_channels;
	resized_img = (unsigned char*) malloc(resized_img_size);
  if(resized_img == NULL) {
      printf("Unable to allocate memory for the output image.\n");
      return 1;
  }

	stbir_resize_uint8(input_img, input_width, input_height, 0, resized_img, resized_width, resized_height, 0, input_channels);
  stbi_image_free(input_img);

  // Convert to greyscale
  // ITU-R Recommendation BT.601 luma calculation
  // https://en.wikipedia.org/wiki/Luma_%28video%29#Rec._601_luma_versus_Rec._709_luma_coefficients
  // same as OpenCV's grayscale conversion algorithm
  // https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
  // Y = 0.299 * R + 0.587 * G + 0.114 * B
  for(unsigned char *p = resized_img; p != resized_img + resized_img_size; p += input_channels) {
    Y = (uint8_t)((*p * 0.299)/*R*/ + 
            (*(p + 1) * 0.587)/*G*/ + 
            (*(p + 2) * 0.114)/*B*/);
  }
  
  Y = 0;
  printf("%d, ", Y);

  //stbi_write_png("output_color.png", out_width, out_height, channels, output_pixels, 0);
  stbi_write_jpg(output_filepath_ptr, resized_width, resized_height, input_channels, resized_img, 85);
  //stbi_write_jpg("output_gray.jpg", out_width, out_height, gray_channels, grayscale_pixels, 85);

  stbi_image_free(resized_img);

return 0;
}

