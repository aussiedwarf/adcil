#include "adpalette.h"

#include <stdint.h>
#include <assert.h>


static void QuickSort(uint32_t* a_value, uint32_t* a_item, uint32_t a_start, uint32_t a_end, uint32_t a_pivot)
{

  uint32_t center = 0;
  for(int i = a_start; i < a_end; i++)
  {
    //if(a_value[i] > a_pivot)
  }
}


static void PaletteByPopularity(const AdImage* a_image, uint32_t* a_palette,
                         uint32_t* a_count, uint32_t a_colorCount)
{
  uint32_t *colors = malloc(1<<24);
  for(int i = 0; i < (1<<24); i++)
  {
    colors[i] = i;
  }





  free(colors);
}

void adCreatePalette(const AdImage* a_image, uint32_t* a_palette)
{
  uint32_t* count = malloc(1<<24);
  memset(count, 0, 1<<24);

  uint32_t colorCount = 0;

  const uint32_t pixels = a_image->pixels;

  if(a_image->width == a_image->pitch)
  {
    for(uint32_t i = 0; i < a_image->width * a_image->height; i++)
    {
      //uint32_t c = count[0xffffff & pixels[i]];
      //colorCount += !c; //only increment if count of that color is 0
      //count[0xffffff & pixels[i]]++;
    }
  }
  else
  {
    assert(0);
  }

  //quantization phase
  if(colorCount > 256)
  {
    PaletteByPopularity(a_image, a_palette, count, colorCount);
  }
  else
  {
    int index = 0;

    if(a_image->width == a_image->pitch)
    {
      for(uint32_t i = 0; i < a_image->width * a_image->height; i++)
      {
        a_palette[index] = a_image->pixels[i] & 0xffffff;
        index++;
      }
    }
    else
    {
      assert(0);
    }
  }

  free(count);
}
