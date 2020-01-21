/*

SOurces
http://www.fileformat.info/format/bmp/egff.htm
https://blog.mozilla.org/nnethercote/2015/11/06/i-rewrote-firefoxs-bmp-decoder/
https://github.com/jsummers/bmpsuite
*/


#include "adbmp.h"
#include "adcil.h"
#include "adbuffer.h"
#include "adBits.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define BM 1
#define BA 2

#define BMP_HEADER_SIZE 14
#define BMP_WINDOWS_2X_HEADER_SIZE 12
#define BMP_OS2_SHORT_HEADER_SIZE 16
#define BMP_WINDOWS_3X_HEADER_SIZE 40
#define BMP_WINDOWS_2_INFO_HEADER_SIZE 52
#define BMP_WINDOWS_3_INFO_HEADER_SIZE 56
#define BMP_OS2_HEADER_SIZE 64
#define BMP_WINDOWS_4X_HEADER_SIZE 108
#define BMP_WINDOWS_5X_HEADER_SIZE 124

#define Color16ToColor32(x) (((x << 3) & 0xff) | ((x << 5) & 0xff00) | ((x << 8) & 0xff0000))


static int CheckFileType(const unsigned char *a_srcImage)
{
  if(a_srcImage[0] == 'B' && a_srcImage[1] == 'M')
    return BM;
  else if(a_srcImage[0] == 'B' && a_srcImage[1] == 'A')
    return BA;
  else return 0;
}



static void LoadBGR8Palette32ToBGRA32(const uint8_t* const a_src,
                       const uint32_t a_width,    //width pixels
                       const uint32_t a_height,   //height pixels
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest,
                       const uint32_t a_pitchDest,
                       const uint8_t* const a_palette,
                       const int a_paletteLength) //num palette enteries
{
  const uint32_t* palette = (uint32_t*) a_palette;
  const uint32_t pitchDest = a_pitchDest/4;

  uint32_t *dest = (uint32_t*) a_dest;

  /*If palette is short we need to check any entry does not go beyond palette
   * into pixels or even beyond the buffer size*/
  if(a_paletteLength < 256)
  {
    for(uint32_t y = 0; y < a_height; y++)
    {
      for(uint32_t x = 0; x < a_width; x++)
      {
        int p = a_src[x+y*a_pitchSrc];
        if(p >= a_paletteLength)
          p = a_paletteLength;
        dest[x+y*pitchDest] = p;
      }
    }
  }
  else
  {
    for(uint32_t y = 0; y < a_height; y++)
    {
      for(uint32_t x = 0; x < a_width; x++)
      {
        dest[x+y*pitchDest] = palette[a_src[x+y*a_pitchSrc]];
      }
    }
  }
}



static void LoadBGR16ToBGRA32(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest,
                       const uint32_t a_pitchDest)
{
  const uint32_t *src = (uint32_t*)a_src;
  uint32_t *dest = (uint32_t*)a_dest;
  uint32_t pitchDest = a_pitchDest/4;
  uint32_t pitchSrc = a_pitchSrc/4;

  uint32_t w = a_width & (~1); //get width rounded down to even number
  for(uint32_t y = 0; y < a_height; y++)
  {
    uint32_t x = 0;

    while(x*2 < w)
    {
      *(dest + x*2+0) = Color16ToColor32(*(src + x)) | 0xff000000;
      *(dest + x*2+1) = Color16ToColor32(*(src + x) >> 16) | 0xff000000;

      ++x;
    }

    if(a_width-w > 0)
    {
      *(dest + x*2+0) = Color16ToColor32(*(src + w)) | 0xff000000;
    }
    dest += pitchDest;
    src += pitchSrc;
  }
}

