/*******************************************************************************
Copyright (c) 2018 Eden Harris

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgment in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*******************************************************************************/

#ifndef AD_PLATFORM_H
#define AD_PLATFORM_H

/*

		
x86
	x64
	x86_64
arm
	arm_64
windows
	win
	win rt
	win phone
linux
	linux
	android
apple
	osx
	ios
bsd

gcc
	gcc
	mingw32
	mingw64
llvm/clang
intel
msvc


X86		//defined if x32 or x64 compatible
X86_32
X86_64
ARM
ARM_32
ARM_64
POWERPC
POWERPC_32
POWERPC_64

ANDROID
LINUX
WINDOWS
OSX
IOS

GCC
CLANG
MSVC
INTEl

http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html

http://nadeausoftware.com/articles/2012/10/c_c_tip_how_detect_compiler_name_and_version_using_compiler_predefined_macros

Clang also defines the __GNUC__ macros
#if defined __clang__  The Clang frontend to LLVM (Clang)
#define COMPILER "Clang"
#elif defined __llvm__  The GCC frontend to LLVM (LLVM-GCC)
#define COMPILER "LLVM-GCC"
#elif defined __GNUC__  The GNU GCC compiler
#define COMPILER "GCC"
#else
#define COMPILER "Unknown"
#endif
	

*/

//Compiler
#if defined(__clang__)  //must detect before gnuc as clang also defines it
  #define AD_CLANG 1
#elif defined(__ICC) || defined(__INTEL_COMPILER) //must detect before gnuc as intel also defines it
  #define AD_INTEL_COMPILER 1
#elif defined(__GNUC__)
  #define AD_GNUC 1
#elif defined(_MSC_VER)
  #define AD_MSVC 1
#endif

#if defined(__x86_64__) || defined(_M_X64) || defined(__i386) || defined(_M_IX86)
  #define AD_X86 1
#endif

#if defined(__x86_64__) || defined(_M_X64)
  #define AD_X86_64 1
  #define AD_64BIT 1
#elif defined(__i386) || defined(_M_IX86)
  #define AD_X86_32 1
#elif defined(__arm__) || defined(_M_ARM)
  #define AD_ARM 1
  #if defined(__aarch64__)
    #define AD_ARM_64 1
    #define AD_64BIT 1
  #else
    #define AD_ARM_32 1
  #endif
#endif

#if defined(_WIN32)
  #define AD_WINDOWS 1
  #define AD_DESKTOP 1
#elif defined (__ANDROID__)
  #define AD_ANDROID 1
  #define AD_MOBILE 1
#elif defined(__linux__)
  #define AD_LINUX 1
  #define AD_DESKTOP 1
#elif defined(__APPLE__)
  #define AD_APPLE 1
  #if defined(TARGET_OS_IPHONE)
    #define AD_IOS 1
    #define AD_MOBILE 1
  #else// TARGET_OS_MAC
	  #define AD_OSX 1
	  #define AD_DESKTOP 1
	#endif
#endif

#if defined(AD_LINUX) && defined(AD_ARM)
  #define AD_RASPI 1
  #define AD_DESKTOP 1
#endif

#if defined(__EMSCRIPTEN__)
  #define AD_EMSCRIPTEN 1
  #define AD_BROWSER 1
#endif

#ifndef AD_64BIT
  #define AD_64BIT 0
  #define AD_32BIT 1
#else
  #define AD_32BIT 0
#endif

//define anything not defined already as 0
//compilers

#ifndef AD_CLANG
  #define AD_CLANG 0
#endif
#ifndef AD_INTEL_COMPILER
  #define AD_INTEL_COMPILER 0
#endif
#ifndef AD_GNUC
  #define AD_GNUC 0
#endif
#ifndef AD_MSVC
  #define AD_MSVC 0
#endif

//arch

#ifndef AD_X86
  #define AD_X86 0
#endif
#ifndef AD_X86_64
  #define AD_X86_64 0
#endif
#ifndef AD_X86_32
  #define AD_X86_32 0
#endif

#ifndef AD_ARM
  #define AD_ARM 0
#endif
#ifndef AD_ARM_64
  #define AD_ARM_64 0
#endif
#ifndef AD_ARM_32
  #define AD_ARM_32 0
#endif

//OS

#ifndef AD_WINDOWS
  #define AD_WINDOWS 0
#endif
#ifndef AD_LINUX
  #define AD_LINUX 0
#endif
#ifndef AD_ANDROID
  #define AD_ANDROID 0
#endif
#ifndef AD_OSX
  #define AD_OSX 0
#endif
#ifndef AD_IOS
  #define AD_IOS 0
#endif
#ifndef AD_APPLE
  #define AD_APPLE 0
#endif
#ifndef AD_RASPI
  #define AD_RASPI 0
#endif

#ifndef AD_DESKTOP
  #define AD_DESKTOP 0
#endif
#ifndef AD_MOBILE
  #define AD_MOBILE 0
#endif

#ifndef AD_EMSCRIPTEN
  #define AD_EMSCRIPTEN 0
#endif
#ifndef AD_BROWSER
  #define AD_BROWSER 0
#endif


#if AD_MSVC
  #define AD_USE_WINTHREADS 1
#endif

//c11 threads dont exist on glibc < 2.28. ubuntu 18.04 has 2.27
#if AD_LINUX
  #define AD_USE_PTHREADS 1
#endif

#ifndef AD_USE_PTHREADS
  #define AD_USE_PTHREADS 0
#endif

#ifndef AD_USE_WINTHREADS
  #define AD_USE_WINTHREADS 0
#endif

#ifndef AD_HANDLE_MEMORY
  #define AD_HANDLE_MEMORY 0
#endif

#if !AD_APPLE && !AD_BROWSER
  #define AD_ENABLE_VULKAN 1
#endif

#ifndef AD_ENABLE_VULKAN
  #define AD_ENABLE_VULKAN 0
#endif

#if AD_WINDOWS
  #define AD_ENABLE_DIRECTX 1
#endif

#if AD_APPLE
  #define AD_ENABLE_METAL 1
#endif

#if AD_ANDROID || AD_IOS || AD_BROWSER || AD_ARM
  #define AD_ENABLE_OPENGLES 1
#endif

#ifndef AD_ENABLE_DIRECTX
  #define AD_ENABLE_DIRECTX 0
#endif

#ifndef AD_ENABLE_METAL
  #define AD_ENABLE_METAL 0
#endif

#ifndef AD_ENABLE_OPENGLES
  #define AD_ENABLE_OPENGLES 0
#endif

//only enable opengl if not using opengles
#if !AD_ENABLE_OPENGLES
  #define AD_ENABLE_OPENGL 1
#endif

#ifndef AD_ENABLE_OPENGL
  #define AD_ENABLE_OPENGL 0
#endif

#if AD_CLANG
#define AD_PADDING(a) \
_Pragma("GCC diagnostic push") \
_Pragma("GCC diagnostic ignored \"-Wunused-private-field\"") \
char _padding[a]; \
_Pragma("GCC diagnostic pop")
#else
#define AD_PADDING(a)
#endif

#define AD_UNUSED(x) (void)(x)

#endif	//AD_PLATFORM_H

