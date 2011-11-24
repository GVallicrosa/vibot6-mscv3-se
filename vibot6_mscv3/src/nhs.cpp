/*
 * nhs.cpp
 */
#include <cmath>

#include "nhs.h"

float
normalise_hue(float hue)
{
  return M_PI;
}

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
          unsigned int s = *ihls_data++;
          unsigned int l = *ihls_data++;
          unsigned int h = *ihls_data++;
          *nhs_data++ = ((h < HUE_MAX || h > HUE_MIN) && s > SAT_MIN) ? 255 : 0;
        }
    }

  return nhs_image;
}