static void LoadBGR24ToBGRA32(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest,
                       const uint32_t a_pitchDest)
{
  const uint32_t *src = (uint32_t*)a_src;
  uint32_t *dest = (uint32_t*)a_dest;

  const uint32_t widthRounded = a_width & (~3); //get width rounded down
  for(uint32_t y = 0; y < a_height; y++)
  {
    uint32_t x = 0;
    uint32_t w = 0;

    while(x < widthRounded)
    {
      //memcpy(a_dest + a_pitcDest*y, a_src + a_pitchSrc*y, size);
      *(dest + x+0) = *(src + w) | 0xff000000;
      *(dest + x+1) = (*(src + w) >> 24) | (*(src + w+1) << 8) | 0xff000000;
      *(dest + x+2) = (*(src + w+1) >> 16) | (*(src + w+2) << 16) | 0xff000000;
      *(dest + x+3) = (*(src + w+2) >> 8) | 0xff000000;

      x+=4;
      w+=3;
    }

    if(a_width-x > 0)
    {
      *(dest + x+0) = *(src + w) | 0xff000000;
      if(a_width-w > 1)
      {
        *(dest + x+1) = (*(src + w) >> 24) | (*(src + w+1) << 8) | 0xff000000;
        if(a_width-w > 2)
        {
          *(dest + x+2) = (*(src + w+1) >> 16) | (*(src + w+2) << 16) | 0xff000000;
        }
      }
    }

    dest += a_width;
    src += ((a_pitchSrc + 3) & (~3))/4;
  }
}


static void LoadBGRX32ToBGRA32(const uint8_t* const a_src,
                        const uint32_t a_height,
                        const uint32_t a_pitchSrc,
                        uint8_t* a_dest,
                        const uint32_t a_pitchDest)
{
  if(a_pitchDest == a_pitchSrc)
  {
    size_t size = a_pitchSrc * a_height;
    memcpy(a_dest, a_src, size);
  }
  else
  {
    for(uint32_t y = 0; y < a_height; y++)
    {
      memcpy(a_dest + a_pitchDest*y, a_src + a_pitchSrc*y, a_pitchDest);
    }
  }
}

static void LoadBGRA32ToBGRA32(const uint8_t* const a_src,
                        const uint32_t a_height,
                        const uint32_t a_pitchSrc,
                        uint8_t* a_dest,
                        const uint32_t a_pitchDest)
{
  if(a_pitchDest == a_pitchSrc)
  {
    size_t size = a_pitchSrc * a_height;
    memcpy(a_dest, a_src, size);
  }
  else
  {
    for(uint32_t y = 0; y < a_height; y++)
    {
      memcpy(a_dest + a_pitchDest*y, a_src + a_pitchSrc*y, a_pitchDest);
    }
  }
}


static void SaveBGRA32ToBGR24(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest)
{
  uint32_t* dest = (uint32_t*)a_dest;
  uint32_t* src = (uint32_t*)a_src;

  if(!(a_width & 3) && (a_pitchSrc/4) == a_width)
  {
    uint32_t j = 0;
    for(uint32_t i = 0; i< a_width*a_height; i+=4)
    {
      dest[j]    = src[i  ] & 0x00ffffff;       //0rgb
      dest[j]   |= src[i+1]<<24;                //b000
      dest[j+1]  = (src[i+1]>>8) & 0x0000ffff;  //00rg
      dest[j+1] |= src[i+2]<<16;                //gb00
      dest[j+2]  = (src[i+2]>>16) & 0xff;       //000r
      dest[j+2] |= src[i+3] << 8;               //rgb0
      j+=3;
    }
  }
}

static void SaveBGRA32ToBGRX32(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest)
{
  uint32_t* dest = (uint32_t*)a_dest;

  if(!(a_width & 3) && (a_pitchSrc/4) == a_width)
  {
    memcpy(a_dest, a_src, a_pitchSrc*a_height);
    //set high byte to 0
    for(uint32_t i = 0; i< a_width*a_height; i++)
    {
      dest[i] &= 0x00ffffff;
    }
  }
  else
  {

  }

}

static void SaveBGRA32ToBGRA32(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest)
{
  uint8_t* dest = (uint8_t*)a_dest;

  if(!(a_width & 3) && (a_pitchSrc/4) == a_width)
  {
    memcpy(dest, a_src, a_pitchSrc*a_height);
  }
  else
  {
    const uint32_t pitchDest = a_width*4;
    for(int i = 0; i < a_height; ++i){
      memcpy(dest + pitchDest*i, a_src + i*a_pitchSrc, a_pitchSrc);
    }
  }

}

