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
#include <iostream>

#include "ihls.h"
#include "nhs.h"

using namespace cv;

void help();

int
main(int argc, char** argv)
{
  if( argc != 2 )
    {
      help();
      return -1;
    }
    
  Mat image = imread(argv[1]);
 
  if( image.channels() != 3 )
    {
      std::cerr << "image.channels() != 3\n";
      return -1;
    }
  
  namedWindow("Display Image", CV_WINDOW_AUTOSIZE);

  Mat ihls_image = convert_rgb_to_ihls(image);
  Mat nhs_image = convert_ihls_to_nhs(ihls_image);

  imshow("Display Image", nhs_image);

  waitKey(0);

  return 0;
}

/**
 * help
 */
void help()
{
    std::cout << "Usage: ./ihls_nhs <img>\n";
}
