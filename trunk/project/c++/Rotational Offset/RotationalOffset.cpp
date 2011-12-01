/*
Description:    This module find the rotational offset required by Gilies Curve.
				Input:  Contour Pointer
				Output: Array of rotational offset

Date Created:   22/11/2011
Date Modified:  
Author:         Team T
*/


#include "RotationalOffset.h"

//Constructor
cRotationalOffset::cRotationalOffset()
{
		mpOutput	= new float[cMAX_POINTS];
		mNoOfPoints  = 0;
		memset(reinterpret_cast<void *> (mPrm) , 0 , sizeof(roParams) * cMAX_POINTS);
}

//Destructor
cRotationalOffset::~cRotationalOffset()
{
}


void cRotationalOffset::CartisanToPolar(Contour* ContourPtr)
{
	unsigned int i;

	int vXc			= 0;
	int vYc			= 0;
	int vX			= 0;
	int vY			= 0;
	mNoOfPoints		= ContourPtr->nop;
	Contour* CPtr	= ContourPtr;
	
	//Calculate the Centre of Mass(COM)of the contour.
	for (i = 0; i < mNoOfPoints; i++) {
		vXc			+= ContourPtr->mX;
		vYc			+= ContourPtr->mY;
		//ContourPtr	=  ContourPtr->next;
		ContourPtr++;//change here...above one is correct
	}

	//Normalise the COM
	vXc /= mNoOfPoints;
	vYc /= mNoOfPoints;

	//Restore the base pointer.
	ContourPtr = CPtr;
	
	//Translation of the origin to COM and convert from Cartisian to Polar co-ordinates
	for (i = 0; i < mNoOfPoints; i++) {
		vX		 = ContourPtr->mX - vXc;
		vY		 = ContourPtr->mY - vYc;
		mPrm[i].mRad = static_cast<float> (sqrt(static_cast<double>(vX*vX) + static_cast<double>(vY*vY)));
	
		mPrm[i].mTheta = static_cast<float> (atan2(static_cast<double> (vY), static_cast<double> (vX))); /*Gives the output in range -PI to PI */
		
		//if theta lies in 3rd or 4th quadrant then make it in range of 0 - 2 * pi, add 2*PI to all theta values.
		if(vX < 0 && vY < 0 || (vX > 0 && vY < 0)) 
			mPrm[i].mTheta += 2 * static_cast<float> (cPI);		
		//ContourPtr = ContourPtr->next;
		ContourPtr++;//change here...above one is correct
	}
}

//Swap the structure
int cRotationalOffset::Compare( const void *first, const void *second )
{
	roParams* object_1 = reinterpret_cast<roParams*> (const_cast<void*> (first));
	roParams* object_2 = reinterpret_cast<roParams*> (const_cast<void*> (second));

	if (object_1->mTheta > object_2->mTheta)
		return 1;
	else if (object_1->mTheta < object_2->mTheta)
		return -1;
	else
		return 0;
}


void cRotationalOffset::SortThetaAscend(void)
{
	/* Sort using inbuilt Quicksort algorithm in stdlib.h */
	qsort(mPrm, mNoOfPoints, sizeof(roParams), Compare);
}


void cRotationalOffset::FindMinimum(void)
{
	float vCurr			= 0;
	float vPrev			= 0;
	float vPrev2		= 0;
	float vNext			= 0;
	float vNext2		= 0;
	unsigned int i		= 0;
	unsigned int count	= 0;

	for (i = 2; i < mNoOfPoints - 2; i++) {
		vCurr = mPrm[i].mRad;
		vPrev = mPrm[i-1].mRad; vPrev2 = mPrm[i-2].mRad;
		vNext = mPrm[i+1].mRad; vNext2 = mPrm[i+2].mRad;

		if(vCurr < vPrev && vCurr < vPrev2 && vCurr < vNext && vCurr < vNext2) {
			mpOutput[count] = mPrm[i].mTheta;
			count++;
		}
	}
}


float* cRotationalOffset::GetMinRadius(Contour* ContourPtr)
{
	CartisanToPolar(ContourPtr);
	SortThetaAscend();
	FindMinimum();
	return(mpOutput);
}

