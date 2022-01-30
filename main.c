#include <ctype.h>
#include <limits.h>
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

/*
 * Checks if a user provided argument is within range.
 * lower_exclusive: non-zero indicates the lower bound is exclusive
 * upper_exclusive: non-zero indicates the upper bound is exclusive
 */
void check_arg_range (char flag, double user_arg, int32_t min, int32_t max,
                      int32_t lower_exclusive, int32_t upper_exclusive) {
  if ( (user_arg < min && lower_exclusive == 0) || 
       (user_arg > max && upper_exclusive == 0) ) {
    printf ("Option -%c requires a value in range [%d,%d].\n", flag, min, max);
    exit(1);
  } else if ( (user_arg <= min && lower_exclusive != 0) || 
              (user_arg >  max && upper_exclusive == 0) ) {
    printf ("Option -%c requires a value in range (%d,%d].\n", flag, min, max);
    exit(1);
  } else if ( (user_arg <  min && lower_exclusive == 0) || 
              (user_arg >= max && upper_exclusive != 0) ) {
    printf ("Option -%c requires a value in range [%d,%d).\n", flag, min, max);
    exit(1);
  } else if ( (user_arg <= min && lower_exclusive != 0) ||
              (user_arg >= max && upper_exclusive != 0) ) {
    printf ("Option -%c requires a value in range (%d,%d).\n", flag, min, max);
    exit(1);
  }
  return;
}

enum scaling_method{X_FACTOR, TO_WIDTH, TO_HEIGHT, ALLOWABLE_DIE};
enum image_file_type{JPG, PNG};

