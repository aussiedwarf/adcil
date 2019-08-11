#ifndef ADCIL_TYPES_H
#define ADCIL_TYPES_H

#include <stdio.h>
#include <stdint.h>

#define adInline inline

#if 0
typedef unsigned char       uint8_t;
typedef unsigned short      adUint16;
typedef unsigned int        uint32_t;
typedef unsigned long long  adUint64;
typedef char                adInt8;
typedef short               adInt16;
typedef int                 adInt32;
typedef long long           adInt64;
#if 0
typedef bool                adBool;
#else
typedef char                adBool;
#endif

#endif
/*
 *
 * Generic image format coULLd be made up of 64 bits:
 * 2 bit flags (special type, palette)
 * 3 bit number of components (up to 5)
 * 2 bit type (signed, unsigned, float)
 * 7 bit type (rgb grey, yuv, cmyk or more)
 * 6 bit size, 4 bit color type
 * special for things like dxt1, etc1
 * palette then needs to indicate palette index size, and palette color types
 * */

#define AD_COLOR_VARTYPE_SHIFT 5ULL
#define AD_COLOR_COLORTYPE_SHIFT 7ULL

#define AD_COLOR_BUILD(colorType, numBits) colorType | (numBits << 6ULL)

#define AD_COLOR_BUILD1(flag, num, varType, colorType, color0) (flag | (num << 2ULL) | (varType << 5ULL) | (colorType << 7ULL) | (color0 << 14ULL))
#define AD_COLOR_BUILD2(flag, num, varType, colorType, color0, color1) (flag | (num << 2ULL) | (varType << 5ULL) | (colorType << 7ULL) | (color0 << 14ULL) | (color1 << 24ULL))
#define AD_COLOR_BUILD3(flag, num, varType, colorType, color0, color1, color2) (flag | (num << 2ULL) | (varType << 5ULL) | (colorType << 7ULL) | (color0 << 14ULL) | (color1 << 24ULL) | (color2 << 34ULL))
#define AD_COLOR_BUILD4(flag, num, varType, colorType, color0, color1, color2, color3) (flag | (num << 2ULL) | (varType << 5ULL) | (colorType << 7ULL) | (color0 << 14ULL) | (color1 << 24ULL) | (color2 << 34ULL) | (color3 << 44ULL))
#define AD_COLOR_BUILD5(flag, num, varType, colorType, color0, color1, color2, color3, color4) (flag | (num << 2ULL) | (varType << 5ULL) | (colorType << 7ULL) | (color0 << 14ULL) | (color1 << 24ULL) | (color2 << 34ULL) | (color3 << 44ULL) | (color4 << 54ULL))

#define AD_COLOR_PALETTE 2ULL

#define AD_COLOR_RGB 0ULL

#define AD_COLOR_UNSIGNED 0ULL
#define AD_COLOR_SIGNED 1ULL
#define AD_COLOR_FLOAT 2ULL

#define AD_COLOR_PAD 0ULL
#define AD_COLOR_ALPHA 1ULL

#define AD_COLOR_GREY 2ULL
#define AD_COLOR_RED 3ULL
#define AD_COLOR_GREEN 4ULL
#define AD_COLOR_BLUE 5ULL

#define AD_COLOR_RED5 AD_COLOR_BUILD(AD_COLOR_RED, 5ULL)
#define AD_COLOR_RED8 AD_COLOR_BUILD(AD_COLOR_RED, 8ULL)
#define AD_COLOR_GREEN6 AD_COLOR_BUILD(AD_COLOR_GREEN, 6ULL)
#define AD_COLOR_GREEN8 AD_COLOR_BUILD(AD_COLOR_GREEN, 8ULL)
#define AD_COLOR_BLUE5 AD_COLOR_BUILD(AD_COLOR_BLUE, 5ULL)
#define AD_COLOR_BLUE8 AD_COLOR_BUILD(AD_COLOR_BLUE, 8ULL)
#define AD_COLOR_GREY8 AD_COLOR_BUILD(AD_COLOR_GREY, 8ULL)
#define AD_COLOR_ALPHA8 AD_COLOR_BUILD(AD_COLOR_ALPHA, 8ULL)

#define AD_IMG_NONE 0ULL

#define AD_IMG_MONO1 AD_COLOR_BUILD1(0ULL, 1ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BUILD(AD_COLOR_GREY, 1ULL))
#define AD_IMG_GREY8 AD_COLOR_BUILD1(0ULL, 1ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_GREY8)
#define AD_IMG_BGR8 AD_COLOR_BUILD3(AD_COLOR_PALETTE, 3ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BLUE8, AD_COLOR_GREEN8, AD_COLOR_RED8)
#define AD_IMG_BGR16 AD_COLOR_BUILD3(0ULL, 3ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BLUE5, AD_COLOR_GREEN6, AD_COLOR_RED5)
#define AD_IMG_BGR24 AD_COLOR_BUILD3(0ULL, 3ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BLUE8, AD_COLOR_GREEN8, AD_COLOR_RED8)
#define AD_IMG_BGRA32 AD_COLOR_BUILD4(0ULL, 4ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BLUE8, AD_COLOR_GREEN8, AD_COLOR_RED8, AD_COLOR_ALPHA8)
#define AD_IMG_BGRX32 AD_COLOR_BUILD4(0ULL, 4ULL, AD_COLOR_UNSIGNED, AD_COLOR_RGB, AD_COLOR_BLUE8, AD_COLOR_GREEN8, AD_COLOR_RED8, AD_COLOR_BUILD(AD_COLOR_PAD,8ULL))

#if 0
typedef enum
{
  AD_IMG_None = 0,//or unknown
  AD_IMG_RGB16,
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
  AD_IMG_ASTC,
  AD_IMG_RAW
} AdImageFormat;
#endif
typedef uint64_t AdImageFormat;

typedef enum
{
  AD_IMG_OK = 0,
  AD_IMG_ERR,
  AD_IMG_ERR_MALLOC,  //malloc return nULLl, possibly out of memory
  AD_IMG_ERR_BADPARRAM,
  AD_IMG_ERR_FILETOOSMALL,  //expected image file to be bigger
  AD_IMG_ERR_WRONGFILETYPE
} AdImageError;

#if 0
typedef struct
{

} AdImageStream;
#endif

typedef struct
{
  uint8_t*        pixels;
  uint32_t        width;
  uint32_t        pitch;
  uint32_t        height;
  uint32_t        bpp;
  uint32_t        align;
  size_t          size; //size of pixel data in bytes
  AdImageFormat   format;
  void*           streamIn;
} AdImage;

typedef enum
{
  AD_IMG_RED    = 0x01,
  AD_IMG_GREEN  = 0x02,
  AD_IMG_BLUE   = 0x04,
  AD_IMG_ALPHA  = 0x08,
  AD_IMG_GREY   = 0x10


} AdImageColor;

typedef struct {
  uint8_t numComponents; //num
  uint8_t bits[4];
  uint8_t pos[4];


} AdImageFormatRaw;



#endif
