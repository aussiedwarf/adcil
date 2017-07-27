/*

SOurces
http://www.fileformat.info/format/bmp/egff.htm
*/


#include "adbmp.h"
#include "adcil.h"

#include <stdlib.h>
#include <string.h>



#define BMP_HEADER_SIZE 14
#define BMP_WINDOWS_2X_HEADER_SIZE 12
#define BMP_WINDOWS_3X_HEADER_SIZE 40
#define BMP_WINDOWS_4X_HEADER_SIZE 108
#define BMP_WINDOWS_5X_HEADER_SIZE 124

#define Color16ToColor32(x) (((x << 3) & 0xff) | ((x << 5) & 0xff00) | ((x << 8) & 0xff0000))


int CheckFileType(const unsigned char *a_srcImage)
{
  if(a_srcImage[0] == 'B' && a_srcImage[1] == 'M')
    return 1;
  else if(a_srcImage[0] == 'B' && a_srcImage[1] == 'A')
    return 2;
  else return 0;
}


//potential to create short palette in small image and have the image address
//entries that go beyond palette + image into overflow. SHould create safe
//function to handlethese specific images
void LoadBGR8Palette32ToBGRA32(const adUint8* const a_src,
                       const adUint32 a_width,
                       const adUint32 a_height,
                       const adUint32 a_pitchSrc,
                       adUint8* a_dest,
                       const adUint32 a_pitcDest,
                               const adUint8* const a_palette,
                               const int a_paletteLength) //num palette enteries
{
  const uint32_t* palette = a_palette;

  uint32_t *dest = a_dest;

  for(int y = 0; y < a_height; y++)
  {
    for(int x = 0; x < a_width; x++)
    {
      dest[x+y*pitchDest] = a_palette[a_src[x+y*a_pitchSrc]];
    }

  }
}



void LoadBGR16ToBGRA32(const adUint8* const a_src,
                       const adUint32 a_width,
                       const adUint32 a_height,
                       const adUint32 a_pitchSrc,
                       adUint8* a_dest,
                       const adUint32 a_pitcDest)
{
  const uint32_t *src = a_src;
  uint32_t *dest = a_dest;

  int w = a_width & (~1); //get width rounded down
  for(int y = 0; y < a_height; y++)
  {
    int x = 0;
    int w = 0;

    while(x < w)
    {
      *(dest + x+0) = Color16ToColor32(*(src + w)) & | 0xff000000;
      *(dest + x+1) = Color16ToColor32(*(src + w) >> 16) & | 0xff000000;


      x+=2;
      w++;
    }

    if(a_width-w > 0)
    {
      *(dest + x+0) = Color16ToColor32(*(src + w)) & | 0xff000000;
    }

  }
}

void LoadBGR24ToBGRA32(const adUint8* const a_src,
                       const adUint32 a_width,
                       const adUint32 a_height,
                       const adUint32 a_pitchSrc,
                       adUint8* a_dest,
                       const adUint32 a_pitcDest)
{
  const uint32_t *src = a_src;
  uint32_t *dest = a_dest;

  int w = a_width & (~3); //get width rounded down
  for(int y = 0; y < a_height; y++)
  {
    int x = 0;
    int w = 0;

    while(x < w)
    {
      //memcpy(a_dest + a_pitcDest*y, a_src + a_pitchSrc*y, size);
      *(dest + x+0) = *(src + w) & | 0xff000000;
      *(dest + x+1) = (*(src + w) >> 24) | (*(src + w+1) << 8) | 0xff000000;
      *(dest + x+2) = (*(src + w+1) >> 16) | (*(src + w+2) << 16) | 0xff000000;
      *(dest + x+3) = *(src + w+2) >> 8;

      x+=4;
      w+=3;
    }

    if(a_width-w > 0)
    {
      *(dest + x+0) = *(src + w) & | 0xff000000;
      if(a_width-w > 1)
      {
        *(dest + x+1) = (*(src + w) >> 24) | (*(src + w+1) << 8) | 0xff000000;
        if(a_width-w > 2)
        {
          *(dest + x+2) = (*(src + w+1) >> 16) | (*(src + w+2) << 16) | 0xff000000;
        }
      }
    }
  }
}


