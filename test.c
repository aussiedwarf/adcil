/*


Other libraries to test performance against



*/

#include "adcil.h"

int main()
{
  AdImage image;
  AdImageError err = adLoadImage("samples/test-rgb24.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8-rle.bmp", 0, &image, 0);
  //AdImageError err = adLoadImage("samples/test-rgb8.bmp", 0, &image, 0);

  err = adSaveImage("test.bmp", 0, &image, NULL);

  return 0;
}
