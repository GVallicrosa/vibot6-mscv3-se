# include "cv.h"
# include "cxcore.h"
# include "highgui.h"
# include "PostProcessing.h"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>
#include <vector>
#include <algorithm>

using namespace cv;

void main()
{
    Mat img=imread("E:\\VIBOT\\VIBOT-cursuri\\Sem I\\Software Engineering\\C++2011-2012\\(Backup) Road Sign - Gielis Code\\NHS Output\\Different0011.jpg",0);
	PostProcessing p(img);
	p.DisplayImage("InputImage",img);
	
	Mat fimg=p.FilterImage();
	p.DisplayImage("FilteredImage",fimg);

	long int aspectAria=1500;
	double lowRatio=0.25;
	double highRatio=1.3;
	vector<vector<Point> > copyCont;
	Mat eimg=p.Elimination(fimg,copyCont,aspectAria,lowRatio,highRatio);
	p.DisplayImage("Elimination",eimg);

	vector<vector<Point> >hull( copyCont.size() );
	Mat himg=p.Convex(eimg,hull,copyCont);
	p.DisplayImage("ConvexHull",himg);

	waitKey(0);
}