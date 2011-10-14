/*
 * TODO: write unit testing for all the functions.
 */
#include <iostream>
#include <cmath>

#include "ihls.h"
#include "math_utils.h"

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
