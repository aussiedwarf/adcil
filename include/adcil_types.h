#ifndef ADCIL_TYPES_H
#define ADCIL_TYPES_H

#include <stdio.h>

#define adInline inline

typedef unsigned char       adUint8;
typedef unsigned short      adUint16;
typedef unsigned int        adUint32;
typedef unsigned long long  adUint64;
typedef char                adInt8;
typedef short               adInt16;
typedef int                 adInt32;
typedef long long           adInt64;
#if 0
typedef bool                adBool;
#else
typedef int                 adBool;
#endif

/*
 *
 * Generic image format could be made up of 64 bits:
 * 2 bit number of components (up to 4)
 * 2 bit flags (special type, palette)
 * 15 bit color component, 8 bit size, 7 bit type
 * 8 bit size allows up to 256 bits
 * types allow for r,g,b,a,x,y,u,v,grey,mono or more
 * special for things like dxt1, etc1
 * palette then needs to indicate palette index size, and palette color types
 * */

typedef enum
{
  AD_IMG_None = 0,//or unknown
  AD_IMG_RGB24,
  AD_IMG_RGBX32,
  AD_IMG_RGBA32,
  AD_IMG_BGR,
  AD_IMG_BGR8,
  AD_IMG_BGR16,
  AD_IMG_BGR24,
  AD_IMG_BGRX32,
  AD_IMG_BGRA32,
  AD_IMG_Grey8,
  AD_IMG_Mono1,
  AD_IMG_DXT1,
  AD_IMG_DXT3,
  AD_IMG_DXT5,
  AD_IMG_ETC1,
  AD_IMG_ETC2,
  AD_IMG_PVRTC,
  AD_IMG_PVRTC2,
  AD_IMG_ASTC
} AdImageFormat;

typedef enum
{
  AD_IMG_OK = 0,
  AD_IMG_ERR,
  AD_IMG_ERR_MALLOC,  //malloc return null, possibly out of memory
  AD_IMG_ERR_BADPARRAM,
  AD_IMG_ERR_FILETOOSMALL,  //expected image file to be bigger
  AD_IMG_ERR_WRONGFILETYPE
} AdImageError;


typedef struct
{

} AdImageStream;


typedef struct
{
  unsigned char*  pixels;
  int             width;
  int             pitch;
  int             height;
  int             bpp;
  size_t          size; //size of pixel data in bytes
  AdImageFormat   format;
  void*           streamIn;
}AdImage;

typedef enum
{
  AD_IMG_RED    = 0x01,
  AD_IMG_GREEN  = 0x02,
  AD_IMG_BLUE   = 0x04,
  AD_IMG_ALPHA  = 0x08,
  AD_IMG_GREY   = 0x10


} AdImageColor;

typedef struct {
  adUint8 numComponents; //num
  adUint8 bits[4];
  adUint8 pos[4];


} AdImageFormatRaw;



#endif
