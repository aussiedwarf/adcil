/*
AussieDwarf C Image Library

A crossplatform c library for encoding and decoding many common image formats.

Goal for the library is to work on Windows, Macos, Linux, Android, iOS, x64, x64, arm and 
arm64. The library does not depend on any third party libraries and shall be as fast as 
possible.

compatible c99 (20 years), c++03 (10 years)

Compilers: gcc, clang, visual studio

bmp
ico
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
heif/heic
jpegxr

software testing
MacOS Preview
Windows Viewer
MS Paint XP-10
IE 6-11
Edge
Firefox
Chrome
Safari
Gimp
Paint.net



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
 * \param a_memoryLimit The number of bytes to limit adcil to use internally.
 * Adcil uses large buffers when encoding and decoding and this setting limits
 * these buffers such that their total size will not go above this. Setting to
 * 0 uses default vaule. Current default is a little over 1gb (enough for a
 * 16kx16k rgba image).
 * \param a_setting Any setting such as specific hardwre acceleration
 * \returns Returns true on success
 **/
//adBool adcilInit(const int a_workers, const adUint64 a_memoryLimit, const void* a_settings);//creates thread workers


/**\brief Frees adcil resources.
 *
 * This function only needs to be called if adcilInit is called. It will
 * terminate any ongoing streaming and threads.
 **/
//void adcilFinish();

/**
 * \param a_file path to file
 * \param a_filenameLength length of file or 0 if null terminated
 **/
AdImageError adLoadImage(const char *a_file, const int a_filenameLength, AdImage* a_destImage, const void* a_destFormat);
AdImageError adLoadImagePointer(const unsigned char *a_srcImage, size_t a_size, AdImage* a_destImage, const void* a_destFormat);
AdImageError adSaveImage(const char *a_file, const int a_filenameLength, const AdImage* a_image, const void* a_settings);
AdImageError adSaveImagePointer(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_settings);

void adFreeImage(AdImage* a_image);


//int adStreamInImage(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage, int a_destFormat);

//update stream in image with more data
//int adStreamInUpdate(const unsigned char *a_srcImage, size_t a_srcSize, unsigned char** a_destImage);
//int adStreamOutImage(unsigned char** a_file, size_t* a_size, const AdImage* a_image, const void* a_setting);


#endif //ADCIL
