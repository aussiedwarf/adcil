/*


Other libraries to test performance against



*/

#include "adcil.h"
#include "adbmp.h"

#include <assert.h>
#include <string.h>

int main(int argc, char** argv)
{
  AdImage image;
  memset(&image, 0, sizeof(AdImage));
  AdImageError err = adLoadImage("../mozilla/reftest/bmp/bmpsuite/g/pal8os2.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8-rle.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8.bmp", 0, &image, 0);



  err = adSaveImage("test32.bmp", 0, &image, NULL);
  assert(!err);
  AdBmpFormat formatBmp;
  memset(&formatBmp, 0, sizeof(AdBmpFormat));
  formatBmp.format = AD_IMG_BGR24;

  err = adSaveImage("test24.bmp", 0, &image, &formatBmp);

  assert(!err);

  formatBmp.format = AD_IMG_BGR16;

  err = adSaveImage("test16.bmp", 0, &image, &formatBmp);
  assert(!err);

  formatBmp.format = AD_IMG_BGR8;

  err = adSaveImage("test16.bmp", 0, &image, &formatBmp);
  assert(!err);

  return 0;
}