static void SaveBGRX32ToBGRX32(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest)
{
  SaveBGRA32ToBGRA32(a_src, a_width, a_height, a_pitchSrc, a_dest);
}

static void SaveBGRA32ToBGR16(const uint8_t* const a_src,
                       const uint32_t a_width,
                       const uint32_t a_height,
                       const uint32_t a_pitchSrc,
                       uint8_t* a_dest)
{
  uint32_t* dest = (uint32_t*)a_dest;
  uint32_t* src = (uint32_t*)a_src;

  if(!(a_width & 3) && (a_pitchSrc/4) == a_width)
  {
    for(uint32_t i = 0; i < a_width*a_height; i+=2)
    {
      dest[i>>1]  = 0;
      dest[i>>1] |= 0;
    }
  }
}


AdImageError adLoadBmp(const char *a_file, const int a_filenameLength, AdImage* a_destImage, const void* a_destFormat)
{
  return AD_IMG_ERR;
}

AdImageError adLoadBmpPointer(const unsigned char* a_srcImage, size_t a_size,
  AdImage* a_destImage, const void* a_destFormat)
{


  AdBmpType fileType;
  uint32_t headerSize = 0;

  uint32_t bitmapOffset = 0;

  if (!a_destImage)
    return AD_IMG_ERR_BADPARRAM;

  //sanity check
  if (a_size < 14)
  {
    return AD_IMG_ERR_FILETOOSMALL;
  }

  int bmType = CheckFileType(a_srcImage);
  if (!bmType)
    return AD_IMG_ERR_WRONGFILETYPE;

  size_t fileSize = adBufferReuint32_t(a_srcImage + 2);//(int) * ((int*)(a_srcImage + 2));

  if (a_size != fileSize)
    return AD_IMG_ERR;

  AdImageFormat sourceFormat = (AD_COLOR_UNSIGNED << AD_COLOR_VARTYPE_SHIFT) | (AD_COLOR_RGB << AD_COLOR_COLORTYPE_SHIFT);

  //headerSize = (int)*((int*)(a_srcImage + BMP_HEADER_SIZE));
  headerSize = adBufferReuint32_t(a_srcImage + BMP_HEADER_SIZE);
  //bitmapOffset = (int)*((int*)(a_srcImage + 10));
  bitmapOffset = adBufferReuint32_t(a_srcImage + 10);
  /*
   * Note that the only difference between Windows 2.x BMP and OS/2 1.x BMP is
   * the data type of the Width and Height fields. For Windows 2.x, they are
   * signed shorts and for OS/2 1.x, they are unsigned shorts.
   * */
  switch (headerSize)
  {
    //12-64 is OS2_2x
  case BMP_WINDOWS_2X_HEADER_SIZE:  //or OS2_1x
    fileType = Windows_2x;
    break;
  case BMP_WINDOWS_3X_HEADER_SIZE:
    //compression level 3 is NT
    fileType = Windows_3x;
    break;
  case BMP_WINDOWS_2_INFO_HEADER_SIZE:
    fileType = Windows_2_Info;
    break;
  case BMP_WINDOWS_3_INFO_HEADER_SIZE:
    fileType = Windows_3_Info;
    break;
  case BMP_OS2_HEADER_SIZE:
    fileType = OS2_2x;
    break;
  case BMP_WINDOWS_4X_HEADER_SIZE:
    fileType = Windows_4x;
    break;
  case BMP_WINDOWS_5X_HEADER_SIZE:
    fileType = Windows_5x;
    break;
  default:
    return AD_IMG_ERR;

  }

  if (fileSize < (size_t)headerSize + BMP_HEADER_SIZE)
    return AD_IMG_ERR;

  AdImageHeaderBmp header;
  memset(&header, 0, sizeof(AdImageHeaderBmp));

  if (fileType != Windows_2x) {
    memcpy(&header, a_srcImage + BMP_HEADER_SIZE, headerSize);
  }
  else {
    AdImageFileHeaderBmp fileHeader;
    memset(&fileHeader, 0, sizeof(AdImageFileHeaderBmp));
    memcpy(&fileHeader, a_srcImage + BMP_HEADER_SIZE, headerSize);

    header.sizeHeader = fileHeader.sizeHeader;
    header.width = fileHeader.width;
    header.height = fileHeader.height;
    header.planes = fileHeader.planes;
    header.bitsPerPixel = fileHeader.bitsPerPixel;
  }



  //Work out size of image data taking into account that a scanline must be a multiple of 4 bytes
  uint32_t w = header.width * header.bitsPerPixel;  //in bits
  uint32_t pitch = ((w + 31) & ~31) / 8;     //round up to nearest 4 bytes
  uint32_t pixelSize = pitch * header.height;

  //check file size less than image size
  if(pixelSize + bitmapOffset > fileSize)
    return AD_IMG_ERR;

  //get color palette size if used
  uint32_t paletteSize = 0;
  uint32_t numPaletteEntries = 0;
  if (header.bitsPerPixel <= 8 && header.bitsPerPixel > 1)
  {
    sourceFormat |= (AD_COLOR_PALETTE);

    paletteSize = bitmapOffset - BMP_HEADER_SIZE - headerSize;

    int paletteEntrySize = 0;
    if (fileType == Windows_2x){
      paletteEntrySize = 3;
      sourceFormat |= AD_IMG_BGR24;
    }
    else{
      paletteEntrySize = 4;
      sourceFormat |= AD_IMG_BGRX32;
    }
    numPaletteEntries = paletteSize / paletteEntrySize;
  }



  
  if(header.compression == BI_BITFIELDS || header.compression  == BI_ALPHABITFIELDS){
    typedef struct {
      uint32_t msb;
      uint32_t lsb;
      uint64_t color;
    } ColorSort;

    uint32_t numColors = 0;
    uint32_t numSort = 3;
    ColorSort colorSort[4];

    colorSort[0].msb = adMsbUint32(header.maskBlue);
    colorSort[0].lsb = adLsbUint32(header.maskBlue);
    colorSort[0].color = AD_COLOR_BLUE;

    colorSort[1].msb = adMsbUint32(header.maskGreen);
    colorSort[1].lsb = adLsbUint32(header.maskGreen);
    colorSort[1].color = AD_COLOR_GREEN;

    colorSort[2].msb = adMsbUint32(header.maskRed);
    colorSort[2].lsb = adLsbUint32(header.maskRed);
    colorSort[2].color = AD_COLOR_RED;
    

    if (fileType != Windows_2_Info) {
      colorSort[3].msb = adMsbUint32(header.maskAlpha);
      colorSort[3].lsb = adLsbUint32(header.maskAlpha);
      colorSort[3].color = AD_COLOR_ALPHA;
      ++numSort;
    }

    for (uint32_t i = 0; i < numSort; ++i) {
      if (colorSort[i].msb - colorSort[i].lsb) {
        ++numColors;
      }
    }

    for(uint32_t j = 0; j < numSort-1; ++j) {
      for (uint32_t i = j; i < numSort - 1; ++i) {
        //swap
        if (colorSort[i].msb > colorSort[i + 1].msb) {
          ColorSort tcolor;
          memcpy(&tcolor, &colorSort[i], sizeof(ColorSort));
          memcpy(&colorSort[i], &colorSort[i + 1], sizeof(ColorSort));
          memcpy(&colorSort[i + 1], &tcolor, sizeof(ColorSort));
        }
      }
    }

    for (uint32_t i = 0; i < numColors; ++i) {
      uint64_t numBits = colorSort[i].msb - colorSort[i].lsb;
      sourceFormat |= AD_COLOR_BUILD(colorSort[i].color, numBits) << (i*10ULL + 14ULL);
    }

    uint64_t blankSize = header.bitsPerPixel - colorSort[numColors - 1].msb;

    if (blankSize) {
      sourceFormat |= AD_COLOR_BUILD(AD_COLOR_PAD, blankSize) << (numColors * 10ULL + 14ULL);
      ++numColors;
    }

    sourceFormat |= numColors << 2ULL;

  }
  else {
    switch (header.bitsPerPixel) {
    case 8:

      break;
    case 16:
      break;
    case 24:
      break;
    case 32:
      break;
    }
  }

  //more error checking
  //need to make sure total file size extends to where we expect
  if(fileSize != BMP_HEADER_SIZE + headerSize + pixelSize + paletteSize)
    return AD_IMG_ERR;

  
  //todo
  a_destImage->bpp = 32;
  a_destImage->width = header.width;
  a_destImage->height = header.height;
  a_destImage->pitch = a_destImage->width * 4;
  a_destImage->size = a_destImage->width * a_destImage->height * (a_destImage->bpp / 8);
  if((sourceFormat & ~AD_COLOR_PALETTE) == AD_IMG_BGRA32)
    a_destImage->format = AD_IMG_BGRA32;
  else
    a_destImage->format = AD_IMG_BGRX32;

  a_destImage->pixels = malloc(a_destImage->size);

  if(!a_destImage->pixels)
    return AD_IMG_ERR_MALLOC;

  const uint8_t* pixels = a_srcImage + bitmapOffset;

  //time to load data
  if(sourceFormat & AD_COLOR_PALETTE){
    const uint8_t* palette = a_srcImage + headerSize + BMP_HEADER_SIZE;
    LoadBGR8Palette32ToBGRA32(pixels, header.width, header.height, pitch,
                              a_destImage->pixels, a_destImage->pitch,
                              palette, numPaletteEntries);
  }
  else if(header.bitsPerPixel == 16)
    LoadBGR16ToBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 24)
    LoadBGR24ToBGRA32(pixels, header.width, header.height, pitch, a_destImage->pixels, a_destImage->pitch);
  else if(header.bitsPerPixel == 32)
    LoadBGRX32ToBGRA32(pixels, header.height, pitch, a_destImage->pixels, a_destImage->pitch);

  return AD_IMG_OK;  
}

