#include "adpalette.h"


static void QuickSort(adUint32* a_value, adUint32* a_item, adUint32 a_start, adUint32 a_end, adUint32 a_pivot)
{

  adUint32 center = 0;
  for(int i = a_start; i < a_end; i++)
  {
    if(a_value[i] > a_pivot)
  }
}


static void PaletteByPopularity(const AdImage* a_image, adUint32* a_palette,
                         adUint32* a_count, adUint32 a_colorCount)
{
  adUint32 *colors = malloc(1<<24);
  for(int i = 0; i < (1<<24); i++)
  {
    colors[i] = i;
  }





  free(colors);
}

void adCreatePalette(const AdImage* a_image, adUint32* a_palette)
{
  adUint32* count = malloc(1<<24);
  memset(count, 0, 1<<24);

  adUint32 colorCount = 0;

  const adUint32 pixels = a_image->pixels;

  if(a_image->width == a_image->pitch)
  {
    for(int i = 0; i < a_image->width * a_image->height; i++)
    {
      adUint32 c = count[0xffffff & pixels[i]];
      colorCount += !c; //only increment if count of that color is 0
      count[0xffffff & pixels[i]]++;
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
      for(int i = 0; i < a_image->width * a_image->height; i++)
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
