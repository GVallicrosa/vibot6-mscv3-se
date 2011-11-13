/*
 * nhs.h
 *
 * This module is to normalise IHLS images.
 *
 * The main function to be called from outside is convert_ihls_to_nhs,
 * which returns a binary image indicating road signs in an image.
 *
 */

#ifndef NHS_H_
#define NHS_H_

#include <cv.h>

using namespace cv;

/**
 * Normalising the hue value.
 */
float
normalise_hue(float hue);

/**
 * This function receives an IHLS image as an argument, and converts it
 * to an binary image. The pixels that are assumed to be part of a road-sign
 * are going to have value 1, and the rest of picture is going to have value 0.
 *
 * The reason that we have a return matrix and not overriding the original
 * RGB image, is to keep the original RGB values for reference.
 *
 * FIXME: both ihls_image and the returned binary images are taken as
 * two dimensional matrices of floating and integer points respectively.
 * This is the starting prototype and can be changed later on to
 * some IMAGE types from some libraries (like OpenCV ...).
 */
Mat
convert_ihls_to_nhs(Mat ihls_image);

#endif /* NHS_H_ */
