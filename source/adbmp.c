/*

SOurces
http://www.fileformat.info/format/bmp/egff.htm
*/


#include "adbmp.h"
#include "adcil.h"

#include <stdlib.h>
#include <string.h>

typedef enum{
  Unknown = 0,
  Windows_2x,
  Windows_3x,
  Windows_NTx,
  Windows_4x,
  Windows_5x,
  OS2_1x,
  OS2_2x,
} AdBmpType;

#define BMP_HEADER_SIZE 14
#define BMP_WINDOWS_2X_HEADER_SIZE 12
#define BMP_WINDOWS_3X_HEADER_SIZE 40
#define BMP_WINDOWS_4X_HEADER_SIZE 108
#define BMP_WINDOWS_5X_HEADER_SIZE 124


int CheckFileType(const unsigned char *a_srcImage)
{
  if(a_srcImage[0] == 'B' && a_srcImage[1] == 'M')
    return 1;
  else if(a_srcImage[0] == 'B' && a_srcImage[1] == 'A')
    return 2;
  else return 0;
}

void LoadBGR16toBGRA32(const adUint8* const a_src,
                       const adUint32 a_width,
                       const adUint32 a_height,
                       const adUint32 a_pitchSrc,
                       adUint8* a_dest,
                       const adUint32 a_pitcDest)
{

}

void LoadBGR24toBGRA32(const adUint8* const a_src,
                       const adUint32 a_width,
                       const adUint32 a_height,
                       const adUint32 a_pitchSrc,
                       adUint8* a_dest,
                       const adUint32 a_pitcDest)
{
  for(int y = 0; y < a_height; y++)
  {
    memcpy(a_dest + a_pitcDest*y, a_src + a_pitchSrc*y, size);
  }
}

void LoadBGRX32toBGRA32(const adUint8* const a_src,
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
    LoadBGR16toBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 24)
    LoadBGR24toBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 32)
    LoadBGRX32toBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);

  return AD_IMG_ERR_OK;
  
}

AdImageError adSaveBmp(const char *a_file, const int a_filenameLength, const AdImage* a_image, const void* a_settings)
{
  return AD_IMG_ERR;
}

AdImageError adSaveBmpPointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_settings)
{
  return AD_IMG_ERR;
}

