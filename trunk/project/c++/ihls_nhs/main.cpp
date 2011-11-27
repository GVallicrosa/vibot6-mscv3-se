/*
 * This is just a sample main file for IHLS and NHS
 * module. If you want to test this module, please change
 * the file_path and run the main file.
 */
#include <cv.h>
#include <highgui.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "ihls.h"
#include "nhs.h"

using namespace cv;

int
main(int argc, char** argv)
{
  Mat image;
  char *file_path = "Your File Path";

  image = imread(file_path);

  namedWindow("Display Image", CV_WINDOW_AUTOSIZE);

  Mat ihls_image = convert_rgb_to_ihls(image);
  Mat nhs_image = convert_ihls_to_nhs(ihls_image);

  imshow("Display Image", nhs_image);

  waitKey(0);

  return 0;
}

