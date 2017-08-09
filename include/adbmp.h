
#ifndef ADBMP_H
#define ADBMP_H

#include "adcil_types.h"

int adOpenBmp();

typedef struct {
  /* Windows 2.x 12 bytes */
  adUint32        sizeHeader;
  adInt32         width;
  adInt32         height;
  adUint16        planes;          /* Number of color planes */
  adUint16        bitsPerPixel;    /* Number of bits per pixel */
  /* Windows 3.x 40 bytes */
  adUint32        compression;     /* Compression methods used */
  adUint32        sizeFile;        /* Size of bitmap in bytes */
  adInt32         resolutionHorz;  /* Horizontal resolution in pixels per meter */
  adInt32         resolutionVert;  /* Vertical resolution in pixels per meter */
  adUint32        colorsUsed;      /* Number of colors in the image */
  adUint32        colorsImportant; /* Minimum number of important colors */
  /* Windows 4.x */
  adUint32        maskRed;       /* Mask identifying bits of red component */
  adUint32        maskGreen;     /* Mask identifying bits of green component */
  adUint32        maskBlue;      /* Mask identifying bits of blue component */
  adUint32        maskAlpha;     /* Mask identifying bits of alpha component */
  adUint32        colorSpaceType;        /* Color space type */
  adInt32         redX;          /* X coordinate of red endpoint */
  adInt32         redY;          /* Y coordinate of red endpoint */
  adInt32         redZ;          /* Z coordinate of red endpoint */
  adInt32         greenX;        /* X coordinate of green endpoint */
  adInt32         greenY;        /* Y coordinate of green endpoint */
  adInt32         greenZ;        /* Z coordinate of green endpoint */
  adInt32         blueX;         /* X coordinate of blue endpoint */
  adInt32         blueY;         /* Y coordinate of blue endpoint */
  adInt32         blueZ;         /* Z coordinate of blue endpoint */
  adUint32        gammaRed;      /* Gamma red coordinate scale value */
  adUint32        gammaGreen;    /* Gamma green coordinate scale value */
  adUint32        gammaBlue;     /* Gamma blue coordinate scale value */
  /*Windows 5.x*/
  adUint32        intent;
  adUint32        profileData;
  adUint32        profileSize;
  adUint32        reserved;
} AdImageHeaderBmp;

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

typedef struct{
  AdBmpType       type;
  AdImageFormat   format;
  adBool          compression;
} AdBmpFormat;

AdImageError adLoadBmp(const char *a_file, const int a_filenameLength, AdImage* a_destImage, const void* a_destFormat);
AdImageError adLoadBmpPointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, const void* a_destFormat);

//if settings is null then try to save as same format as image
//if image type is unknown (set to 0) then guess best type (probably 3x)
AdImageError adSaveBmp(const char *a_file, const int a_filenameLength, const AdImage* a_image, const AdBmpFormat* a_settings);
AdImageError adSaveBmpPointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const AdBmpFormat* a_settings);

#endif //#define ADBMP_H

