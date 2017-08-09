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

#include "adcil_types.h"

#include <stdio.h>


/**\brief Initializes adcil threads.
 *
 * This function is optinal but must be called if threading or any hardware
 * acceleration is used. adcilFinish() must be called to clean afterwards.
 *
 * \param a_workers number of thread workers to spawn. 0 creates none and -1
 * lets adcil decide.
 * \param a_setting Any setting such as specific hardwre acceleration
 * \returns Returns true on success
 **/
adBool adcilInit(int a_workers, const void* a_settings);//creates thread workers


/**\brief Frees adcil resources.
 *
 * This function only needs to be called if adcilInit is called. It will
 * terminate any ongoing streaming and threads.
 **/
void adcilFinish();

//loads a image given
AdImageError adLoadImage(const char *a_file, const int a_filenameLength, AdImage* a_destImage, const void* a_destFormat);
AdImageError adLoadImagePointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, const void* a_destFormat);
AdImageError adSaveImage(const char *a_file, const int a_filenameLength, const AdImage* a_image, const void* a_settings);
AdImageError adSaveImagePointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_settings);


int adStreamInImage(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage, int a_destFormat);

//update stream in image with more data
int adStreamInUpdate(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage);
int adStreamOutImage(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_setting);


#endif //ADCIL
