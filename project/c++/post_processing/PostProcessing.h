 /**
 *Description of the class
 */

//INCLUDES
 #include "cv.h"
 #include "cxcore.h"
 #include "highgui.h"
 #include <iostream>
 #include <stdio.h>
 #include <stdlib.h>

using namespace std;

class PostProcessing
{
	IplImage* image;
public:
	//Constructor and destructors
	PostProcessing(IplImage *);
    ~PostProcessing();

	//Methods
	void DisplayImage(char*, IplImage*);
	IplImage *FilteredImage();
	IplImage *ObjectElimination(IplImage*);
	IplImage *ConvexHull(IplImage*,vector<CvSeq*>&);
};