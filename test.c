/*


Other libraries to test performance against



*/

#include "adcil.h"
#include "adbmp.h"

#include <assert.h>
#include <string.h>

AdImageError testImageBmp(const char* a_file, const char* a_saveName)
{
  AdImage image;
  AdImageError err;
  err = adLoadImage(a_file, 0, &image, 0);
  assert(!err);

  err = adSaveImage(a_saveName, 0, &image, NULL);
  assert(!err);

  AdImage image2;
  err = adLoadImage(a_saveName, 0, &image2, 0);
  assert(!err);

  for(int i = 0; i < image.size; ++i){
    assert(image.pixels[i] == image2.pixels[i]);
  }


  adFreeImage(&image2);
  adFreeImage(&image);

  return err;
}

int main(int argc, char** argv)
{
  argc;
  argv;

  testImageBmp("../samples/rgb8_w3x.bmp", "test.bmp");
  testImageBmp("../samples/rgb8_w4x.bmp", "test.bmp");
  testImageBmp("../samples/rgb16.bmp", "test.bmp");
  testImageBmp("../samples/rgb24_w3x.bmp", "test.bmp");
  testImageBmp("../samples/rgb24_w4x.bmp", "test.bmp");
  testImageBmp("../samples/rgbx16.bmp", "test.bmp");
  testImageBmp("../samples/rgbx32.bmp", "test.bmp");

  testImageBmp("../samples/rgb16n.bmp", "test.bmp");
  testImageBmp("../samples/rgbx24n.bmp", "test.bmp");
  testImageBmp("../samples/rgbx32n.bmp", "test.bmp");

#if 0


  AdImage image;
  //memset(&image, 0, sizeof(AdImage));
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
#endif
  return 0;
}
