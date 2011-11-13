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
  Mat nhs_image;
  nhs_image = ihls_image.clone();

  for (int i = 0; i < nhs_image.rows; i++)
    {
      for (int j = 0; j < nhs_image.cols; j++)
        {
          // gray-level image
          if (nhs_image.channels() == 1)
            {
              nhs_image.at<uchar> (i, j) = 255;
            }
          // colour image
          else if (nhs_image.channels() == 3)
            {
              unsigned int h = ihls_image.at<Vec3b> (i, j)[2];
              unsigned int s = ihls_image.at<Vec3b> (i, j)[1];
              unsigned int l = ihls_image.at<Vec3b> (i, j)[0];
              // Got it from the matlab code.
              if ((h > 230 && s > 30) || (h < 11 && s > 30))
                {
                  nhs_image.at<Vec3b> (i, j)[0] = 0;
                  nhs_image.at<Vec3b> (i, j)[1] = 0;
                  nhs_image.at<Vec3b> (i, j)[2] = 0;
                }
              else
                {
                  nhs_image.at<Vec3b> (i, j)[0] = 255;
                  nhs_image.at<Vec3b> (i, j)[1] = 255;
                  nhs_image.at<Vec3b> (i, j)[2] = 255;
                }
            }
        }
    }

  return nhs_image;
}
