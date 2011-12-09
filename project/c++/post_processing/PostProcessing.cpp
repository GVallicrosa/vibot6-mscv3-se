#include "PostProcessing.h"

//constructor
PostProcessing::PostProcessing(Mat filename)
{
	image=filename;
}

//destructor
PostProcessing::~PostProcessing()
{

}

//NAME OF FUNCTION: DisplayImage
//PURPOSE:
//    The function will create a window, giving it a name, 
//    and will dislay the image in this window.
//INPUT PARAMETERS:
//     name         type     value/reference               description
//--------------------------------------------------------------------------------
//     name         char*        value               the name of the window
//    image          Mat      value          the image that we want to display 
//
//OUTPUT PARAMETERS:
//     name         type     value/reference               description
//--------------------------------------------------------------------------------
//                             NO OUTPUT
//
void PostProcessing::DisplayImage(char *name, Mat img)
{
	namedWindow(name,0);
	imshow(name,img);
}

//NAME OF FUNCTION: FilterImage
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
//      dst          Mat             value                  the filtered image

Mat PostProcessing::FilterImage()
{
	//Morphological operation
    //dilate
	Mat sd=getStructuringElement(MORPH_CROSS,Size(4,4)); //structuring element used for dilate and erode
	dilate(image,image,sd);

    Mat dst=image.clone();                                //destination image; copy of the original image
	threshold(image,image,254,255,CV_THRESH_BINARY);      // make the image binary 

	// find the contours of the objects
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(image, contours,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
	
	//draw the filled contours to the destination image
	Scalar color(255,255,255);
	drawContours(dst, contours, -1, color, CV_FILLED, 8);

	//Morphological operations
    //dilate
	erode(dst,dst,sd);

	//eliminate nois using a median filter
	for (int i=0;i<5;i++)
	   medianBlur(dst,dst,5);

	//output
	return dst;
}

//NAME OF FUNCTION: Elimination
//PURPOSE:
//    The function will eliminate the small objects in the image.
//    First the area of the image is computed. Secondly, for each contour
//    its area is computed and also the the aspect ratio (ration of the 
//    width and the height) of an object. If an object has area less than 
//    1/1500 of image size, and the aspect ratio outside the range of 0.25 and 1.3 then
//    we consider the region as insignificant object, therefore it is eliminated.
//INPUT PARAMETERS:
//     name         type                 value/reference               description
//---------------------------------------------------------------------------------------
//    fimage         Mat                     value          the filtered image, the output 
//                                                           image of the previous method
//   copyCont  vector<vector<Point> >      reference         the contours of the objects
//
//OUTPUT PARAMETERS:
//     name         type          value/reference               description
//--------------------------------------------------------------------------------
//     dest         Mat          value                     the image obtained after 
//                                                      elimination of the small objects
//
Mat PostProcessing::Elimination(Mat fimage,vector<vector<Point> > &copyCont)
{
	long int area=image.size().area();                   //area of the original image
	    cout<<"Aria/1500: "<<area/1500<<endl;
	
	Mat dst=Mat::zeros(fimage.rows,fimage.cols,CV_8U);   //destination image  
	
	threshold(fimage,fimage,254,255,CV_THRESH_BINARY);   //make image binary

	//find the new contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(fimage,contours,hierarchy,CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	     cout<<"Dimensiune: "<<contours.size()<<endl;
    
    //for each contour   
	for (int i=0;i<contours.size();i++)
	   {
		
		 //  RotatedRect b=minAreaRect(Mat(contours[i]));       //associate a rectangle to calculate the width and height
		 	Rect b=boundingRect(Mat(contours[i]));

		  
		   //double ratio=b.size.width/b.size.height;          // compute the aspect ratio 
		 double ratio=(double)b.width/(double)b.height;
			 
		   //long int areaRegion=b.size.area();
			       long int areaRegion=b.area();

			
			         cout<<"Ratie: "<<ratio<<" Area: "<<areaRegion<<endl;
			
			if ((areaRegion<area/1500) || ((ratio>1.3) || (ratio<0.25)))  //conditions for eliminating the objects
			{
				 cout<<"sters de la pozitia "<<i<<endl;
			}
			else
				copyCont.push_back(contours[i]);	
			
	   }

	   cout<<copyCont.size()<<endl;
   
	Scalar color(255,255,255);
	drawContours(dst,copyCont, -1, color, CV_FILLED, 8);

	//return image after elimination
    return dst;
}

//NAME OF FUNCTION: Convex
//PURPOSE:
//    The function will recover the shape of the segmented figures .
//    The contours of the shape is found and then the function 'convexHull2'
//    is used. 
//INPUT PARAMETERS:
//     name         type          value/reference              description
//------------------------------------------------------------------------------------
//   eimage         Mat               value            the image after the elimination   
//                                                         of the small shapes 
//   hull     vector<vector<Point>   reference        the memory storage, for each contour
//                                                   the points of  the convex hull will
//                                                             be memorated
// copyCont   vector<vector<Point>   reference          the contours of the objects
//
//
//OUTPUT PARAMETERS:
//     name         type          value/reference               description
//-------------------------------------------------------------------------------------
//     dest         Mat                value           the image obtained after elimination of 
//                                                            the recovery of the shape
//

Mat PostProcessing::Convex(Mat eimage,vector<vector<Point> >&hull,vector<vector<Point> > &copyCont)
{
    	 Mat dest = Mat::zeros( image.size(), CV_8U ); // the output image
	     for (int i=0;i<copyCont.size();i++)
		 {
			 convexHull(Mat(copyCont[i]),hull[i]);    //convex hull operation
		 }
        
		

       for( int i = 0; i<copyCont.size(); i++ )   //drawing the points for convex hull
        {
			for (int j=0;j<hull[i].size();j++)
			{
			Scalar color = Scalar(255, 0, 0);
			     //drawContours( drawing, copyCont, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			//drawContours( dest, hull, i, color, 1, CV_AA, vector<Vec4i>(), 0, Point() );
			 circle(dest,hull[i][j],2,color,1,8);
			}
        };
		
		return dest;

}