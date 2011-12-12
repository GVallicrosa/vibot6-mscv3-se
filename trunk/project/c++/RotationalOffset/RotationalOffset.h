/*
Description:    This is the header file for its corresponding cpp file. Provide the class for the module. 

Date Created:   22/11/2011
Date Modified:  
Author:         Team T
*/


#ifndef RotationalOffset_H
#define RotationalOffset_H

#include <iostream>
#include <math.h>
#include <stdlib.h>

using namespace std;

#define cPI 3.141
#define cMAX_POINTS 5

//Dummy Contour class
typedef struct Contour {
	int mX;
	int mY;
	unsigned int nop;
	//Contour *next;
}structContour;

//Interface class
class IRO
{
public:
	virtual float* GetMinRadius(Contour* ContourPtr) = 0;
};

//This structure hold the parameters of the RO
typedef struct Params {
	float mRad;
	float mTheta;
}roParams;

//RO module class
class cRotationalOffset:public IRO
{
	private:
		roParams		mPrm[cMAX_POINTS];
		unsigned int	mNoOfPoints;
		float			*mpOutput;

	public:
		void		CartisanToPolar	(Contour* ContourPtr);
		static int	Compare			(const void *first, const void *second);
		void		SortThetaAscend	(void);
		void		FindMinimum		(void);
		float*		GetMinRadius	(Contour* ContourPtr);

		cRotationalOffset();
	   ~cRotationalOffset();
};

#endif