void LoadBGRX32ToBGRA32(const adUint8* const a_src,
                        const adUint32 a_width,
                        const adUint32 a_height,
                        const adUint32 a_pitchSrc,
                        adUint8* a_dest,
                        const adUint32 a_pitcDest)
{
  if(a_pitchDest == a_pitchSrc)
  {
    size_t size = a_pitchSrc * a_height;
    memcpy(a_dest, a_src, size);
  }
  else
  {
    for(int y = 0; y < a_height; y++)
    {
      memcpy(a_dest + a_pitcDest*y, a_src + a_pitchSrc*y, size);
    }
  }
}

void LoadBGRA32ToBGRA32(const adUint8* const a_src,
                        const adUint32 a_width,
                        const adUint32 a_height,
                        const adUint32 a_pitchSrc,
                        adUint8* a_dest,
                        const adUint32 a_pitcDest)
{
  if(a_pitchDest == a_pitchSrc)
  {
    size_t size = a_pitchSrc * a_height;
    memcpy(a_dest, a_src, size);
  }
  else
  {
    for(int y = 0; y < a_height; y++)
    {
      memcpy(a_dest + a_pitcDest*y, a_src + a_pitchSrc*y, size);
    }
  }
}


AdImageError adLoadBmp(const char *a_file, const int a_filenameLength, AdImage* a_destImage, int a_destFormat)
{
  return AD_IMG_ERR;
}

AdImageError adLoadBmpPointer(const unsigned char *a_srcImage, size_t a_size,
                              AdImage* a_destImage, int a_destFormat)
{


  AdBmpType fileType;
  int headerSize = 0;

  unsigned int bitmapOffset = 0;

  if(!a_destImage)
    return AD_IMG_ERR_BADPARRAM; 
     
  //sanity check
  if(a_size < 14 )
  {
    return AD_IMG_ERR_FILETOOSMALL;
  }
  
  int bmType = CheckFileType(a_srcImage);
  if(!bmType )
    return AD_IMG_ERR_WRONGFILETYPE;
    
  size_t fileSize = (int) *((int*) (a_srcImage + 2));
  
  if(a_size != fileSize)
    return AD_IMG_ERR;

  headerSize = (int)*((int*)(a_srcImage + BMP_HEADER_SIZE));
  bitmapOffset = (int)*((int*)(a_srcImage + BMP_HEADER_SIZE + 10));
  /*
   * Note that the only difference between Windows 2.x BMP and OS/2 1.x BMP is
   * the data type of the Width and Height fields. For Windows 2.x, they are
   * signed shorts and for OS/2 1.x, they are unsigned shorts.
   * */
  switch(headerSize)
  {
    //12-64 is OS2_2x
    case BMP_WINDOWS_2X_HEADER_SIZE:  //or OS2_1x
      fileType = Windows_2x;
      break;
    case BMP_WINDOWS_3X_HEADER_SIZE:
      //compression level 3 is NT
      fileType = Windows_3x;
      break;
    case BMP_WINDOWS_4X_HEADER_SIZE:
      fileType = Windows_4x;
      break;
    case BMP_WINDOWS_5X_HEADER_SIZE:
      fileType = Windows_5x;
    default:
      return AD_IMG_ERR;

  }

  if(fileSize < (size_t)headerSize + BMP_HEADER_SIZE)
    return AD_IMG_ERR;

  AdImageHeaderBmp header;
  memset(&header, 0, sizeof(AdImageHeaderBmp));

  memcpy(&header, a_srcImage + BMP_HEADER_SIZE, headerSize);

  //Work out size of image data taking into account that a scanline must be a multiple of 4 bytes
  adUint32 w = header.width * header.bitsPerPixel;  //in bits
  adUint32 pitch = ((w + 31) & ~31)/8;     //round up to nearest 4 bytes
  adUint32 pixelSize = pitch * header.height;

  //get color palette size if used

  adUint32 paletteSize = 0;
  adUint32 numPaletteEntries = 0;

  if(fileType == Windows_2x)
    paletteSize = (bitmapOffset - BMP_HEADER_SIZE - BMP_WINDOWS_2X_HEADER_SIZE);
  else if(fileType == Windows_3x)
    paletteSize = (bitmapOffset - BMP_HEADER_SIZE - BMP_WINDOWS_3X_HEADER_SIZE);
  else if(fileType == Windows_4x)
    paletteSize = (bitmapOffset - BMP_HEADER_SIZE - BMP_WINDOWS_4X_HEADER_SIZE);
  else if(fileType == Windows_5x)
    paletteSize = (bitmapOffset - BMP_HEADER_SIZE - BMP_WINDOWS_5X_HEADER_SIZE);

  if(fileType == Windows_2x)
    numPaletteEntries = paletteSize / 12;
  else
    numPaletteEntries = paletteSize / 16;

  //more error checking
  //need to make sure total file size extends to where we expect
  if(fileSize != BMP_HEADER_SIZE + headerSize + pixelSize + paletteSize)
    return AD_IMG_ERR;

  a_destImage->bpp = 32;
  a_destImage->width = header.width;
  a_destImage->height = header.height;
  a_destImage->pitch = a_destImage->width * 4;
  a_destImage->size = a_destImage->width * a_destImage->height * (a_destImage->bpp / 8);

  a_destImage->pixels = malloc(a_destImage->size);

  if(!a_destImage->pixels)
    return AD_IMG_ERR_MALLOC;

  const adUint8* pixels = a_srcImage + bitmapOffset;

  //time to load data
  if(header.bitsPerPixel == 16)
    LoadBGR16ToBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 24)
    LoadBGR24ToBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 32)
    LoadBGRX32ToBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);

  return AD_IMG_ERR_OK;
  
}

