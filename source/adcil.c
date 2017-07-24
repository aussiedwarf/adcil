#include "../adcil.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

AdImageError adLoadImage(const char *a_file, const int a_filenameLength, AdImage* a_destImage, int a_destFormat)
{
  if(!a_file)
    return AD_IMG_ERR;

  int len = a_filenameLength;
  if(!a_filenameLength)
    len = strlen(a_file);

  FILE* file = NULL;

  file = fopen(a_file, "r");

  if(!file)
    return AD_IMG_ERR;

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);

  fseek(file, 0, SEEK_SET);

  void* data = malloc(size);

  fread(data, size, 1, file);

  adLoadBmpPointer(data, size, a_destImage, a_destFormat);

  free(data);


  fclose(file);


  return AD_IMG_ERR_OK;
}


AdImageError adLoadImagePointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, int a_destFormat)
{
  return AD_IMG_ERR;
}


AdImageError adSaveImage(const char *a_file, const int a_filenameLength, const AdImage* a_image, const void* a_settings)
{
  return AD_IMG_ERR;
}


AdImageError adSaveImagePointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_settings)
{
  return AD_IMG_ERR;
}

