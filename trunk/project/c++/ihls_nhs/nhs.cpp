/*
 * nhs.cpp
 */
#include "nhs.h"
#include <cmath>

Mat
convert_ihls_to_nhs(Mat ihls_image)
{
  assert(ihls_image.channels() == 3);

  Mat nhs_image(ihls_image.rows, ihls_image.cols, CV_8UC1);

  for (int i = 0; i < ihls_image.rows; ++i)
    {
      const uchar *ihls_data = ihls_image.ptr<uchar> (i);
      uchar *nhs_data = nhs_image.ptr<uchar> (i);
      for (int j = 0; j < ihls_image.cols; ++j)
        {
          uchar s = *ihls_data++;
          // Although l is not being used and we could have
          // replaced the next line with ihls_data++
          // but for the sake of readability, we left it as it it.
          uchar l = *ihls_data++;
          uchar h = *ihls_data++;
          *nhs_data++ = ((h < HUE_MAX || h > HUE_MIN) && s > SAT_MIN) ? 255 : 0;
        }
    }

  return nhs_image;
}