AdImageError adSaveBmp(const char *a_file, const int a_filenameLength, const AdImage* a_image, const AdBmpFormat* a_settings)
{
  if(!a_file)
    return AD_IMG_ERR_BADPARRAM;

  if(!a_image)
    return AD_IMG_ERR_BADPARRAM;

  AdBmpType type = Windows_3x;
  AdImageFormat format = AD_IMG_None;
  AdImageHeaderBmp header;
  memset(&header, 0, sizeof(AdImageHeaderBmp));

  //determine filesize
  int size = 0;
  int sizeHeader = 0;
  int width = 0;
  int height = 0;
  int pitch = 0;

  //get format and type info, or guess based of image given
  if(a_settings)
  {
    if(a_settings->type)
      type = a_settings->type;
    if(a_settings->format)
      format = a_settings->format;
  }

  if(!format)
  {
    format = a_image->format;

    //TODO, reverse rgb formats to bgr

    if( !( (format == AD_IMG_BGR8) ||
           (format == AD_IMG_BGR16) ||
           (format == AD_IMG_BGR24) ||
           (format == AD_IMG_BGRX32) ||
           (format == AD_IMG_BGRA32) ||
           (format == AD_IMG_Grey8) ||
           (format == AD_IMG_Mono1)))
    {
      return AD_IMG_ERR;
    }

  }

  switch(type)
  {
    case Windows_2x:
      sizeHeader = BMP_WINDOWS_2X_HEADER_SIZE;
      break;
    case Windows_3x:
      sizeHeader = BMP_WINDOWS_3X_HEADER_SIZE;
      break;

    case Windows_4x:
      sizeHeader = BMP_WINDOWS_4X_HEADER_SIZE;
      break;
    case Windows_5x:
      sizeHeader = BMP_WINDOWS_5X_HEADER_SIZE;
      break;
    default:
      return AD_IMG_ERR;
  }

  width = a_image->width;
  height = a_image->height;

  size += sizeHeader + BMP_HEADER_SIZE;

  if(format == AD_IMG_BGRA32 || format == AD_IMG_BGRX32)
  {
    pitch = a_image->width * 4;
    size += height * pitch;
  }
  else if(format == AD_IMG_BGR24)
  {
    pitch = (a_image->width * 3 + 3) & ~3;
    size += height * pitch;
  }
  else if(format == AD_IMG_BGR16)
  {
    pitch = (a_image->width * 2 + 3) & ~3;
    size += height * pitch;
  }
  else if(format == AD_IMG_BGR8)
  {
    pitch = (a_image->width + 3) & ~3;
    pitch += 256;
    size += height * pitch;
  }

  return AD_IMG_ERR;
}

AdImageError adSaveBmpPointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const AdBmpFormat* a_settings)
{
  return AD_IMG_ERR;
}

