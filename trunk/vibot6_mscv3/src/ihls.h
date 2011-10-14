/*
 * ihls.h
 *
 * Header file for IHLS (Improved Hue Luminance Saturation)
 */

#ifndef IHLS_H_
#define IHLS_H_

/**
 * Returning hue value of RGB.
 */
float
retrieve_hue(unsigned int r, unsigned int g, unsigned int b);

/**
 * Returning luminance value of RGB.
 */
float
retrieve_luminance(unsigned int r, unsigned int g, unsigned int b);

/**
 * Returning saturation value of RGB.
 */
float
retrieve_saturation(unsigned int r, unsigned int g, unsigned int b);

#endif /* IHLS_H_ */
