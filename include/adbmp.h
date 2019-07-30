
#ifndef ADBMP_H
#define ADBMP_H

#include "adcil_types.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
  /* Windows 2.x 12 bytes */
  uint32_t        sizeHeader;
  uint16_t        width;
  uint16_t        height;
  uint16_t        planes;          /* Number of color planes */
  uint16_t        bitsPerPixel;    /* Number of bits per pixel */
} AdImageHeaderBmpSmall;


typedef struct {
  /* Windows 2.x 12 bytes */
  uint32_t        sizeHeader;
  int32_t         width;
  int32_t         height;
  /* OS2 2x short 16 bytes */
  uint16_t        planes;          /* Number of color planes */
  uint16_t        bitsPerPixel;    /* Number of bits per pixel */
  /* Windows 3.x 40 bytes */
  uint32_t        compression;     /* Compression methods used */
  uint32_t        sizeFile;        /* Size of bitmap in bytes */
  int32_t         resolutionHorz;  /* Horizontal resolution in pixels per meter */
  int32_t         resolutionVert;  /* Vertical resolution in pixels per meter */
  uint32_t        colorsUsed;      /* Number of colors in the image */
  uint32_t        colorsImportant; /* Minimum number of important colors */
  /* 52 */
  union {
    /* windows */
    struct {
      uint32_t        maskRed;       /* Mask identifying bits of red component */
      uint32_t        maskGreen;     /* Mask identifying bits of green component */
      uint32_t        maskBlue;      /* Mask identifying bits of blue component */
      /* 56 */
      uint32_t        maskAlpha;     /* Mask identifying bits of alpha component */
      /* Windows 4.x 108 bytes */
      uint32_t        colorSpaceType;        /* Color space type */
      int32_t         redX;          /* X coordinate of red endpoint */
    } ;//AdImageHeaderBmpWin
    /* os2 64 bytes*/
    struct {
      uint16_t        units;
      uint16_t        reserved1;
      uint16_t        recordings;   /* Recording algorithm */
      uint16_t        rendering;    /* Halftoning algorithm used */
      uint32_t        size1;            /* Reserved for halftoning algorithm use */
      uint32_t        size2;            /* Reserved for halftoning algorithm use */
      uint32_t        colorEncoding;    /* Color model used in bitmap */
      uint32_t        identifier;       /* Reserved for application use */
    } ;//AdImageHeaderBmpOs2
  };//AdImageHeaderBmpInternal
  int32_t         redY;          /* Y coordinate of red endpoint */
  int32_t         redZ;          /* Z coordinate of red endpoint */
  int32_t         greenX;        /* X coordinate of green endpoint */
  int32_t         greenY;        /* Y coordinate of green endpoint */
  int32_t         greenZ;        /* Z coordinate of green endpoint */
  int32_t         blueX;         /* X coordinate of blue endpoint */
  int32_t         blueY;         /* Y coordinate of blue endpoint */
  int32_t         blueZ;         /* Z coordinate of blue endpoint */
  uint32_t        gammaRed;      /* Gamma red coordinate scale value */
  uint32_t        gammaGreen;    /* Gamma green coordinate scale value */
  uint32_t        gammaBlue;     /* Gamma blue coordinate scale value */
  /* Windows 5.x 124 */
  uint32_t        intent;
  uint32_t        profileData;
  uint32_t        profileSize;
  uint32_t        reserved;

} AdImageHeaderBmp;

typedef enum{
  Unknown = 0,
  Windows_2x,
  Windows_3x,
  Windows_NTx,
  Windows_2_Info,
  Windows_3_Info,
  Windows_4x,
  Windows_5x,
  OS2_1x,
  OS2_2x,
  OS2_2x_Short,
} AdBmpType;

typedef struct{
  AdBmpType       type;
  AdImageFormat   format;
  bool            compression;
} AdBmpFormat;

AdImageError adLoadBmp(const char *a_file, const int a_filenameLength, AdImage* a_destImage, const void* a_destFormat);
AdImageError adLoadBmpPointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, const void* a_destFormat);

//if settings is null then try to save as same format as image
//if image type is unknown (set to 0) then guess best type (probably 3x)
AdImageError adSaveBmp(const char *a_file, const int a_filenameLength, const AdImage* a_image, const AdBmpFormat* a_settings);
AdImageError adSaveBmpPointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const AdBmpFormat* a_settings);

#endif //#define ADBMP_H

