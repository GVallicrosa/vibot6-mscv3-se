#include "PostProcessing.h"

//Constructor
PostProcessing::PostProcessing(IplImage *filename)
{
	image=filename;
}

//Destructor
PostProcessing::~PostProcessing()
{
	cvReleaseImage(&image);
}



//NAME OF FUNCTION: DisplayImage
//PURPOSE:
//    The function will create a window, giving it a name, 
//    and will dislay the image in this window.
//INPUT PARAMETERS:
//     name         type     value/reference               description
//--------------------------------------------------------------------------------
//     name         char*        value               the name of the window
//    image       IplImage*      value          the image that we want to display 
//
//OUTPUT PARAMETERS:
//     name         type     value/reference               description
//--------------------------------------------------------------------------------
//                             NO OUTPUT
//

void PostProcessing::DisplayImage(char *name,IplImage *image)
{
	cvNamedWindow(name,0);
	cvShowImage(name,image);
}

//NAME OF FUNCTION: FilteredImage
//PURPOSE:
//    The function will eliminate the noise in the image, using morphological 
//    operations and the median filer. First the source image is dilated,
//    after the contours are filled, the resulted image is eroded and at the 
//    end the image is smooted by the median filter. 
//INPUT PARAMETERS:
//     name         type          value/reference               description
//--------------------------------------------------------------------------------
//                             NO INPUT
//
//OUTPUT PARAMETERS:
//     name         type          value/reference               description
//--------------------------------------------------------------------------------
//    fimage      IplImage*           value                  the filtered image
//
IplImage* PostProcessing::FilteredImage()
{
   //Morphological operations
   //dilate
   IplConvKernel* sd=cvCreateStructuringElementEx(4,4,0,0,CV_SHAPE_ELLIPSE); //structuring element used for dilate and erode
   IplImage *dimage=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U ,1);  //destination image, where the image after dilation will be placed
   cvDilate(image,dimage,sd);
	
   //find the contours  
   CvMemStorage * storage = cvCreateMemStorage(0);
     // cvThreshold(dimage,dimage,200,255,CV_THRESH_BINARY);
   CvSeq* contour=NULL;
   IplImage *dest=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U ,1); //destination image, image after filling the contours
   cvZero(dest); //make all image black
   cvFindContours(dimage,storage,&contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));//finds the contours
   
   //fill the contours
   while (contour != NULL){
		cvDrawContours(dest,contour,cvScalarAll(255),cvScalarAll(255),-1,CV_FILLED,8);
		contour=contour->h_next;
   }

   //erode
   IplImage *eimage=cvCreateImage(cvGetSize(dest),IPL_DEPTH_8U ,1);  //destination image, where the image after erosion will be placed
   cvErode(dest,eimage,sd); 
   
   //Filter using Median Filter
   IplImage *fimage=cvCreateImage(cvGetSize(eimage),IPL_DEPTH_8U ,1); //destination image
   for (int i=0;i<5;i++) { 
	   cvSmooth(eimage, fimage, CV_MEDIAN,5,0,0,0);}

   return fimage;
}

//NAME OF FUNCTION: ObjectElimination
//PURPOSE:
//    The function will eliminate the small objects in the image.
//    First the area of the image is computed. Secondly, for each contour
//    its area is computed and also the the aspect ratio (ration of the 
//    width and the height) of an object. If an object has area less than 
//    1/1500 of image size, and the aspect ratio outside the range of 0.25 and 1.3 then
//    we consider the region as insignificant object, therefore it is eliminated.
//INPUT PARAMETERS:
//     name         type          value/reference               description
//--------------------------------------------------------------------------------
//    fimage      IplImage*           value                 the filtered image, the output 
//                                                           image of the previous method
//
//OUTPUT PARAMETERS:
//     name         type          value/reference               description
//--------------------------------------------------------------------------------
//     dest      IplImage*           value                  the image obtained after 
//                                                      elimination of the small objects
//
IplImage* PostProcessing::ObjectElimination(IplImage *fimage)
{
	//area of the hole image 
	double area=image->height*image->width;
	   cout<<area/1500<<endl;
	   cout<<endl;
    IplImage *dest=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U ,1); //the output image
	dest=cvCloneImage(fimage);   //copy of the input image 

   //find the contours in the new image
   CvMemStorage * storage = cvCreateMemStorage(0);
   CvSeq * first_contour = NULL;
   cvFindContours(fimage,storage,&first_contour, sizeof(CvContour),  CV_RETR_LIST, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
   
   // for each contour
   CvSeq* result;
	while (first_contour != NULL)
 	{
		//area of the contour
		double areaRegion=fabs(cvContourArea(first_contour, CV_WHOLE_SEQ));

		//associate one rectangle for each contour to calculate the height and width 
		CvBox2D rect=cvMinAreaRect2(first_contour,NULL);
		//CvRect rect=cvBoundingRect(first_contour,1);
		//the ratio
		double ratio=rect.size.width/rect.size.height;
		//double areaRegion=rect.size.width*rect.size.height;
		cout<<ratio<<" "<<areaRegion;
		      //double ratio=rect.width/rect.height;
        // checks the area and the ratio and eliminates the contours that do not respect the rules
		if (((areaRegion<area/1500) || (ratio<0.25 || ratio>1.3)))
		{
			  cvDrawContours(dest,first_contour,cvScalarAll(0),cvScalarAll(0),-1,CV_FILLED,8);
			  cout << "deleted";
		}
		cout << endl;
		
		first_contour=first_contour->h_next;
		
	}
    //returns the image after elimination
	return dest;
}

//NAME OF FUNCTION: ConvexHull
//PURPOSE:
//    The function will recover the shape of the segmented figures .
//    The contours of the shape is found and then the function 'convexHull2'
//    is used. 
//INPUT PARAMETERS:
//     name         type          value/reference              description
//------------------------------------------------------------------------------------
//    image      IplImage*            value            the image after the elimination   
//                                                         of the small shapes 
// hull_vector    vector            reference        the memory storage, for each contour
//                                                   the points of  the convex hull will
//                                                             be memorated
//OUTPUT PARAMETERS:
//     name         type          value/reference               description
//-------------------------------------------------------------------------------------
//     dest      IplImage*           value           the image obtained after elimination of 
//                                                          the recovery of the shape
//

IplImage* PostProcessing::ConvexHull(IplImage *image,vector<CvSeq*>&hull_vector)
{
   
	IplImage *dest=cvCreateImage(cvGetSize(image),IPL_DEPTH_8U ,1); //destination image
	cvZero(dest);
	//finds the new contours 
	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq * first_contour = NULL;
	cvFindContours(image,storage,&first_contour, sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE, cvPoint(0,0));
    
	CvSeq* hull=NULL;
	//for each contour
	while (first_contour!=NULL)
	{
         //compute the convex hull
		 hull=cvConvexHull2(first_contour,0,CV_CLOCKWISE,1);
		 // store the coordinates of the convex hull
		 hull_vector.push_back(hull);

		 //draws the region in a new picture
		 cvDrawContours(dest,hull,cvScalarAll(255),cvScalarAll(255),-1,CV_FILLED,8);
		 first_contour=first_contour->h_next;		 

	}
   
   //returns the new image 
   return dest;
}

