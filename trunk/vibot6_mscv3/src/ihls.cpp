/*
 * TODO: write unit testing for all the functions.
 */
#include "ihls.h"
#include "math_utils.h"

#include <cmath>

#include <iostream>
using namespace std;

float
retrieve_theta(unsigned int r, unsigned int g, unsigned int b);

/**
 * It calculates theta bases on the equation provided in Valentine thesis.
 */
float
retrieve_theta(unsigned int r, unsigned int g, unsigned int b)
{
  float theta;

  // The numerator part of equation
  float numerator = r - (g / 2) - (b / 2);
  // The denominator part of equation
  float denominator = (r * r) + (g * g) + (b * b) - (r * g) - (r * b) - (g * b);
  float temp = numerator / sqrt(denominator);

  theta = acos(temp);

  return theta;
}

/**
 * Calculating the hue value based on the blow formula:
 *
 * H = θ if B <= G
 * H = 360 − θ if B > G
 */
float
retrieve_hue(unsigned int r, unsigned int g, unsigned int b)
{
  float hue;

  if (b <= g)
    {
      hue = retrieve_theta(r, g, b);
    }
  else
    {
      hue = 360.0 - retrieve_theta(r, g, b);
    }

  return hue;
}

/**
 * Luminance is calculated as:
 *
 * L = 0.212R + 0.715G + 0.072B
 */
float
retrieve_luminance(unsigned int r, unsigned int g, unsigned int b)
{
  float luminance = (0.212 * r) + (0.715 * g) + (0.072 * b);

  return luminance;
}

/**
 * Saturation is calculates as below:
 *
 * S = max(R, G, B) − min(R, G, B)
 */
float
retrieve_saturation(unsigned int r, unsigned int g, unsigned int b)
{
  float saturation;
  unsigned int max = get_maximum(r, g, b);
  unsigned int min = get_minimum(r, g, b);

  saturation = max - min;

  return saturation;
}

Mat
convert_rgb_to_ihls(Mat rgb_image)
{
  Mat ihls_image;
  ihls_image = rgb_image.clone();

  for (int i = 0; i < ihls_image.rows; i++)
    {
      for (int j = 0; j < ihls_image.cols; j++)
        {
          // gray-level image
          if (ihls_image.channels() == 1)
            {
              ihls_image.at<uchar> (i, j) = 255;
            }
          // colour image
          else if (ihls_image.channels() == 3)
            {
              unsigned int r = ihls_image.at<Vec3b> (i, j)[2];
              unsigned int g = ihls_image.at<Vec3b> (i, j)[1];
              unsigned int b = ihls_image.at<Vec3b> (i, j)[0];
              float h = retrieve_hue(r, g, b);
              float s = retrieve_luminance(r, g, b);
              float l = retrieve_saturation(r, g, b);
              ihls_image.at<Vec3b> (i, j)[0] = h;
              ihls_image.at<Vec3b> (i, j)[1] = s;
              ihls_image.at<Vec3b> (i, j)[2] = l;
            }
        }
    }

  return ihls_image;
}
