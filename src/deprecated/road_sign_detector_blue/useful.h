//*********************************************************************/
//
//					useful.h
//
//*********************************************************************/

#ifndef _USEFUL_
#define _USEFUL_

#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <iostream>

#include "Eigen/Core"
#include "Eigen/LU"
// import most common Eigen types
//USING_PART_OF_NAMESPACE_EIGEN

#include "time.h"
#include "constante.h"

using namespace std;
using namespace Eigen;
extern ofstream logfile;

	// convertion RGB <--> value between [0,1]
	Vector3d DoubleToColor( double d );
	Vector3d DoubleToColorDiscrete( const double d, double n = 20.0);
	Vector3d DoubleToGreyDiscrete( const double d, double n = 20.0);

	double ColorToDouble(Vector3d RGB);

	//OpenGL text display
	void PrintMessage(int,int, const std::string);

    // Read OpenInventor/VRML1.0 file

    //Function for CG Labs II
    void RFunctions();
    void RFunctions2();

    //Export screenshot
    void ScreenShot(std::string Fichiersdfsd);
    void RpUnion(double f1, double f2, vector<double> Df1, vector<double> Df2, double &f, vector<double> &Df);
    void RpIntersection(double f1, double f2, vector<double> Df1, vector<double> Df2, double &f, vector<double> &Df);


void IEEE_DrawingForRGC();
void Nature();

void OBB(vector < Vector2d >, Vector2d &T, Vector2d &R, Vector2d &S);

vector < Vector2d>  GenerateOutlier( vector < Vector2d > , double outlier_percentage = 0.1, double radiusratio = 2);
vector <Vector2d> RemoveData (vector < Vector2d >, Vector3d Plane);
vector <Vector2d> ReadData2D(string filename);

int     LoadJPG ( char* filename, int nb, Vector2d &ImageRes );

#endif



