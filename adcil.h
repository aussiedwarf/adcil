/*
AussieDwarf C Image Library

A crossplatform c library for encoding and decoding many common image formats.

Goal for the library is to work on Windows, Macos, Linux, Android, iOS, x64, x64, arm and 
arm64. The library does not depend on any third party libraries and shall be as fast as 
possible.

compatible ansi c (20 years), c++03 (10 years)

Compilers: gcc, clang, visual studio

bmp
gif
png
jpeg
jpeg2000
bpg
tga
tiff
webp
flif
ico
dds
pkm
heif
jpegxr





*/

#ifndef ADCIL_H
#define ADCIL_H

#include "source/adcil_types.h"

#include <stdio.h>



void adcilInit(int a_workers);//creates thread workers
void adcilFinish();//needed if workers are created or any streaming is ongoing

//loads a image given
AdImageError adLoadImage(const char *a_file, const int a_filenameLength, AdImage* a_destImage, int a_destFormat);
AdImageError adLoadImagePointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, int a_destFormat);
AdImageError adSaveImage(const char *a_file, const int a_filenameLength, const AdImage* a_image, const void* a_settings);
AdImageError adSaveImagePointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_settings);


int adStreamInImage(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage, int a_destFormat);

//update stream in image with more data
int adStreamInUpdate(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage);
int adStreamOutImage(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_setting);


#endif //ADCIL
