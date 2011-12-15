# include "cv.h"
# include "cxcore.h"
# include "highgui.h"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace std;
using namespace cv;

class PostProcessing
{
	Mat image;
public:
	//Constructor and destructors
	PostProcessing(Mat);
    ~PostProcessing();

	//Methods
	void DisplayImage(char*, Mat);
	Mat FilterImage();
	Mat Elimination(Mat,vector<vector<Point> >&,long int, double, double);
	Mat Convex(Mat, vector<vector<Point> >&hull,vector<vector<Point> > &copyCont);
};