AdImageError adSaveBmp(const char *a_file, const int a_filenameLength, const AdImage* a_image, const AdBmpFormat* a_settings)
{
  if(!a_file)
    return AD_IMG_ERR_BADPARRAM;

  if(!a_image)
    return AD_IMG_ERR_BADPARRAM;

  //bmp type to save to
  AdBmpType type = Unknown;

  //image format to save to
  AdImageFormat format = AD_IMG_NONE;
  //AdImageHeaderBmp header;
  //memset(&header, 0, sizeof(AdImageHeaderBmp));

  //determine filesize
  uint32_t size = 0;
  uint32_t dataSize = 0;
  uint32_t sizeHeader = 0;
  uint32_t sizePalette = 0;
  uint32_t sizeMask = 0;
  int width = 0;
  int height = 0;
  int pitch = 0;
  int planes = 1;
  int bpp = 24;
  int compression = 0;
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
#if 0
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
#endif
  }

  AdImageFormat a = AD_IMG_BGRA32;
  AdImageFormat b = AD_IMG_BGRX32;

  width = a_image->width;
  height = a_image->height;



  if(format == AD_IMG_BGRA32 || format == AD_IMG_BGRX32)
  {
    pitch = a_image->width * 4;
    dataSize = height * pitch;
    size += dataSize;
    if(format == AD_IMG_BGRA32)
      planes = 4;
    else
      planes = 3;
    bpp=32;
    compression = 3;
    sizeMask = 16;

    //Apple preview does not appear to load bmp with v4 header and rgba
    if(!type)
      type = Windows_5x;
  }
  else if(format == AD_IMG_BGR24)
  {
    pitch = (a_image->width * 3 + 3) & ~3;
    dataSize = height * pitch;
    size += dataSize;
    planes = 3;
    bpp=24;

    if(!type)
      type = Windows_3x;
  }
  else if(format == AD_IMG_BGR16)
  {
    pitch = (a_image->width * 2 + 3) & ~3;
    dataSize = height * pitch;
    size += dataSize;
    planes = 3;
    bpp=16;
    compression = 3;
    sizeMask = 16;

    if(!type)
      type = Windows_3x;
  }
  else if(format == AD_IMG_BGR8)
  {
    planes = 3;
    sizePalette = 256;

    pitch = (a_image->width + 3) & ~3;
    pitch += sizePalette;
    dataSize = height * pitch;
    size += dataSize;
    bpp=8;

    if(!type)
      type = Windows_3x;
    //gen palette
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

  //sizeHeader += sizeMask; //???
  uint32_t sizeHeaderTotal = sizeHeader + BMP_HEADER_SIZE + sizePalette;
  size += sizeHeaderTotal;


  //request buffer then offset by 2 to align by 4 byte values after 2 byte 'bm'
  //at the start.
  adBuffer buffer;
  buffer.buffer = malloc(size+2);
  buffer.start = buffer.buffer + 2;
  buffer.pos = buffer.start;

  if(!buffer.buffer)
    return AD_IMG_ERR;



  adBufferWrite(&buffer, "BM", 2);
  adBufferWriteUint32(&buffer, size);

  /*set reserved*/
  adBufferWriteUint32(&buffer, 0);

  /*set offset*/
  adBufferWriteUint32(&buffer, sizeHeaderTotal);

  /*Windows_2x*/

  adBufferWriteUint32(&buffer, sizeHeader);
  adBufferWriteUint32(&buffer, width);
  adBufferWriteUint32(&buffer, height);

  //planes
  adBufferWriteUint16(&buffer, 1);

  //bpp
  /*BitsPerPixel is the number of bits per pixel in each plane. This value will
   * be in the range 1 to 24; the values 1, 4, 8, and 24 are the only values
   * considered legal by the Windows 2.x and 3.x API.
   *
  */
  adBufferWriteUint16(&buffer, (uint16_t)bpp);

  if(type == Windows_3x || type == Windows_4x || type == Windows_5x)
  {
    //DWORD Compression;     /* Compression methods used */
    adBufferWriteUint32(&buffer, compression);
      //DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
    adBufferWriteUint32(&buffer, dataSize);
      //LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
    adBufferWriteUint32(&buffer, 2835);
      //LONG  VertResolution;  /* Vertical resolution in pixels per meter */
    adBufferWriteUint32(&buffer, 2835);
      //DWORD ColorsUsed;      /* Number of colors in the image */
    adBufferWriteUint32(&buffer, 0);
      //DWORD ColorsImportant; /* Minimum number of important colors, 0 for all colors */
    adBufferWriteUint32(&buffer, 0);
  }

  if(format == AD_IMG_BGRA32 || format == AD_IMG_BGRX32)
  {
    //DWORD RedMask;       /* Mask identifying bits of red component */
    adBufferWriteUint32(&buffer, 0x00ff0000);//0xff000000;//
    //DWORD GreenMask;     /* Mask identifying bits of green component */
    adBufferWriteUint32(&buffer, 0x0000ff00);//0x00ff0000;//
    //DWORD BlueMask;      /* Mask identifying bits of blue component */
    adBufferWriteUint32(&buffer, 0x000000ff);//0x0000ff00;//
    //DWORD AlphaMask;     /* Mask identifying bits of alpha component */
    if(format == AD_IMG_BGRA32)
      adBufferWriteUint32(&buffer, 0xff000000);//0x000000ff;//
    else
      adBufferWriteUint32(&buffer, 0x0);
  }
  else if(format == AD_IMG_BGR16)
  {
    //DWORD RedMask;       /* Mask identifying bits of red component */
    adBufferWriteUint32(&buffer, 0x0000f800);//0xff000000;//
    //DWORD GreenMask;     /* Mask identifying bits of green component */
    adBufferWriteUint32(&buffer, 0x000007e0);//0x00ff0000;//
    //DWORD BlueMask;      /* Mask identifying bits of blue component */
    adBufferWriteUint32(&buffer, 0x0000001f);//0x0000ff00;//
    //DWORD AlphaMask;     /* Mask identifying bits of alpha component */
    adBufferWriteUint32(&buffer, 0x00000000);//0x000000ff;//
  }

  if(type == Windows_4x || type == Windows_5x)
  {
    //DWORD CSType;        /* Color space type */
    /*CSType is the color space type used by the bitmap data. Values for this
     * field include 00h (calibrated RGB), 01h (device-dependent RGB),
     * and 02h (device-dependent CMYK). Device-dependent RGB is the default
     * color space. Calibrated RGB is defined by the 1931 CIE XYZ standard.
     * */

    adBufferWriteUint32(&buffer, 0x73524742);
    //*(ptrU32+17) = 0x00000000;


    /* RedX, RedY, and RedZ specify the CIE X, Y, and Z coordinates,
     * respectively, for the endpoint of the red component of a specified
     * logical color space. These fields are used only when CSType is 00h
     * (calibrated RGB).
     * */
    //LONG  RedX;          /* X coordinate of red endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  RedY;          /* Y coordinate of red endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  RedZ;          /* Z coordinate of red endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  GreenX;        /* X coordinate of green endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  GreenY;        /* Y coordinate of green endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  GreenZ;        /* Z coordinate of green endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  BlueX;         /* X coordinate of blue endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  BlueY;         /* Y coordinate of blue endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //LONG  BlueZ;         /* Z coordinate of blue endpoint */
    adBufferWriteUint32(&buffer, 0x0);
    //DWORD GammaRed;      /* Gamma red coordinate scale value */
    adBufferWriteUint32(&buffer, 0x0);
    //DWORD GammaGreen;    /* Gamma green coordinate scale value */
    adBufferWriteUint32(&buffer, 0x0);
    //DWORD GammaBlue;     /* Gamma blue coordinate scale value */
    adBufferWriteUint32(&buffer, 0x0);

    if(type == Windows_5x)
    {
      /*
      uint32_t        intent;
      uint32_t        profileData;
      uint32_t        profileSize;
      uint32_t        reserved;*/
      adBufferWriteUint32(&buffer, 0x0);
      adBufferWriteUint32(&buffer, 0x0);
      adBufferWriteUint32(&buffer, 0x0);
      adBufferWriteUint32(&buffer, 0x0);
    }
  }

#if 0
  ptrU32 = (uint32_t*)(destBuffer+2);
  ptrU16 = (uint16_t*)(destBuffer+2);
  *ptrU32 = size;

  //set reserved
  *(ptrU32+1) = 0;

  //set offset
  *(ptrU32+2) = sizeHeaderTotal;

  //write type header
  //Windows_2x
  //header size
  *(ptrU32+3) = sizeHeader;

  //width
  *(ptrU32+4) = width;

  //height
  *(ptrU32+5) = height;

  //planes
  *(ptrU16+12) = 1;

  //bpp
  /*BitsPerPixel is the number of bits per pixel in each plane. This value will
   * be in the range 1 to 24; the values 1, 4, 8, and 24 are the only values
   * considered legal by the Windows 2.x and 3.x API.
   *
  */
  *(ptrU16+13) = bpp;

  if(type == Windows_3x || type == Windows_4x || type == Windows_5x)
  {
      //DWORD Compression;     /* Compression methods used */
    *(ptrU32+7) = compression;
      //DWORD SizeOfBitmap;    /* Size of bitmap in bytes */
    *(ptrU32+8) = dataSize;
      //LONG  HorzResolution;  /* Horizontal resolution in pixels per meter */
    *(ptrU32+9) = 2835;
      //LONG  VertResolution;  /* Vertical resolution in pixels per meter */
    *(ptrU32+10) = 2835;
      //DWORD ColorsUsed;      /* Number of colors in the image */
    *(ptrU32+11) = 0;
      //DWORD ColorsImportant; /* Minimum number of important colors, 0 for all colors */
    *(ptrU32+12) = 0;

    if(type == Windows_4x || type == Windows_5x)
    {
      if(format == AD_IMG_BGRA32 || format == AD_IMG_BGRX32)
      {
        //DWORD RedMask;       /* Mask identifying bits of red component */
        *(ptrU32+13) = 0x00ff0000;//0xff000000;//
        //DWORD GreenMask;     /* Mask identifying bits of green component */
        *(ptrU32+14) = 0x0000ff00;//0x00ff0000;//
        //DWORD BlueMask;      /* Mask identifying bits of blue component */
        *(ptrU32+15) = 0x000000ff;//0x0000ff00;//
        //DWORD AlphaMask;     /* Mask identifying bits of alpha component */
        *(ptrU32+16) = 0xff000000;//0x000000ff;//
      }
      else
      {
        *(ptrU32+13) = 0;
        *(ptrU32+14) = 0;
        *(ptrU32+15) = 0;
        *(ptrU32+16) = 0;
      }
      //DWORD CSType;        /* Color space type */
      /*CSType is the color space type used by the bitmap data. Values for this
       * field include 00h (calibrated RGB), 01h (device-dependent RGB),
       * and 02h (device-dependent CMYK). Device-dependent RGB is the default
       * color space. Calibrated RGB is defined by the 1931 CIE XYZ standard.
       * */

      *(ptrU32+17) = 0x73524742;
      //*(ptrU32+17) = 0x00000000;


      /* RedX, RedY, and RedZ specify the CIE X, Y, and Z coordinates,
       * respectively, for the endpoint of the red component of a specified
       * logical color space. These fields are used only when CSType is 00h
       * (calibrated RGB).
       * */
      //LONG  RedX;          /* X coordinate of red endpoint */
      *(ptrU32+18) = 0x0;
      //LONG  RedY;          /* Y coordinate of red endpoint */
      *(ptrU32+19) = 0x0;
      //LONG  RedZ;          /* Z coordinate of red endpoint */
      *(ptrU32+20) = 0x0;
      //LONG  GreenX;        /* X coordinate of green endpoint */
      *(ptrU32+21) = 0x0;
      //LONG  GreenY;        /* Y coordinate of green endpoint */
      *(ptrU32+22) = 0x0;
      //LONG  GreenZ;        /* Z coordinate of green endpoint */
      *(ptrU32+23) = 0x0;
      //LONG  BlueX;         /* X coordinate of blue endpoint */
      *(ptrU32+24) = 0x0;
      //LONG  BlueY;         /* Y coordinate of blue endpoint */
      *(ptrU32+25) = 0x0;
      //LONG  BlueZ;         /* Z coordinate of blue endpoint */
      *(ptrU32+26) = 0x0;
      //DWORD GammaRed;      /* Gamma red coordinate scale value */
      *(ptrU32+27) = 0x0;
      //DWORD GammaGreen;    /* Gamma green coordinate scale value */
      *(ptrU32+28) = 0x0;
      //DWORD GammaBlue;     /* Gamma blue coordinate scale value */
      *(ptrU32+29) = 0x0;

      if(type == Windows_5x)
      {
        /*
        uint32_t        intent;
        uint32_t        profileData;
        uint32_t        profileSize;
        uint32_t        reserved;*/
        *(ptrU32+30) = 0x0;
        *(ptrU32+31) = 0x0;
        *(ptrU32+32) = 0x0;
        *(ptrU32+33) = 0x0;
      }
    }
  }

#endif
  if(format == AD_IMG_BGR24 && a_image->format == AD_IMG_BGRA32)
    SaveBGRA32ToBGR24(a_image->pixels, a_image->width, a_image->height,
                      a_image->pitch, buffer.start+sizeHeaderTotal);
  else if(format == AD_IMG_BGRA32 && a_image->format == AD_IMG_BGRA32)
    SaveBGRA32ToBGRA32(a_image->pixels, a_image->width, a_image->height,
                      a_image->pitch, buffer.start+sizeHeaderTotal);
  else if(format == AD_IMG_BGRX32 && a_image->format == AD_IMG_BGRX32)
    SaveBGRX32ToBGRX32(a_image->pixels, a_image->width, a_image->height,
                      a_image->pitch, buffer.start+sizeHeaderTotal);
  else if(format == AD_IMG_BGR16 && a_image->format == AD_IMG_BGRA32)
    SaveBGRA32ToBGR16(a_image->pixels, a_image->width, a_image->height,
                      a_image->pitch, buffer.start+sizeHeaderTotal);


  //write to file
#if _WIN32
  FILE* file = NULL;
  fopen_s(&file, a_file, "wb");
#else
  FILE* file = fopen(a_file, "wb");
#endif

  fwrite(buffer.start, 1, size, file);
  fclose(file);

  free(buffer.buffer);

  return AD_IMG_OK;
}

AdImageError adSaveBmpPointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const AdBmpFormat* a_settings)
{
  return AD_IMG_ERR;
}

