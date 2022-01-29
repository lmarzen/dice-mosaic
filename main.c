#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

int main(int argc, char *argv[])
{
  //read_png(argv[1]);
  //write_png(argv[2]);

  int width, height, channels;
  unsigned char *img = stbi_load(argv[1], &width, &height, &channels, 0);
  if(img == NULL) {
    printf("Error in loading the image\n");
    exit(1);
  }
  printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);

  stbi_write_jpg("test.jpg", width, height, channels, img, 80);

  stbi_image_free(img);    

return 0;
}

