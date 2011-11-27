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

// Got it from the Matlab code
#define HUE_MAX 11
#define HUE_MIN 230
#define SAT_MIN 30

/**
 * This function receives an IHLS image as an argument, and converts it
 * to an binary image. The pixels that are assumed to be part of a road-sign
 * are going to have value 1, and the rest of picture is going to have value 0.
 *
 * The reason that we have a return matrix and not overriding the original
 * RGB image, is to keep the original RGB values for reference.
 *
 */
Mat
convert_ihls_to_nhs(Mat ihls_image);

#endif /* NHS_H_ */
