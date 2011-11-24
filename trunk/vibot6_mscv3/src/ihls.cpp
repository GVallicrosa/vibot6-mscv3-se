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
  float numerator = r - (g * 0.5) - (b * 0.5);

  // The denominator part of equation
  float denominator = (r * r) + (g * g) + (b * b) - (r * g) - (r * b) - (g * b);
  if (denominator == 0.f)
    {
      printf("DIVIDE BY ZERO!! o_O!\n");
			// TODO: Must be corrected.
      denominator = 200;
    }

  float temp = numerator / sqrtf(denominator);
  theta = acosf(temp) * 28.648f; // acosf(temp) * 180 / 2 / PI (from radian to degree)

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
  if (b <= g)
    {
      return retrieve_theta(r, g, b);
    }
  else
    {
      return 360.0f - retrieve_theta(r, g, b);
    }
}

/**
 * Luminance is calculated as:
 *
 * L = 0.212R + 0.715G + 0.072B
 */
float
retrieve_luminance(unsigned int r, unsigned int g, unsigned int b)
{
  return (0.212f * r) + (0.715f * g) + (0.072f * b);
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
  assert(rgb_image.channels() == 3);

  Mat ihls_image(rgb_image.rows, rgb_image.cols, CV_8UC3);

  for (int i = 0; i < rgb_image.rows; ++i)
    {
      const uchar* rgb_data = rgb_image.ptr<uchar> (i);
      uchar* ihls_data = ihls_image.ptr<uchar> (i);

      for (int j = 0; j < rgb_image.cols; ++j)
        {
          unsigned int b = *rgb_data++;
          unsigned int g = *rgb_data++;
          unsigned int r = *rgb_data++;
          *ihls_data++ = (uchar) retrieve_saturation(r, g, b);
          *ihls_data++ = (uchar) retrieve_luminance(r, g, b);
          // to store the data in 1 byte we need to divide it by 2 (360 is more than 1 byte)
          *ihls_data++ = (uchar) retrieve_hue(r, g, b) * 0.5;
        }
    }

  return ihls_image;
}
