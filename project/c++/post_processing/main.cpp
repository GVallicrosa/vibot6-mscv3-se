 #include "cv.h"
 #include "cxcore.h"
 #include "highgui.h"
 #include "PostProcessing.h"
#include "nhs.h"
#include "ihls.h"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>



void main()
{  
	//RGB to NHS---team P
	Mat image=imread("E:\\VIBOT\\VIBOT-cursuri\\Sem I\\Software Engineering\\C++2011-2012\\(Backup) Road Sign - Gielis Code\\NHS Code\\Images\\Different0038.jpg");
	Mat ihlsImage=convert_rgb_to_ihls(image);
	Mat nhsImage=convert_ihls_to_nhs(ihlsImage);
	cvNamedWindow("input for team Q",0);
	imshow("input for team Q",nhsImage);

	//PostProcessing
	IplImage *x=new IplImage(nhsImage);

	IplImage* pFimage, *pElimImage, *pConvexImage;
	CvSeq * pFirst_contour = NULL;
	
    PostProcessing *p=new PostProcessing(x);
	//Noise removal; morphological operations
	pFimage=p->FilteredImage();
    //Display image
	p->DisplayImage("FilteredImage",pFimage);

	//Object Elimination
	pElimImage=p->ObjectElimination(pFimage);
	p->DisplayImage("Elimination",pElimImage);

	//ConvexHull
	vector<CvSeq*> hullVector;
	pConvexImage=p->ConvexHull(pElimImage,hullVector);
	p->DisplayImage("Convex_Hull",pConvexImage);
	cout<<hullVector.size();

	cvWaitKey(0);
}