int main(int argc, char *argv[])
{
	unsigned char* input_img;
	unsigned char* resized_img;
  //unsigned char* output_img;
	int32_t input_width, input_height, input_channels;
	int32_t resized_width, resized_height;
  size_t resized_img_size;
  uint8_t Y;

  // options
  char        *input_filepath_ptr = "input.jpg";
  char        *output_filepath_ptr = "output.jpg";
  enum        scaling_method selected_scaling_method = X_FACTOR;
  uint8_t     scaling_method_selected = 0;
  double      scaling_factor = 0.05;
  int32_t    scaling_limit = 0;
  char        *output_txt_filepath_ptr = "output.txt";
  uint8_t     list_output_enabled = 0;
  uint8_t     invert_colors = 0;
  uint8_t     jpeg_quality = 85;
  enum        image_file_type output_file_type = JPG;
  uint8_t     output_type_selected = 0;

  uint32_t c,i;
 
  // process option flags
  opterr = 0;
  while ((c = getopt (argc, argv, "i:o:x:w:h:m:l:c:fj:p")) != -1) {
    switch (c) {
      case 'i':
        input_filepath_ptr = optarg;
        break;
      case 'o':
        output_filepath_ptr = optarg;
        break;
      case 'x':
        if (scaling_method_selected == 0) {
          selected_scaling_method = X_FACTOR;
          scaling_factor = atof(optarg);
          check_arg_range('x', scaling_factor, 0, INT_MAX, 1, 0);
          scaling_method_selected = 1;
        } else {
          printf("Too many scaling options.\n");
          return 1;
        }
        break;
      case 'w':
        if (scaling_method_selected == 0) {
          selected_scaling_method = TO_WIDTH;
          scaling_limit = atoi(optarg);
          check_arg_range('w', (double) scaling_limit, 1, INT_MAX, 0, 0);
          scaling_method_selected = 1;
        } else {
          printf("Too many scaling options.\n");
          return 1;
        }
        break;
      case 'h':
        if (scaling_method_selected == 0) {
          selected_scaling_method = TO_HEIGHT;
          scaling_limit = atoi(optarg);
          check_arg_range('h', (double) scaling_limit, 1, INT_MAX, 0, 0);
          scaling_method_selected = 1;
        } else {
          printf("Too many scaling options.\n");
          return 1;
        }
        break;
      case 'm':
        if (scaling_method_selected == 0) {
          selected_scaling_method = ALLOWABLE_DIE;
          scaling_limit = atoi(optarg);
          check_arg_range('m', (double) scaling_limit, 1, INT_MAX, 0, 0);
          scaling_method_selected = 1;
        } else {
          printf("Too many scaling options.\n");
          return 1;
        }
        break;
      case 'l':
        output_txt_filepath_ptr = optarg;
        list_output_enabled = 1;
        break;
      case 'c':
        if (strcmp(optarg,"m") == 0) {
          // TODO
        } else if (strcmp(optarg,"b") == 0) {
          // TODO
        } else if (strcmp(optarg,"w") == 0) {
          // TODO
        } else {
          printf("Invalid argument for option -c.\n");
          return 1;
        }
        break;
      case 'f':
        invert_colors = 1;
        break;
      case 'j':
        if (output_type_selected == 0) {
          output_file_type = JPG;
          jpeg_quality = atoi(optarg);
          check_arg_range('j', jpeg_quality, 0, 100, 0, 0);
          output_type_selected = 1;
        } else {
          printf("Conflicting output file type options. -j conflicts with -p. \n");
          return 1;
        }
        break;
      case 'p':
        if (output_type_selected == 0) {
          output_file_type = PNG;
          output_type_selected = 1;
        } else {
          printf("Conflicting output file type options. -p conflicts with -j. \n");
          return 1;
        }
        break;
      case '?':
        if (optopt == 'i' || optopt == 'o' || optopt == 'x' || optopt == 'w' || 
            optopt == 'h' || optopt == 'm' || optopt == 'l' || optopt == 'c')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        return 1;
    }
  } // end while-loop

  for (i = optind; i < argc; i++) {
    printf ("Non-option argument %s\n", argv[i]);
  }

  //debug
  printf ("input_filepath = %s\n", input_filepath_ptr);
  printf ("output_filepath = %s\n", output_filepath_ptr);
  printf ("scaling_factor = %f\n", scaling_factor);
  printf ("scaling_limit = %d\n", scaling_limit);
  

  //debug
  //struct timeval start, end;
  //double time_taken = 0;

  // read input image to memory
	input_img = stbi_load(input_filepath_ptr, &input_width, &input_height, &input_channels, 0);

  // allocate memory for resized image
  switch (selected_scaling_method) {
    case X_FACTOR:
      resized_width = ceil(input_width * scaling_factor);
      resized_height = ceil(input_height * scaling_factor);
      break;
    case TO_WIDTH:
      resized_width = scaling_limit;
      resized_height = ceil( ((double) input_height) / input_width * scaling_limit);
      break;
    case TO_HEIGHT:
      resized_height = scaling_limit;
      resized_width = ceil( ((double) input_width) / input_height * scaling_limit);
      break;
    case ALLOWABLE_DIE:
      resized_width = (int32_t) (sqrt(scaling_limit) * sqrt(input_width) / sqrt(input_height) );
      resized_height = (uint32_t) (sqrt(scaling_limit) * sqrt(input_height) / sqrt(input_width) );
      break;
    default:
      return 1;
  }

  resized_img_size = resized_width * resized_height * input_channels;
	resized_img = (unsigned char*) malloc(resized_img_size);
  if(resized_img == NULL) {
      printf("Unable to allocate memory for the output image.\n");
      return 1;
  }

	stbir_resize_uint8(input_img, input_width, input_height, 0, resized_img, resized_width, resized_height, 0, input_channels);
  stbi_image_free(input_img);

  // Calculate greyscale values
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

  if ()
  //stbi_write_png(output_filepath_ptr, resized_width, resized_height, input_channels, resized_img, 0);
  stbi_write_jpg(output_filepath_ptr, resized_width, resized_height, input_channels, resized_img, jpeg_quality);
  //stbi_write_jpg("output_gray.jpg", out_width, out_height, gray_channels, grayscale_pixels, 85);

  stbi_image_free(resized_img);

return 0;
}

