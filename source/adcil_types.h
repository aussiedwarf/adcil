#ifndef ADCIL_TYPES_H
#define ADCIL_TYPES_H

#include <stdio.h>

typedef unsigned char       adUint8;
typedef unsigned short      adUint16;
typedef unsigned int        adUint32;
typedef unsigned long long  adUint64;
typedef char                adInt8;
typedef short               adInt16;
typedef int                 adInt32;
typedef long long           adInt64;

typedef enum
{
  AD_IMG_RGBA32,
  AD_IMG_BGRA32
} AdImageFormat;

typedef enum
{
  AD_IMG_ERR_OK = 0,
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

#endif
