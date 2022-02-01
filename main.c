#include <ctype.h>
#include <limits.h>
#include <math.h>
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

enum scaling_method{X_FACTOR, TO_WIDTH, TO_HEIGHT, ALLOWABLE_DIE};
enum image_file_type{JPG, PNG};
enum dice_color{BW = -1, B = 0, W = 255};

/*
 * Checks if a user provided argument is within range.
 * lower_exclusive: non-zero indicates the lower bound is exclusive
 * upper_exclusive: non-zero indicates the upper bound is exclusive
 */
void check_arg_range (uint32_t flag, double user_arg, int32_t min, int32_t max,
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

/*
 * Keeps a pixel's grayscale value in a valid range, by snapping out-of-range 
 * values to the closest in-range value
 */
void snap_to_range (int32_t *x) {
  if (*x < 0) {
    *x = 0;
  } else if (*x > 255) {
    *x = 255;
  }
  return;
}

/*
 * Returns 1 if the pixel should be colored as part of one of a dice's dot
 *
 * Takes parmeters for the dice resolution, row and column within a dice, 
 * and the dice's value
 */
int is_dot_pixel (int32_t dice_resolution, int32_t i, int32_t j, int32_t dice_value) {
  int32_t dot_radius = dice_resolution / 10;
  double dice_center = dice_resolution * 0.5;
  int32_t x = j;
  int32_t y = dice_resolution - i;
  double dot_x;
  double dot_y;
  
  if (dice_value >= 2) { // 2,3,4,5,6 have these dots
    // dot at 1,-1
    dot_x = dice_center + dice_resolution * 0.25 * 1;
    dot_y = dice_center + dice_resolution * 0.25 * -1;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
    // dot at -1,1
    dot_x = dice_center + dice_resolution * 0.25 * -1;
    dot_y = dice_center + dice_resolution * 0.25 * 1;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
  }
  if (dice_value >= 4) { // 4,5,6 have these dots
    // dot at 1,1
    dot_x = dice_center + dice_resolution * 0.25 * 1;
    dot_y = dice_center + dice_resolution * 0.25 * 1;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
    // dot at -1,-1
    dot_x = dice_center + dice_resolution * 0.25 * -1;
    dot_y = dice_center + dice_resolution * 0.25 * -1;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
  }
  if ( (dice_value % 2) == 1) { // 1,3,5 have this dot
    // dot at 0,0
    dot_x = dice_center;
    dot_y = dice_center;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
  }
  if (dice_value == 6) { // 6 has these dots
    // dot at 1,0
    dot_x = dice_center + dice_resolution * 0.25 * 1;
    dot_y = dice_center;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
    // dot at -1,0
    dot_x = dice_center + dice_resolution * 0.25 * -1;
    dot_y = dice_center;
    if ( pow(x - dot_x, 2) + pow(y - dot_y, 2) < pow(dot_radius, 2) ) {
      return 1;
    }
  }
  return 0;
}

int main(int argc, char *argv[])
{
  unsigned char* input_img;
  unsigned char* resized_img;
  unsigned char* output_img;
  int32_t input_width, input_height, input_channels;
  int32_t resized_width, resized_height;
  int32_t output_width, output_height;
  size_t resized_img_size;
  size_t output_img_size;
  int32_t Y;
  int32_t dice_value;
  uint32_t x_pixel;
  unsigned char *write_p;

  // default options
  char        *input_filepath_ptr = "input.jpg";
  char        *output_filepath_ptr = "output.png";
  enum        scaling_method selected_scaling_method = X_FACTOR;
  uint32_t    scaling_method_selected = 0;
  double      scaling_factor = 0.05;
  int32_t     scaling_limit = 0;
  char        *output_txt_filepath_ptr = "output.txt";
  uint32_t    grayscale_steps = 6;
  enum        dice_color selected_dice_color = B;
  uint32_t    list_output_enabled = 0;
  uint32_t    jpeg_quality = 85;
  enum        image_file_type output_file_type = PNG;
  uint32_t    output_type_selected = 0;
  double      contrast_modifier = 1;
  double      brightness_modifier = 0;
  uint32_t    dice_resolution = 40;
  double      cost_per_dice = -1;
  double      dice_size = -1;
  
  uint32_t c,i;

  // process option flags
  opterr = 0;
  while ((c = getopt (argc, argv, "i:o:x:w:h:m:l:g:j:pc:b:r:d:s:")) != -1) {
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
          check_arg_range(c, scaling_factor, 0, INT_MAX, 1, 0);
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
          check_arg_range(c, (double) scaling_limit, 1, INT_MAX, 0, 0);
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
          check_arg_range(c, (double) scaling_limit, 1, INT_MAX, 0, 0);
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
          check_arg_range(c, (double) scaling_limit, 1, INT_MAX, 0, 0);
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
      case 'g':
        if (strcmp(optarg,"m") == 0) {
          grayscale_steps = 12;
          selected_dice_color = BW;
        } else if (strcmp(optarg,"b") == 0) {
          grayscale_steps = 6;
          selected_dice_color = B;
        } else if (strcmp(optarg,"w") == 0) {
          grayscale_steps = 6;
          selected_dice_color = W;
        } else {
          printf("Invalid argument for option -c.\n");
          return 1;
        }
        break;
      case 'j':
        if (output_type_selected == 0) {
          output_file_type = JPG;
          jpeg_quality = atoi(optarg);
          check_arg_range(c, jpeg_quality, 0, 100, 0, 0);
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
      case 'c':
        contrast_modifier = atof(optarg);
        check_arg_range(c, contrast_modifier, 0, 255, 1, 0);
        break;
      case 'b':
        brightness_modifier = atof(optarg);
        check_arg_range(c, brightness_modifier, -255, 255, 0, 0);
        break;
      case 'r':
        dice_resolution = atoi(optarg);
        check_arg_range(c, dice_resolution, 12, INT_MAX, 0, 0);
        break;
      case 'd':
        cost_per_dice = atof(optarg);
        check_arg_range(c, cost_per_dice, 0, INT_MAX, 0, 0);
        break;
      case 's':
        dice_size = atof(optarg);
        check_arg_range(c, dice_size, 0, INT_MAX, 0, 0);
        break;
      case '?':
        if (optopt == 'i' || optopt == 'o' || optopt == 'x' || optopt == 'w' || 
            optopt == 'h' || optopt == 'm' || optopt == 'l' || optopt == 'g' ||
            optopt == 'j' || optopt == 'c' || optopt == 'b' || optopt == 'r' || 
            optopt == 'd' || optopt == 's' )
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
        return 1;
      default:
        return 1;
    }
  } // end options while-loop

  for (i = optind; i < argc; i++) {
    printf ("Non-option argument %s\n", argv[i]);
  }

  // read input image to memory
  input_img = stbi_load(input_filepath_ptr, &input_width, &input_height, &input_channels, 0);
  if(input_img == NULL) {
      printf("Unable to allocate memory for the input image. Input image may not exist.\n");
      return 1;
  }

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
      resized_height = (int32_t) (sqrt(scaling_limit) * sqrt(input_height) / sqrt(input_width) );
      break;
    default:
      return 1;
  }
  resized_img_size = resized_width * resized_height * input_channels;
  resized_img = (unsigned char*) malloc(resized_img_size);
  if(resized_img == NULL) {
      printf("Unable to allocate memory for the resized image.\n");
      return 1;
  }

  stbir_resize_uint8(input_img, input_width, input_height, 0, resized_img, resized_width, resized_height, 0, input_channels);
  stbi_image_free(input_img);

  output_width = dice_resolution * resized_width;
  output_height = dice_resolution * resized_height;
  output_img_size = output_width * output_height; // grayscale only needs 1 channel
  output_img = (unsigned char*) malloc(output_img_size);
  if(output_img == NULL) {
      printf("Unable to allocate memory for the output image.\n");
      return 1;
  }
  
  // draw dice
  write_p = output_img;
  x_pixel = 0;
  for(unsigned char *read_p = resized_img; read_p != resized_img + resized_img_size; read_p += input_channels) {
    int32_t temp_selected_color = 0;
    // Calculate grayscale value
    // ITU-R Recommendation BT.601 luma calculation
    // https://en.wikipedia.org/wiki/Luma_%28video%29#Rec._601_luma_versus_Rec._709_luma_coefficients
    // same as OpenCV's grayscale conversion algorithm
    // https://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html
    // Y = 0.299 * R + 0.587 * G + 0.114 * B
    Y = (int32_t)((*read_p * 0.299)/*R*/ + 
            (*(read_p + 1) * 0.587)/*G*/ + 
            (*(read_p + 2) * 0.114)/*B*/);

    // Apply contrast and brightness modifiers if nessesary.
    // OpenCV's brightness and contrast adjustments algorithm
    // https://docs.opencv.org/2.4/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
    // f(x) = a(x) + b
    if (contrast_modifier != 1 || brightness_modifier != 0) {
      Y = contrast_modifier * Y + brightness_modifier;
      snap_to_range(&Y);
    }
    dice_value = (int32_t) ceil( ((double) grayscale_steps) * Y / 255);
    if (dice_value == 0) {
      dice_value = 1;
    }
    // handle dice color options
    if (selected_dice_color == BW) {
      if (dice_value >= 7) {
        // invert value for white dice
        dice_value = 13 - dice_value;
        temp_selected_color = 255;
      } else {
        temp_selected_color = 0;
      }
    } else if (selected_dice_color == B || selected_dice_color == W) {
      if (selected_dice_color == W) {
        // invert value for white dice
        dice_value = 7 - dice_value;
      }
      temp_selected_color = selected_dice_color;
    }

    // color one dice
    for (uint32_t j = 0; j < dice_resolution; j++) {
      for (uint32_t i = 0; i < dice_resolution; i++) {
        if (i == 0 || j == 0) {
          // left and top sides get a gray border (boarder between dice)
          *(write_p + j + (i * output_width) ) = 50; 
        } else if (is_dot_pixel(dice_resolution, i, j, dice_value) == 1) { 
          // pixel is part of a dice's dot
            *(write_p + j + (i * output_width) ) = 255 - temp_selected_color;
        } else {
          // otherwise the pixel is set pixel to dice color
          *(write_p + j + (i * output_width) ) = temp_selected_color;
        }
      }
    } // finished coloring one dice

    // update row if we hit the end of our current row
    x_pixel += dice_resolution;
    write_p += dice_resolution;
    if (x_pixel > output_width - dice_resolution) {
      write_p += ((dice_resolution - 1) * output_width);
      x_pixel = 0;
    }
  } // end draw dice coloring for-loop

  // write image to file
  if (output_file_type == PNG) {
    stbi_write_png(output_filepath_ptr, output_width, output_height, 1, output_img, 0);
  } else if (output_file_type == JPG) {
    stbi_write_jpg(output_filepath_ptr, output_width, output_height, 1, output_img, jpeg_quality);
  }
  stbi_image_free(output_img);

  // if list option is enabled write all dice values to txt file
  if (list_output_enabled == 1) {
    FILE *fp;

    fp  = fopen (output_txt_filepath_ptr, "w");
    if(fp == NULL) {
      printf("Error: Text file could not be opened.\n");
    }

    x_pixel = 0;
    for(unsigned char *read_p = resized_img; read_p != resized_img + resized_img_size; read_p += input_channels) {
      // Calculate grayscale value
      // Y = 0.299 * R + 0.587 * G + 0.114 * B
      Y = (int32_t)((*read_p * 0.299)/*R*/ + 
              (*(read_p + 1) * 0.587)/*G*/ + 
              (*(read_p + 2) * 0.114)/*B*/);

      // Apply contrast and brightness modifiers if nessesary.
      // f(x) = a(x) + b
      if (contrast_modifier != 1 || brightness_modifier != 0) {
        Y = contrast_modifier * Y + brightness_modifier;
        snap_to_range(&Y);
      }
      dice_value = (int32_t) ceil( ((double) grayscale_steps) * Y / 255);
      if (dice_value == 0) {
        dice_value = 1;
      }

      // handle dice color options
      if (selected_dice_color == BW) {
        if (dice_value >= 7) {
          // invert value for white dice
          fprintf(fp, "w%d ", 13 - dice_value);
        } else {
          fprintf(fp, "b%d ", dice_value);
        }
      } else if (selected_dice_color == B) {
        fprintf(fp, "%d ", dice_value);
      } else if (selected_dice_color == W) {
        // invert value for white dice
        fprintf(fp, "%d ", 7 - dice_value);
      }

      // newline if we hit the end of our current row
      x_pixel++;
      if (x_pixel == resized_width) {
        fprintf(fp, "\n");
        x_pixel = 0;
      }
    }
    fclose (fp);
  }

  stbi_image_free(resized_img);

  printf("Width in Dice: %9d", resized_width);
  if (dice_size != -1) {
    printf(" (%.2f in.)", dice_size * resized_width);
  }
  printf("\nHeight in Dice: %8d", resized_height);
  if (dice_size != -1) {
    printf(" (%.2f in.)", dice_size * resized_height);
  }
  printf("\n");
  printf("Total Dice: %12d\n", resized_width * resized_height);
  if (cost_per_dice != -1) {
    char cost_str[24];
    sprintf(cost_str, "$%.2f", resized_width * resized_height * cost_per_dice);
    printf("Total Cost: %12s\n",cost_str);
  }
  return 0;
}