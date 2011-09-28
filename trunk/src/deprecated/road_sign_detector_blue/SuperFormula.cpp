//superformula.cpp
//superformula.cpp
//superformula.cpp

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <sstream>

#include <algorithm>

#include "Constante.h"
#include "useful.h"
#include "SuperFormula.h"
#include "constante.h"
#include "random-standalone.h"
#include <iomanip>

#include <Eigen/Dense>
#include <Eigen/Geometry>
#include <Eigen/LU>
#include <Eigen/Cholesky>
#include <Eigen/SVD>
#include <limits>

int ndata = 0;


using namespace std;

using namespace Eigen;


extern ofstream logfile;
extern Random R1;

extern double a_scale_1,a_offset_1,b_scale_1,b_offset_1,
       n_scale_1,n_offset_1,
       tht_offset_1, tht_scale_1, phi_offset_1, phi_scale_1,
       x_offset_1, x_scale_1, y_offset_1, y_scale_1, z_offset_1, z_scale_1;

extern double RADIALNOISEPERCENTAGE,OPTIMALERRORSTOP, PROBA_MUTATE, RANDOMRADIUS, kmutate, thresholdvariance;


extern double a_scale_2,a_offset_2,b_scale_2,b_offset_2,
       n_scale_2,n_offset_2,
       tht_offset_2, tht_scale_2, phi_offset_2, phi_scale_2,
       x_offset_2, x_scale_2, y_offset_2, y_scale_2, z_offset_2, z_scale_2;


extern int p_scale_1,p_offset_1,q_scale_1,q_offset_1;
extern int p_scale_2,p_offset_2,q_scale_2,q_offset_2;

extern int ITERATIONMAX, nbdata,pop, MAXRESTART;

//---------------------------------------------------------------------
//
//                      Rational 2D Gielis Curves
//
//---------------------------------------------------------------------

void RationalSuperShape2D :: Init( double a, double b, double n1,double n2,double n3,double p, double q , double thtoffset, double phioffset, double xoffset, double yoffset, double zoffset){

     Parameters.clear();

     Parameters.push_back(a);
     Parameters.push_back(b);

     Parameters.push_back(n1);
     Parameters.push_back(n2);
     Parameters.push_back(n3);

     Parameters.push_back(p);
     Parameters.push_back(q);

     Parameters.push_back(thtoffset);
     Parameters.push_back(phioffset);

     Parameters.push_back(xoffset);
     Parameters.push_back(yoffset);
     Parameters.push_back(zoffset);
}

void RationalSuperShape2D :: Init( double a, double b, double n1,double n2,double n3)
{
     Init(
           a, b,     //scale
           n1, n2, n3,  //shape
           Get_p(), Get_q(), //sym
           Get_thtoffset(),0,      //rota
           0,0,0);   //trans
}

RationalSuperShape2D ::	RationalSuperShape2D(){
     Init(
           1, 1,     //scale
           2, 2, 2,  //shape
           4, 1,     //sym
           0,0,      //rota
           0,0,0);   //trans
}



RationalSuperShape2D :: RationalSuperShape2D(double a, double b, double n1,double n2,double n3,double p, double q, double thtoffset, double phioffset, double xoffset, double yoffset, double zoffset){
     Init(a, b, n1, n2, n3, p, q, thtoffset, phioffset,xoffset, yoffset, zoffset);
}

RationalSuperShape2D :: ~RationalSuperShape2D() {

     Parameters.clear();
     PointList.clear();
     FaceList.clear();
     NormalList.clear();
}

double RationalSuperShape2D :: radius ( const double angle ){

   			double tmp_angle = Get_p() * angle * 0.25 / Get_q() ;

			double tmp1( cos(tmp_angle) );
			double tmp2( sin(tmp_angle) );

			//if( tmp1 != 0)
				tmp1 = pow(fabs(tmp1),Get_n2()) / Get_a();
			//if( tmp2 != 0)
                tmp2 = pow(fabs(tmp2),Get_n3()) / Get_b();

			if( tmp1 + tmp2 !=0 )	return( pow( (tmp1 + tmp2), -1.0/Get_n1() ) );
			else				   {cout<<"ERROR RADIUS NULL"<<endl;return 0;}

}

// drawing function

void RationalSuperShape2D :: DrawCurve(double R, double G, double B, const int width , const int sampling, const bool stippleon){
     // curve sampling

     //adjust sampling values to pass through the maximum radius values

     PointList.clear();


    glColor3f(0,1,0);
    glPointSize(10);
    glBegin(GL_POINTS);
    glVertex3f(Get_xoffset(), Get_yoffset(), 0);
    glEnd();

     int sample = sampling;//(sampling - (sampling % (int)(Parameters[5]))) / (int)(Parameters[5]);

     double thtoffset(Get_thtoffset()), gap (2.*PI*Get_q() / (sample)), angle(0);


     for (int  j = 0; j < 1/*Parameters[6]/2*/; j++)
         {
         for( int i=0; i < sample; i++)
              {
                  double R(radius(angle));
                  PointList.push_back(Vector3d(
                    R*cos(angle + thtoffset) + Get_xoffset(),
                    R*sin(angle + thtoffset) + Get_yoffset() ,
                    0));
                  angle = (double) (i)*gap;
              }
          }

     //rendering
     double n = Parameters[6] * sample;

     glLineWidth(width);
     glColor3f(R,G,B);
     if(stippleon)
     {
     glLineStipple(1,0x00FF);
//     glEnable(GL_LINE_STIPPLE);
     }

     glBegin(GL_LINE_STRIP);
     for ( int i = 0; i < PointList.size(); i++)
        {
             glVertex3f(PointList[i][0],PointList[i][1],PointList[i][2]);
        }
     glEnd();

//     glLineStipple(1,0x0000);
     if(stippleon)     glDisable(GL_LINE_STIPPLE);

     PointList.clear();
}

void RationalSuperShape2D :: SetBuffer(int sizebuffer, double **flowerbuffer){

    Vector2d UpCorner(-8,-8);
    Vector2d BottomCorner(8,8);

    double gapx( (BottomCorner[0] - UpCorner[0]) / (sizebuffer-1)), gapy( (BottomCorner[1] - UpCorner[1]) / (sizebuffer-1));

	int i,j;

	for(i=0; i<sizebuffer; i++)
		for(j=0; j<sizebuffer; j++)
		{
        Vector2d P( UpCorner[0] + i*gapx, UpCorner[1] + j*gapy);
        double f = 0;
        //flowerbuffer[i][j] = ImplicitFunction0( P, false, 0 );
        if(flowerbuffer[i][j]<=0) flowerbuffer[i][j]=0.;

		}

	double mini(1e15), maxi(0);

	for(i=0; i<sizebuffer; i++)
		for(j=0; j<sizebuffer; j++)

	{

//        mini = std::min(flowerbuffer[i][j],mini);
		maxi = std::max(flowerbuffer[i][j],maxi);
	}

	for(i=0; i<sizebuffer; i++)
		for(j=0; j<sizebuffer; j++)
		flowerbuffer[i][j] = (flowerbuffer[i][j])/(maxi);





}

void RationalSuperShape2D :: DrawIntensity( const Vector2d UpCorner, const Vector2d BottomCorner, const int sampling){

     glPointSize(5);

     glBegin(GL_POINTS);
     glColor3f(0,1,0);
     glVertex3f( UpCorner[0], UpCorner[1],0);

     glColor3f(1,1,0);
     glVertex3f( BottomCorner[0], BottomCorner[1],0);
     glEnd();

     double **Map = new double*[sampling];
     for (int i=0; i< sampling; i++)Map[i] =  new double[sampling];

     double gapx( (BottomCorner[0] - UpCorner[0]) / (sampling-1)), gapy( (BottomCorner[1] - UpCorner[1]) / (sampling-1));
     double max(0), min(0);

     for ( int i = 0; i < sampling; i++)
          for ( int j=0; j< sampling; j++)
          {
              Vector2d P( UpCorner[0] + i*gapx, UpCorner[1] + j*gapy);
              //Map[i][j] = ImplicitFunction0( P, true, 0);
              max = std :: max (max, Map[i][j]);
              min = std :: min (min, Map[i][j]);
          }

cout << "Min"<< min<< " Max=" << max <<endl;

    glPolygonMode(GL_FRONT,GL_FILL);
     if ( min <=0 && max >= 0)         // easiest case to display else we do not do anything
     {
          glBegin(GL_QUADS);
          for ( int i = 0; i < sampling-1; i++)
              for ( int j=0; j< sampling -1; j++)
              {
                  Vector3d Col1, Col2, Col3, Col4;

                  if ( Map[i][j] >=0) Col1 = DoubleToColor( Map[i][j] / max );
                  else Col1[0] = Col1[1] = Col1[2] = 1 - Map[i][j] / min;

                  if ( Map[i][j+1] >=0) Col2 = DoubleToColor( Map[i][j+1] / max );
                  else Col2[0] = Col2[1] = Col2[2] = 1 - Map[i][j+1] / min;

                  if ( Map[i+1][j+1] >=0) Col3 = DoubleToColor( Map[i+1][j+1] / max );
                  else Col3[0] = Col3[1] = Col3[2] = 1 - Map[i+1][j+1] / min;

                  if ( Map[i+1][j] >=0) Col4 = DoubleToColor( Map[i+1][j] / max );
                  else Col4[0] = Col4[1] = Col4[2] = 1 - Map[i+1][j] / min;

                  glColor3f(Col1[0],Col1[1],Col1[2]);
                  glVertex3f(UpCorner[0] + i*gapx, UpCorner[1] + j*gapy, 0);

                  glColor3f(Col2[0],Col2[1],Col2[2]);
                  glVertex3f(UpCorner[0] + i*gapx, UpCorner[1] + (j+1)*gapy, 0);

                  glColor3f(Col3[0],Col3[1],Col3[2]);
                  glVertex3f(UpCorner[0] + (i+1)*gapx, UpCorner[1] + (j+1)*gapy, 0);

                  glColor3f(Col4[0],Col4[1],Col4[2]);
                  glVertex3f(UpCorner[0] + (i+1)*gapx, UpCorner[1] + j*gapy, 0);


              }
          glEnd();
  }
}


void RationalSuperShape2D :: RandomInit(){
  Set_a  ( R1.uniform() * a_scale_1 + a_offset_1);
  Set_b  ( R1.uniform() * b_scale_1 + b_offset_1);
  Set_n1 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_n2 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_n3 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_p  ( (int) (R1.uniform() * p_scale_1 + p_offset_1 ));
  Set_q  ( (int) (R1.uniform() * q_scale_1 + q_offset_1 ));

  if( Get_q() > Get_p() ) //permute
  {
      int a = int (Get_q());
      int b = int (Get_p());
      Set_p(a);
      Set_q(b);
  }


  Simplify();

  Set_thtoffset( R1.uniform() *  Get_q() * tht_scale_1 / Get_p() + tht_offset_1);
  Set_phioffset( 0 );

  Set_xoffset (R1.uniform() * x_scale_1 + x_offset_1);
  Set_yoffset (R1.uniform() * y_scale_1 + y_offset_1);
  Set_zoffset ( 0 );

}

double RationalSuperShape2D::MinDist(Vector2d P, int n){
       //quick and dirty solution : sample the curve and get the min distance
       double d=1000000;

       double gap = 2.0*PI * Get_q()/n;
       double angle,r,ddum;

       Vector2d   dum(0,0), Pdum(P[0]-Get_xoffset(), P[1]-Get_yoffset());

       for(int i=0; i<n; i++)
       {
               angle = i*gap;
               r = radius(angle);
               dum[0] = cos(angle+Get_thtoffset()) * r;
               dum[1] = sin(angle+Get_thtoffset()) * r;
               ddum = (Pdum-dum).norm();
               if (ddum<d)
               {
                           d = ddum;


               }
       }



       return d;

      // return ImplicitFunction(P, true, 0);
}

void RationalSuperShape2D::Mutate(){
     extern double kmutate;
     double k = kmutate;
     /*
     for (int i=0; i<Parameters.size(); i++)
     {
         Parameters[i] =          R1.gaussian(Parameters[i], 1);
         //gaussian(Parameters[i], Parameters[i]/2.0);
         if (i>=2 && i<5)
         {
                  while (fabs(Parameters[i])< 0.5) Parameters[i] =          R1.gaussian(Parameters[i], 1);
         }

     }
*/
     Parameters[0] =          R1.gaussian(Parameters[0], a_scale_1 * k);
     Parameters[0] = std::min ((double) (a_scale_1 + a_offset_1), std :: max(Parameters[0], (double) a_offset_1));
     Parameters[1] =          R1.gaussian(Parameters[1], b_scale_1 * k);
     Parameters[1] = std::min ((double) (b_scale_1 + b_offset_1), std :: max(Parameters[1], (double) b_offset_1));
     Parameters[2] =          R1.gaussian(Parameters[2], n_scale_1 * k);
     Parameters[2] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters[2], (double) n_offset_1));
     Parameters[3] =          R1.gaussian(Parameters[3], n_scale_1 * k);
     Parameters[3] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters[3], (double) n_offset_1));
     Parameters[4] =          R1.gaussian(Parameters[4], n_scale_1 * k);
     Parameters[4] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters[4], (double) n_offset_1));

     Parameters[7] =          R1.gaussian(Parameters[7], tht_scale_1 * k);
     Parameters[7] = std::min ((double)(tht_scale_1 + tht_offset_1), std :: max((double)Parameters[7], (double) tht_offset_1));

     Parameters[5] = (int)(R1.uniform() * p_scale_1 + p_offset_1);
     do{
                   Parameters[6] = (int)(R1.uniform() * q_scale_1 + q_offset_1);
     }
     while (Get_q() > Get_p());

     Simplify();

     Parameters[8] = 0; //phi
     Parameters[9] =          R1.gaussian(Parameters[9], x_scale_1 * k);
     Parameters[9] = std::min ((double)(x_scale_1 + x_offset_1), std :: max((double)Parameters[9], (double) x_offset_1));
     Parameters[10] =          R1.gaussian(Parameters[10], y_scale_1 * k);
     Parameters[10] = std::min ((double)(y_scale_1 + y_offset_1), std :: max((double)Parameters[10], (double) y_offset_1));
     Parameters[11] = 0;


}

void RationalSuperShape3D::Mutate(){
     extern double kmutate;
     double k = kmutate;

     Parameters1[0] = R1.gaussian(Parameters1[0], a_scale_1 * k);
     Parameters1[0] = std::min ((double) (a_scale_1 + a_offset_1), std :: max(Parameters1[0], (double) a_offset_1));
     Parameters1[1] = R1.gaussian(Parameters1[1], b_scale_1 * k);
     Parameters1[1] = std::min ((double) (b_scale_1 + b_offset_1), std :: max(Parameters1[1], (double) b_offset_1));
     Parameters1[2] = R1.gaussian(Parameters1[2], n_scale_1 * k);
     Parameters1[2] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters1[2], (double) n_offset_1));
     Parameters1[3] = R1.gaussian(Parameters1[3], n_scale_1 * k);
     Parameters1[3] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters1[3], (double) n_offset_1));
     Parameters1[4] = R1.gaussian(Parameters1[4], n_scale_1 * k);
     Parameters1[4] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters1[4], (double) n_offset_1));

     Parameters1[7] = R1.gaussian(Parameters1[7], tht_scale_1 * k);
     Parameters1[7] = std::min ((double)(tht_scale_1 + tht_offset_1), std :: max((double)Parameters1[7], (double) tht_offset_1));

     Parameters1[5] = (int)(R1.uniform() * p_scale_1 + p_offset_1);
     do{
                   Parameters1[6] = (int)(R1.uniform() * q_scale_1 + q_offset_1);
     }
     while (Get_q1() > Get_p1());



     Parameters1[8] = 0; //phi
     Parameters1[9] = R1.gaussian(Parameters1[9], x_scale_1 * k);
     Parameters1[9] = std::min ((double)(x_scale_1 + x_offset_1), std :: max((double)Parameters1[9], (double) x_offset_1));
     Parameters1[10] = R1.gaussian(Parameters1[10], y_scale_1 * k);
     Parameters1[10] = std::min ((double)(y_scale_1 + y_offset_1), std :: max((double)Parameters1[10], (double) y_offset_1));
     Parameters1[11] = 0;

    // second 2d supershape

     Parameters2[0] = R1.gaussian(Parameters2[0], a_scale_1 * k);
     Parameters2[0] = std::min ((double) (a_scale_1 + a_offset_1), std :: max(Parameters2[0], (double) a_offset_1));
     Parameters2[1] = R1.gaussian(Parameters2[1], b_scale_1 * k);
     Parameters2[1] = std::min ((double) (b_scale_1 + b_offset_1), std :: max(Parameters2[1], (double) b_offset_1));
     Parameters2[2] = R1.gaussian(Parameters2[2], n_scale_1 * k);
     Parameters2[2] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters2[2], (double) n_offset_1));
     Parameters2[3] = R1.gaussian(Parameters2[3], n_scale_1 * k);
     Parameters2[3] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters2[3], (double) n_offset_1));
     Parameters2[4] = R1.gaussian(Parameters2[4], n_scale_1 * k);
     Parameters2[4] = std::min ((double)(n_scale_1 + n_offset_1), std :: max((double)Parameters2[4], (double) n_offset_1));

     Parameters2[7] = R1.gaussian(Parameters2[7], tht_scale_1 * k);
     Parameters2[7] = std::min ((double)(tht_scale_1 + tht_offset_1), std :: max((double)Parameters2[7], (double) tht_offset_1));

     Parameters2[5] = (int)(R1.uniform() * p_scale_1 + p_offset_1);
     do{
                   Parameters2[6] = (int)(R1.uniform() * q_scale_1 + q_offset_1);
     }
     while (Get_q2() > Get_p2());

     //Simplify();

     Parameters2[8] = 0; //phi
     Parameters2[9] = R1.gaussian(Parameters2[9], x_scale_1 * k);
     Parameters2[9] = std::min ((double)(x_scale_1 + x_offset_1), std :: max((double)Parameters2[9], (double) x_offset_1));
     Parameters2[10] = R1.gaussian(Parameters2[10], y_scale_1 * k);
     Parameters2[10] = std::min ((double)(y_scale_1 + y_offset_1), std :: max((double)Parameters2[10], (double) y_offset_1));
     Parameters2[11] = 0;

        Simplify();

}

void CrossOver( RationalSuperShape2D g1,  RationalSuperShape2D g2, RationalSuperShape2D &f1, RationalSuperShape2D &f2){
     int pg1 ((int)(g1.Get_p())),
         qg1 ((int)(g1.Get_q())),
         pg2 ((int)(g2.Get_p())),
         qg2 ((int)(g2.Get_q()));

     for ( int i=0; i<g1.Parameters.size();i++)
     {


         if(R1.uniform()<0.5)
         {
            f1.Parameters[i] = g1.Parameters[i];
            f2.Parameters[i] = g2.Parameters[i];
         }
         else
         {
            f2.Parameters[i] = g1.Parameters[i];
            f1.Parameters[i] = g2.Parameters[i];
         }

    }

    if ( f1.Get_p() > f1.Get_q() || f2.Get_p() > f2.Get_q()) // symmetry transfert non possible, reset old values
    {
         f1.Set_p(pg1);
         f1.Set_q(qg1);
         f2.Set_p(pg2);
         f2.Set_q(qg2);

    }

    f1.Simplify();
    f2.Simplify();

}

void CrossOver( RationalSuperShape3D g1,  RationalSuperShape3D g2, RationalSuperShape3D &f1, RationalSuperShape3D &f2){
     //int pg1 ((int)(g1.Get_p())),         qg1 ((int)(g1.Get_q())),         pg2 ((int)(g2.Get_p())),         qg2 ((int)(g2.Get_q()));

     for ( int i=0; i<g1.Parameters1.size();i++)
     {


         if(R1.uniform()<0.5)
         {
            f1.Parameters1[i] = g1.Parameters1[i];
            f1.Parameters2[i] = g1.Parameters2[i];

            f2.Parameters1[i] = g2.Parameters1[i];
            f2.Parameters2[i] = g2.Parameters2[i];
         }
         else
         {
            f1.Parameters1[i] = g2.Parameters1[i];
            f1.Parameters2[i] = g2.Parameters2[i];

            f2.Parameters1[i] = g1.Parameters1[i];
            f2.Parameters2[i] = g1.Parameters2[i];

         }

    }

  // if ( f1.Get_p() > f1.Get_q() || f2.Get_p() > f2.Get_q()) // symmetry transfert non possible, reset old values
  //  {
  //       f1.Set_p(pg1);
  //       f1.Set_q(qg1);
  //       f2.Set_p(pg2);
  //       f2.Set_q(qg2);
  //    }

    f1.Simplify();
    f2.Simplify();

}

void RationalSuperShape2D :: Aff(){
     for ( int i=0; i<Parameters.size(); i++)
     {
         cout << setiosflags(ios::fixed) << setprecision(2)<<Parameters[i] << " ";
     }
     cout << endl;
 }

void Permute(RationalSuperShape2D &RSS1, RationalSuperShape2D &RSS2){
     double dum;
     for (int i=0; i<RSS1.Parameters.size(); i++)
     {
         dum = RSS1.Parameters[i];
         RSS1.Parameters[i] = RSS2.Parameters[i];
         RSS2.Parameters[i] = dum;
     }
}

void Permute(RationalSuperShape3D &RSS1, RationalSuperShape3D &RSS2){
     double dum;
     for (int i=0; i<RSS1.Parameters1.size(); i++)
     {
         dum = RSS1.Parameters1[i];
         RSS1.Parameters1[i] = RSS2.Parameters1[i];
         RSS2.Parameters1[i] = dum;
     }

     for (int i=0; i<RSS1.Parameters2.size(); i++)
     {
         dum = RSS1.Parameters2[i];
         RSS1.Parameters2[i] = RSS2.Parameters2[i];
         RSS2.Parameters2[i] = dum;
     }

     for (int i=0; i<RSS1.Transformations.size(); i++)
     {
         dum = RSS1.Transformations[i];
         RSS1.Transformations[i] = RSS2.Transformations[i];
         RSS2.Transformations[i] = dum;
     }

}
/*
double GA(std::vector <Vector2d > Data, int pop, std::vector<RationalSuperShape2D> &pool, RationalSuperShape2D  &OptimalSolution, int functionused){
    int ngenmax(10000);
    double avgerr(1000);
    int niteration=0;
    int avgerror =100000000;
    int nshuffle = pop*5;
    int i1,i2;
    double dum2;
    std::vector<double> distpool1,distpool2;
    int datasize=Data.size();
    RationalSuperShape2D AVG, VAR;
    double variance;
    extern double thresholdvariance;
    double err;
    int restart = 0;
extern double      errth;

    //storage best solution

    double optimalerror(1e15);

    double relative_improvement = 1;

    //generate random population
    for (int i=0; i<pop; i++)
    {
        RationalSuperShape2D dum;
        dum.RandomInit();
        pool.push_back(dum);
    }

    //while( niteration < ngenmax && avgerror > avgerrorstop)
    for (int k=0; k<ITERATIONMAX && optimalerror > OPTIMALERRORSTOP && restart < MAXRESTART; k++)
    {
          //shuffle pool

          for (int i=0; i<nshuffle; i++) {
              i1 = (int) ((pop-1)*R1.uniform());
              i2 = (int) ((pop-1)*R1.uniform());
              if (i1 != i2)
              {
                     Permute(pool[i1], pool[i2]);
              }
            }

          //mutation

          for(int i=0; i<pop; i++)
          if(R1.uniform() < PROBA_MUTATE)
          {
             pool[i].Mutate();
          }

           //evaluate pool
           distpool1.clear();
           for( int i=0; i< pop; i++)
           {
                Vector2d T(pool[i].Get_xoffset(), pool[i].Get_yoffset());
                double R(pool[i].Get_thtoffset());
                double s = 0;//pool[i].Curv.norm();
                Vector2d Gdum;
                err = 0;
                for (int j=0; j<datasize; j++)
                {
                   Vector2d P(Data[j] - T);
                   Vector2d CanonP(P[0]*cos(R) + P[1]*sin(R), -P[0]*sin(R) + P[1]*cos(R));

                   //if( functionused == 0)   err += pow(pool[i].ImplicitFunction0(CanonP,Gdum),2);
                   //if( functionused == 1)   err += pow(pool[i].ImplicitFunction1(CanonP,Gdum),2);
                   //if( functionused == 2)   err += pow(pool[i].ImplicitFunction2(CanonP,Gdum),2);
                   //if( functionused == 3)   err += pow(pool[i].ImplicitFunction3(CanonP,Gdum),2);
                   //if( functionused == 4)   err += pow(pool[i].ImplicitFunction4(CanonP,Gdum),2);

                }
                //err =  pow(err,pool[i].Get_q());
                distpool1.push_back(
                                    //err);
                                    err + s + sqrt( err*err + s*s));
                                    //err*err);
           }


           //crossover
           for(int i=0; i<pop; i+=2)
           {
                   RationalSuperShape2D F1,F2;
                   CrossOver(pool[i],pool[i+1],F1,F2);
                   pool.push_back(F1);
                   pool.push_back(F2);
           }

           //evaluate pool2

           for( int i=pop; i< 2*pop; i++)
           {
                err = 0;
                Vector2d T(pool[i].Get_xoffset(), pool[i].Get_yoffset());
                double R(pool[i].Get_thtoffset());
                double s =  pool[i].Curv.norm();
                Vector2d Gdum;
                for (int j=0; j<Data.size(); j++)
                {
                   Vector2d P(Data[j] - T);
                   Vector2d CanonP(P[0]*cos(R) + P[1]*sin(R), -P[0]*sin(R) + P[1]*cos(R));

                 //  if( functionused == 0)   err += pow(pool[i].ImplicitFunction0(CanonP, Gdum),2);
                  // if( functionused == 1)   err += pow(pool[i].ImplicitFunction1(CanonP, Gdum),2);
                  // if( functionused == 2)   err += pow(pool[i].ImplicitFunction2(CanonP, Gdum),2);
                  // if( functionused == 3)   err += pow(pool[i].ImplicitFunction3(CanonP, Gdum),2);
                  // if( functionused == 4)   err += pow(pool[i].ImplicitFunction4(CanonP, Gdum),2);

                }
                distpool1.push_back(
                                    //err*s);
                                    err + s + sqrt(err*err + s*s));
                                    //err);
           }


           // sorting arrays

           for( int i=0; i<pool.size(); i++)
           {
                for ( int j=0; j<pool.size(); j++)
                {
                    if(distpool1[i] < distpool1[j])
                    {
                        double tmp = distpool1[i];
                        distpool1[i] = distpool1[j];
                        distpool1[j] = tmp;

                        Permute(pool[i], pool[j]);

                    }
                }
           }


           //evacuate non adapted individuals

           pool.resize((int) (pop) );

           // store best element if needed
           if (distpool1[0] < optimalerror)
           {
              optimalerror = distpool1[0];
              OptimalSolution = pool[0];
           }

           //STATS

           avgerr = 0;
           for(int i=0; i<pop; i++)
           {
                   avgerr += distpool1[i];
           }

           avgerr /= pop;
           cout <<"AVG ERROR = "<<avgerr<<endl;
           cout<<"BEST ERROR FOUND="<<optimalerror<<endl;
           OptimalSolution.Aff();



           //compute AVG
           AVG.Init(0,0, 0,0,0, 0,0, 0,0, 0,0,0);

           for(int i=0; i<pop; i++)
           {
                   AVG.Set_a ( AVG.Get_a()  + pool[i].Get_a()  );
                   AVG.Set_b ( AVG.Get_b()  + pool[i].Get_b()  );

                   AVG.Set_n1( AVG.Get_n1() + pool[i].Get_n1() );
                   AVG.Set_n2( AVG.Get_n2() + pool[i].Get_n2() );
                   AVG.Set_n3( AVG.Get_n3() + pool[i].Get_n3() );

                   AVG.Set_p ( AVG.Get_p()  + pool[i].Get_p()  );
                   AVG.Set_q ( AVG.Get_q()  + pool[i].Get_q()  );

                   AVG.Set_thtoffset ( AVG.Get_thtoffset() + pool[i].Get_thtoffset()  );
                   AVG.Set_phioffset ( AVG.Get_phioffset() + pool[i].Get_phioffset() );

                   AVG.Set_xoffset (AVG.Get_xoffset() + pool[i].Get_xoffset() );
                   AVG.Set_yoffset (AVG.Get_yoffset() + pool[i].Get_yoffset() );
                   AVG.Set_zoffset (AVG.Get_zoffset() + pool[i].Get_zoffset() );

           }

           AVG.Set_a ( AVG.Get_a() /pop);
           AVG.Set_b ( AVG.Get_b() /pop);

           AVG.Set_n1( AVG.Get_n1()/pop);
           AVG.Set_n2( AVG.Get_n2()/pop);
           AVG.Set_n3( AVG.Get_n3()/pop);

           AVG.Set_p ( AVG.Get_p()/pop);
           AVG.Set_q ( AVG.Get_q()/pop);

           AVG.Set_thtoffset( AVG.Get_thtoffset()/pop);
           AVG.Set_phioffset( AVG.Get_phioffset()/pop);

           AVG.Set_xoffset ( AVG.Get_xoffset() / pop);
           AVG.Set_yoffset ( AVG.Get_yoffset() / pop);
           AVG.Set_zoffset ( AVG.Get_zoffset() / pop);


           //Compute Variance
           VAR.Init(0,0, 0,0,0, 0,0, 0,0, 0,0,0);

           for(int i=0; i<pool.size(); i++)
           {
                   VAR.Set_a ( VAR.Get_a()  + pow(AVG.Get_a()  - pool[i].Get_a() , 2) );
                   VAR.Set_b ( VAR.Get_b()  + pow(AVG.Get_b()  - pool[i].Get_b() , 2) );

                   VAR.Set_n1( VAR.Get_n1() + pow(AVG.Get_n1() - pool[i].Get_n1(), 2) );
                   VAR.Set_n2( VAR.Get_n2() + pow(AVG.Get_n2() - pool[i].Get_n2(), 2) );
                   VAR.Set_n3( VAR.Get_n3() + pow(AVG.Get_n3() - pool[i].Get_n3(), 2) );

                   VAR.Set_p ( VAR.Get_p()  + pow(AVG.Get_p()  - pool[i].Get_p() , 2) );
                   VAR.Set_q ( VAR.Get_q()  + pow(AVG.Get_q()  - pool[i].Get_q() , 2) );

                   VAR.Set_thtoffset ( VAR.Get_thtoffset()  + pow(AVG.Get_thtoffset()  - pool[i].Get_thtoffset() , 2) );
                   VAR.Set_phioffset ( VAR.Get_phioffset()  + pow(AVG.Get_phioffset()  - pool[i].Get_phioffset() , 2) );

                   VAR.Set_xoffset ( VAR.Get_xoffset()  + pow(AVG.Get_xoffset()  - pool[i].Get_xoffset() , 2) );
                   VAR.Set_yoffset ( VAR.Get_yoffset()  + pow(AVG.Get_yoffset()  - pool[i].Get_yoffset() , 2) );
                   VAR.Set_zoffset ( VAR.Get_zoffset()  + pow(AVG.Get_zoffset()  - pool[i].Get_zoffset() , 2) );

           }

           VAR.Set_a ( VAR.Get_a() /pop);
           VAR.Set_b ( VAR.Get_b() /pop);

           VAR.Set_n1( VAR.Get_n1()/pop);
           VAR.Set_n2( VAR.Get_n2()/pop);
           VAR.Set_n3( VAR.Get_n3()/pop);

           VAR.Set_p ( VAR.Get_p() /pop);
           VAR.Set_q ( VAR.Get_q() /pop);

           VAR.Set_thtoffset ( VAR.Get_thtoffset() /pop);
           VAR.Set_phioffset ( VAR.Get_phioffset() /pop);

           VAR.Set_xoffset ( VAR.Get_xoffset() /pop);
           VAR.Set_yoffset ( VAR.Get_yoffset() /pop);
           VAR.Set_zoffset ( VAR.Get_zoffset() /pop);


           //normalize Var


           VAR.Set_a(  VAR.Get_a() / a_scale_1);
           VAR.Set_b(  VAR.Get_b() / b_scale_1);
           VAR.Set_n1( VAR.Get_n1() / n_scale_1);
           VAR.Set_n2( VAR.Get_n2() / n_scale_1);
           VAR.Set_n2( VAR.Get_n2() / n_scale_1);
           VAR.Set_n3( VAR.Get_n3() / n_scale_1);
           VAR.Set_p(  VAR.Get_p()  / p_scale_1);
           if (q_scale_1 !=0) VAR.Set_q(  VAR.Get_q()  / q_scale_1);
           if (tht_scale_1 !=0)VAR.Set_thtoffset( VAR.Get_thtoffset() / tht_scale_1);
           if (phi_scale_1 !=0)VAR.Set_phioffset( VAR.Get_phioffset() / phi_scale_1);
           if (x_scale_1 !=0) VAR.Set_xoffset ( VAR.Get_xoffset() / x_scale_1);
           if (y_scale_1 !=0)VAR.Set_yoffset ( VAR.Get_yoffset() / y_scale_1);
           if (z_scale_1 !=0)VAR.Set_zoffset ( VAR.Get_zoffset() / z_scale_1);


           variance = VAR.norm();
           cout<<"AVG= "; AVG.Aff();
           cout<<"VAR="; VAR.Aff();
           cout<<"Variance length="<<variance<<endl;

           logfile <<  k << " "<<optimalerror <<" "<< avgerr <<" "<<errth<<endl;

           //regenerate population if needed
           if(variance < thresholdvariance)
           {
               cout<<"system locally converged: REGENERATE POPULATION"<<endl;
               for (int i=0; i<pop; i++)
               {
                   pool[i].RandomInit();
                   pool[i].Aff();
                   restart++;
               }
           }
    }

    distpool1.clear();
    distpool2.clear();

    return optimalerror;

}
*/
void RationalSuperShape2D :: Simplify(){
  int r;
  int p = (int)(Get_p());
  int q = (int)(Get_q());
 // if(q>p) {cout <<"erreur param PGCD impossible"<<p<<" "<<q<<endl; system("pause"); exit(0);}

  r=1;//PGCD(p,q);
  p=p/r;
  q=q/r;

  Set_p(p);
  Set_q(q);

}

double RationalSuperShape2D :: CurveLength(int n){

       double gap = 2.*PI/n;
       Vector2d A(Point(0)), B;
       double l(0);
       for(double i=1; i<Get_q() *n; i++)
       {
               B = Point(i*gap);
               l += (B-A).norm();
               A = B;
       }

 return l;
}

Vector2d RationalSuperShape2D :: ClosestPoint( Vector2d P, int lvbrg_itmax){


//P is supposed express in canonical ref

         double alpha, beta;
         double f, fprime, theta, chisquare, r, rprime, delta;
         double C, S, Ctmp, Stmp,x(P[0]), y(P[1]);


         std::vector<Vector2d> Hs;   // storage for closest points on every part
         int nmaxstart = 5;
         for (int partnum=0; partnum < 1/*Get_q()*/; partnum++) // search for each possible q arcs
           {

           //locate the closest point on one angular segment

           theta = atan2(P[1],P[0]); if (theta<0) theta += 2.*PI;
           theta += partnum * 2. * PI;

           double initialtheta = theta  ;
           double lambda = 1e6;
           double oldtheta, angle;

           for (int i=0; i<lvbrg_itmax  /*&& lambda < 1e7*/; i++) // for various random init around the radial point...
             {
             //notation simplification + computation reduction
             r = radius(theta);
             angle = Get_p() * theta *0.25 / Get_q();
             C =  cos (angle);
             S =  sin (angle);
             rprime = 0;      // by default

             if(S!=0 && C!=0)//differentiable function
             {
             rprime = -pow(
                           pow(fabs(C) , Get_n2())/Get_a()
                           +
                           pow(fabs(S) , Get_n3())/Get_b(),
                           -1./Get_n1()-1.0)

                           *

                           (
                           ((Get_n3()*Get_p()*C*pow(fabs(S), Get_n3()))/(4*Get_b()*Get_q()*S))
                           -
                           ((Get_n2()*Get_p()*S*pow(fabs(C), Get_n2()))/(4*Get_a()*Get_q()*C))
                           )
                           /
                           Get_n1();

             //compute f
             //f= (Point(theta) - P ).norm();
             f = sqrt ( (r*C - x)*(r*C - x) + (r*S - y)*(r*S - y));

             //compute cost function
             chisquare = f*f;

             //compute gradient
             //fprime = (r*rprime - rprime*(x*cos(theta) + y*sin(theta)) - r*(-x*sin(theta) + y*cos(theta)) )/f;
             fprime = (r*rprime - rprime*(x*C + y*S) - r*(-x*S + y*C) )/f;

             //compute next parameter displacement
             delta = -f / ( (1.0+lambda)*fprime );
             //store previous value
             oldtheta = theta;
             //compute next value
             theta += delta;
             //evaluate result
             Vector2d PP(Point(theta));

             if ((PP - P).squaredNorm() < chisquare) // success
               {
                 lambda /=100;
               }
               else              //failure
               {
                 theta = oldtheta;
                 lambda *=100;
               }
             }
             else {        // degenerate case, store inter point by default
                  //Hs.push_back(Point(theta));
                  }


         }//end itnum

         Hs.push_back(Point(theta));
         }//end each part


// find min value amnong all sectors
int imin=0;
/*
         std::vector<double> D;
         for (int i=0; i< Get_p(); i++)
         {
             D.push_back((Hs[i]-P).norm());
         }



         double dmin = D[imin];
         for (int i=1; i< Get_p(); i++)
         {
             if (D[i]<dmin) {dmin=D[i];imin=i;}
         }

*/
          double dmax ( (Hs[0]-P).norm() ), dum;
         for (int i=1; i< Hs.size(); i++)
         {
             dum = (Hs[i]-P).norm();
             if( dum < dmax ) { dmax = dum; imin = i; }
         }


         Vector2d H(Hs[imin]);
         Hs.clear();
//         D.clear();

         return H;
}

//Potential fields

/*double RationalSuperShape2D :: ImplicitFunction0( const Vector2d P, bool op, int m) {

       return sqrt((P - ClosestPoint(P)).norm());
}*/

double RationalSuperShape2D :: ImplicitFunction1( const Vector2d P, vector<double> &Dffinal) {

    Dffinal.clear();

    // nothing computable, return zero values, zero partial derivatives
    // the point will have no effect on the ongoing computations

    if ( P[0] == 0 && P[1] == 0)
    {
        // Df/Dx, Df/Dy, Df/Dr set to zero...
        for (int i=0; i<3; i++) Dffinal.push_back(0);
        return 0;
    }

    vector <double> f, Ddum;
    double x(P[0]), y(P[1]), PSL(P.squaredNorm()), PL(sqrt(PSL)), dthtdx (-y/PSL), dthtdy (x/PSL), R,drdth;

    vector< vector<double> > Df;

    //assert angular values between [0, 2q*Pi]

    double tht (atan2(y,x)), thtbase(tht);
    if (tht<0) thtbase += 2.*PI;

    //compute all intersections and associated partial derivatives

    for (int i=0; i<Get_q(); i++)
    {
        tht = thtbase + i*2.*PI;

        R = radius(tht);
        f.push_back(R - PL); //store function

        // store partial derivatives

        vector <double> rowi;
        drdth = DrDtheta(tht);
        rowi.push_back(  drdth*dthtdx -  cos(tht)); //df/dx
        rowi.push_back(  drdth*dthtdy -  sin(tht)); //df/dy
        rowi.push_back( 1. );  //df/dr
        Df.push_back(rowi);
    }

    //bubble sort, not really efficient but acceptable for such small arrays

    for(int i=0; i<Get_q()-1; i++)
        for (int j=i+1; j<Get_q(); j++)
            if (f[i]<f[j])
            {
            //swap values of f[i] and f[j]
            swap(f[i],f[j]);
            //swap rows Df[i] and Df[j]
            Df[i].swap(Df[j]);
            }

    //Compute resulting Rfunction

    vector<double> Df1; //vector for df/dxi

    //iterative evaluation of:
    //      -the resulting R-functions
    //      -the associated partial derivatives

    double f1,fdum;

    f1 = f[0];      // first value of f
    Df1 = Df[0];    // first associated row with partial derivatives

    //combine functions as (...((F1 v F2) v F3 ) v F4) v ...)

    for(int i=1; i<Get_q(); i++) // for all intersections
        {
        //compute R-function, sets all partial derivatives
        //fdum and Ddum temporary results of the union from F1 to Fi

        RpUnion(f1, f[i], Df1, Df[i], fdum, Ddum);

        //update results in f1 and Df1, and iterate
        f1 = fdum;
        Df1 = Ddum;
        }

    //final partial derivatives df/dxi after R-functions
    Dffinal = Df1;

    //clear arrays

    f.clear();    Df.clear();    Ddum.clear();    Df1.clear();

    //return results

    return f1;
}

double RationalSuperShape2D :: ImplicitFunction2( const Vector2d P, vector <double> &Dffinal){

    Dffinal.clear();

    // nothing computable, return zero values, zero partial derivatives
    // the point will have no effect on the ongoing computations

    if ( P[0] == 0 && P[1] == 0)
    {
        // Df/Dx, Df/Dy, Df/Dr set to zero...
        for (int i=0; i<3; i++) Dffinal.push_back(0);
        return 0;
    }

    vector <double> f, Ddum;
    double x(P[0]), y(P[1]), PSL(P.squaredNorm()), PL(sqrt(PSL)), dthtdx (-y/PSL), dthtdy (x/PSL), R,drdth;

    vector<vector<double> > Df;

    //assert angular values between [0, 2q*Pi]

    double tht (atan2(y,x)), thtbase(tht);
    if (tht<0) thtbase += 2.*PI;

    //compute all intersections and associated gradient values

    for (int i=0; i<Get_q(); i++)
    {
        tht = thtbase + i*2.*PI;

        R = radius(tht);
        drdth = DrDtheta(tht);

        f.push_back(1. - PL/R); //store function

        // store partial derivatives

        vector <double> rowi;
        drdth = DrDtheta(tht);
        rowi.push_back( - ( x*R/PL - drdth*dthtdx*PL )/(R*R) ); //df/dx
        rowi.push_back( - ( y*R/PL - drdth*dthtdy*PL )/(R*R) ); //df/dy
        rowi.push_back( PL/(R*R) );  //df/dr
        Df.push_back(rowi);
    }

     //bubble sort, not really efficient but acceptable for such small arrays

    for(int i=0; i<Get_q()-1; i++)
        for (int j=i+1; j<Get_q(); j++)
            if (f[i]<f[j])
            {
            //swap values of f[i] and f[j]
            swap(f[i],f[j]);
            //swap rows Df[i] and Df[j]
            Df[i].swap(Df[j]);
            }

    //Compute resulting Rfunction

    vector<double> Df1; //vector for df/dxi

    //iterative evaluation of:
    //      -the resulting R-functions
    //      -the associated partial derivatives

    double f1,fdum;

    f1 = f[0];      // first value of f
    Df1 = Df[0];    // first associated row with partial derivatives

    //combine functions as (...((F1 v F2) v F3 ) v F4) v ...)

    for(int i=1; i<Get_q(); i++) // for all intersections
        {
        //compute R-function, sets all partial derivatives
        //fdum and Ddum temporary results of the union from F1 to Fi

        RpUnion(f1, f[i], Df1, Df[i], fdum, Ddum);

        //update results in f1 and Df1, and iterate
        f1 = fdum;
        Df1 = Ddum;
        }

    //final partial derivatives df/dxi after R-functions
    Dffinal = Df1;

    //clear arrays

    f.clear();    Df.clear();    Ddum.clear();    Df1.clear();

    //return results

    return f1;
}


double RationalSuperShape2D :: ImplicitFunction3( const Vector2d P, vector <double> &Dffinal){

    Dffinal.clear();

    // nothing computable, return zero values, zero partial derivatives
    // the point will have no effect on the ongoing computations

    if ( P[0] == 0 && P[1] == 0)
    {
        // Df/Dx, Df/Dy, Df/Dr set to zero...
        for (int i=0; i<3; i++) Dffinal.push_back(0);
        return 0;
    }

    vector <double> f, Ddum;
    double x(P[0]), y(P[1]), PSL(P.squaredNorm()), PL(sqrt(PSL)), dthtdx (-y/PSL), dthtdy (x/PSL), R,drdth;

    vector< vector<double> > Df;

    //assert angular values between [0, 2q*Pi]

    double tht (atan2(y,x)), thtbase(tht);
    if (tht<0) thtbase += 2.*PI;

    //compute all intersections and associated gradient values

    for (int i=0; i<Get_q(); i++)
    {
        tht = thtbase + i*2.*PI;

        R = radius(tht);
        drdth = DrDtheta(tht);

        f.push_back( log( R*R / PSL)); //store function

        // store partial derivatives

        drdth = DrDtheta(tht);
        vector <double> rowi;
        rowi.push_back( -2.*(x*R - PSL * drdth*dthtdx)/(R*PSL)  ); //df/dx
        rowi.push_back( -2.*(y*R - PSL * drdth*dthtdy)/(R*PSL) ); //df/dy
        rowi.push_back( 2./R );  //df/dr

        Df.push_back(rowi);
    }

     //bubble sort, not really efficient but acceptable for such small arrays

    for(int i=0; i<Get_q()-1; i++)
        for (int j=i+1; j<Get_q(); j++)
            if (f[i]<f[j])
            {
            //swap values of f[i] and f[j]
            swap(f[i],f[j]);
            //swap rows Df[i] and Df[j]
            Df[i].swap(Df[j]);
            }

    //Compute resulting Rfunction

    vector<double> Df1; //vector for df/dxi

    //iterative evaluation of:
    //      -the resulting R-functions
    //      -the associated partial derivatives

    double f1,fdum;

    f1 = f[0];      // first value of f
    Df1 = Df[0];    // first associated row with partial derivatives

    //combine functions as (...((F1 v F2) v F3 ) v F4) v ...)

    for(int i=1; i<Get_q(); i++) // for all intersections
        {
        //compute R-function, sets all partial derivatives
        //fdum and Ddum temporary results of the union from F1 to Fi

        RpUnion(f1, f[i], Df1, Df[i], fdum, Ddum);

        //update results in f1 and Df1, and iterate
        f1 = fdum;
        Df1 = Ddum;
        }

    //final partial derivatives df/dxi after R-functions
    Dffinal = Df1;

    //clear arrays

    f.clear();    Df.clear();    Ddum.clear();    Df1.clear();

    //return results

    return f1;
}


double RationalSuperShape2D :: DrDtheta(double tht)
{
    double n1(Get_n1()), n2(Get_n2()), n3(Get_n3()), m(Get_p()),q(Get_q()),a(Get_a()),b(Get_b());

    double c ( cos( m*0.25*tht / q)), C( fabs(c)), s( sin( m*0.25*tht / q)), S( fabs(s));

    if(C<EPSILON || S<EPSILON) return 0;

    double DC (-0.25*m*s/q), DS (0.25*m*c/q);

    return
                        (-1./n1) *
                        pow(  pow(C,n2) / a + pow(S,n3)/b , -1./n1 - 1.)*
                        (
								-0.25*m*pow(C,n2)*(n2*tan(m*tht*0.25/q)) / (a*q)
								+
								0.25*m*pow(S,n3)*(n3/tan(m*tht*0.25/q)) / (b*q)
                        );

}






//-----------------------------------------------------------------------------
//
//          Rational Supershapes 3D
//
//-----------------------------------------------------------------------------

void RationalSuperShape3D :: Init(

                double a1, double b1, double n11, double n12, double n13, double p1, double q1, double thtoffset1, double phioffset1, double xoffset1, double yoffset1, double zoffset1,
                double a2, double b2, double n21, double n22, double n23, double p2, double q2, double thtoffset2, double phioffset2, double xoffset2, double yoffset2, double zoffset2)
                {


     Parameters1.clear();
     Parameters2.clear();

     Parameters1.push_back(a1);
     Parameters1.push_back(b1);

     Parameters1.push_back(n11);
     Parameters1.push_back(n12);
     Parameters1.push_back(n13);

     Parameters1.push_back(p1);
     Parameters1.push_back(q1);

     Parameters1.push_back(thtoffset1);
     Parameters1.push_back(phioffset1);

     Parameters1.push_back(xoffset1);
     Parameters1.push_back(yoffset1);
     Parameters1.push_back(zoffset1);

     Parameters2.push_back(a2);
     Parameters2.push_back(b2);

     Parameters2.push_back(n21);
     Parameters2.push_back(n22);
     Parameters2.push_back(n23);

     Parameters2.push_back(p2);
     Parameters2.push_back(q2);

     Parameters2.push_back(thtoffset2);
     Parameters2.push_back(phioffset2);

     Parameters2.push_back(xoffset2);
     Parameters2.push_back(yoffset2);
     Parameters2.push_back(zoffset2);
}

RationalSuperShape3D ::	RationalSuperShape3D()
{
        Init(
        1,1, 200,200,200, 4,1,  0,0,   0,0,0,
        1,1, 20,20,20, 4,1,  0,0,   0,0,0);

        for (int i=0; i<6; i++) Transformations.push_back( 0.0);
        for (int i=0; i<3; i++) Transformations.push_back( 1.0);
}

RationalSuperShape3D :: ~RationalSuperShape3D() {

     Parameters1.clear();
     Parameters2.clear();
     Transformations.clear();

     PointList.clear();
     FaceList.clear();
     NormalList.clear();
}


void RationalSuperShape3D :: RandomInit(){

  //first supershapes

  Set_a1  ( R1.uniform() * a_scale_1 +  a_offset_1);
  Set_b1  ( R1.uniform() * b_scale_1 + b_offset_1);
  Set_n11 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_n12 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_n13 ( R1.uniform() * n_scale_1 + n_offset_1);
  Set_p1  ( (int) (R1.uniform() * p_scale_1 + p_offset_1 ));
  Set_q1  ( (int) (R1.uniform() * q_scale_1 + q_offset_1 ));

  if( Get_q1() > Get_p1() ) //permute
  {
      int a = int (Get_q1());
      int b = int (Get_p1());
      Set_p1(a);
      Set_q1(b);
  }

  Set_thtoffset1( R1.uniform() *  Get_q1() * tht_scale_1 / Get_p1() + tht_offset_1);
  Set_phioffset1( 0 );

  Set_xoffset1 (R1.uniform() * x_scale_1 + x_offset_1);
  Set_yoffset1 (R1.uniform() * y_scale_1 + y_offset_1);
  Set_zoffset1 ( 0 );

  //second supershape

  Set_a2  ( R1.uniform() * a_scale_2 + a_offset_2);
  Set_b2  ( R1.uniform() * b_scale_2 + b_offset_2);
  Set_n21 ( R1.uniform() * n_scale_2 + n_offset_2);
  Set_n22 ( R1.uniform() * n_scale_2 + n_offset_2);
  Set_n23 ( R1.uniform() * n_scale_2 + n_offset_2);
  Set_p2  ( (int) (R1.uniform() * p_scale_2 + p_offset_2 ));
  Set_q2  ( (int) (R1.uniform() * q_scale_2 + q_offset_2 ));

  if( Get_q2() > Get_p2() ) //permute
  {
      int a = int (Get_q2());
      int b = int (Get_p2());
      Set_p2(a);
      Set_q2(b);
  }



  Set_thtoffset2( R1.uniform() *  Get_q1() * tht_scale_2 / Get_p2() + tht_offset_2);
  Set_phioffset2( 0 );

  Set_xoffset2 (R1.uniform() * x_scale_2 + x_offset_2);
  Set_yoffset2 (R1.uniform() * y_scale_2 + y_offset_2);
  Set_zoffset2 ( 0 );


  Simplify();

}


void RationalSuperShape3D :: Simplify(){
  int p,q,r;

  p = (int)(Get_p1());
  q = (int)(Get_q1());

  r=1;//PGCD(p,q);
  p=p/r;
  q=q/r;

  Set_p1(p);
  Set_q1(q);

  p = (int)(Get_p2());
  q = (int)(Get_q2());

  r=1;//PGCD(p,q);
  p=p/r;
  q=q/r;

  Set_p2(p);
  Set_q2(q);

}

Vector2d RationalSuperShape3D :: radius ( const double theta, const double phi ){

   			//radius r1

   			Vector2d R(0,0);
   			double tmp_angle = Get_p1() * theta * 0.25 / Get_q1() ;

			double tmp1( cos(tmp_angle) );
			double tmp2( sin(tmp_angle) );

			if( tmp1 != 0)	tmp1 = pow(fabs(tmp1),Get_n12()) / Get_a1();
			if( tmp2 != 0)	tmp2 = pow(fabs(tmp2),Get_n13()) / Get_b1();

			if( tmp1 + tmp2 !=0 )	R[0] =  pow( (tmp1 + tmp2), -1.0/Get_n11() ) ;
			else				   {cout<<"ERROR RADIUS R1 NULL"<<endl;return R;}


			//radius r2

			tmp_angle = Get_p2() * phi * 0.25 / Get_q2() ;

			tmp1 = ( cos(tmp_angle) );
			tmp2 = ( sin(tmp_angle) );

			if( tmp1 != 0)	tmp1 = pow(fabs(tmp1),Get_n22()) / Get_a2();
			if( tmp2 != 0)	tmp2 = pow(fabs(tmp2),Get_n23()) / Get_b2();

			if( tmp1 + tmp2 !=0 )	R[1] =  pow( (tmp1 + tmp2), -1.0/Get_n21() ) ;
			else				   {cout<<"ERROR RADIUS R2 NULL"<<endl;return R;}

			return R;


}


void RationalSuperShape3D::DrawSurface(bool buildsurface, bool drawpoints, bool drawfullfaces, bool drawwirefaces,bool drawnormals, double R , double G , double B , const int width , const int sampling)
{
    if(buildsurface) BuildSurface(sampling);

    //Draw points if needed
    if(drawpoints){
        //glDisable(GL_LIGHTING);
        glPointSize(5);
        glColor3f(1, 0, 0);
        glBegin(GL_POINTS);
        for(int i=0; i<PointList.size();i++)        {

            Vector3d P(PointList[i]);
            glVertex3f(P[0],P[1],P[2]);
        }
        glEnd();
    }

    //draw faces in full mode if needed

    if(drawfullfaces)
    {
//        glEnable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_FILL);

        glColor3f(R,G,B);
        glLineWidth(width);

        glBegin(GL_TRIANGLES);
        for(int i=0; i<FaceList.size(); i++)    {

            Vector3i F(FaceList[i]);
            for(int k=0; k<3; k++) {

                Vector3d P(PointList[F[k]]);
                Vector3d N(NormalList[F[k]]);
                glNormal3f(N[0], N[1], N[2]);
                glVertex3f(P[0], P[1], P[2]);
            }
        }
    glEnd();
    }

    if(drawwirefaces)
    {
        glDisable(GL_LIGHTING);
        glPolygonMode(GL_FRONT, GL_LINE);

        glColor3f(0,0,1);
        glLineWidth(width);


        for(int i=0; i<FaceList.size(); i++)    {

            Vector3i F(FaceList[i]);
            glBegin(GL_TRIANGLES);
            for(int k=0; k<3; k++) {

                Vector3d P(PointList[F[k]]);
                Vector3d N(NormalList[F[k]]);
                glNormal3f(N[0], N[1], N[2]);
                glVertex3f(P[0], P[1], P[2]);

            }
            glEnd();
        }

    }

    //rendering Normals
    if(drawnormals)    {

        //glDisable(GL_LIGHTING);
        glColor3f(1,1,0);
        glLineWidth(1);
        glBegin(GL_LINES);
        for(int i=0; i<PointList.size(); i++)    {

            Vector3d P(PointList[i]);
            Vector3d N(NormalList[i]);
            N=P+N;
            glVertex3f(P[0], P[1], P[2]);
            glVertex3f(N[0], N[1], N[2]);
        }
        glEnd();
    }

    //glEnable(GL_LIGHTING);



}


void RationalSuperShape3D::BuildSurface(const int theoreticalsampling)
{

    //create points, faces, and normals for a unit supershape NOT TRANSFORMED

    //symmetries are considered as INTEGERS

    //adjust sampling to pass by extrema of the radius for theta

    int sampling = theoreticalsampling - theoreticalsampling%(int)Get_p1();

    //supershape radius is 1 for poles
    Vector3d NP(0,0, 1), SP(0,0,-1);

    // angle for the "horizontal" generating supershape
    double thetaorigin = -PI;
    double thetaend = PI;

    //angle for the "vertical" half supershape
    double phiorigin = -PI*0.5;
    double phiend = PI*0.5;


    double thetagap = (thetaend-thetaorigin) / (sampling );
    double phigap = (phiend-phiorigin) / (sampling * 0.5);

    int sampletheta = (int) (sampling) ; cout <<"sampling="<<sampletheta<<endl;
    int samplephi = (int) (sampling*0.5);

    double theta, phi;

    //clear all data
    PointList.clear();
    FaceList.clear();
    NormalList.clear();

    //
    // Points construction to render a UNIT supershape without transforms
    // using the spherical product
    //

    for (int i=0; i<sampletheta; i++) //n=sampleteta points per slice
    {
        theta = thetaorigin + i*thetagap;
        for( int j=1; j<samplephi ; j++) //no value computed for poles
        {
        phi = phiorigin + j*phigap;

        Vector2d R = radius( theta, phi);
        PointList.push_back( Vector3d( R[0]*R[1]*cos(phi)*cos(theta), R[0]*R[1]*cos(phi)*sin(theta), R[1]*sin(phi) ));
        }
    }

    //both poles are the last points in the point array, radius is 1 for those points

    PointList.push_back(NP);
    PointList.push_back(SP);

    // build faces: general algorithm

    for (int i=0; i<sampletheta; i++)
        for( int j=0; j<samplephi - 2; j++) //no value computed for poles
        {
        Vector3i F1(
            i*(samplephi - 1) + j + 1,
            i*(samplephi - 1) + j,
            ((i+1)%sampletheta) *(samplephi - 1) + j
            );

        FaceList.push_back(F1);

        Vector3i F2(
           ((i+1)%sampletheta) *(samplephi - 1) + j,
           ((i+1)%sampletheta) *(samplephi - 1) + j + 1,
            (i  ) *(samplephi - 1) + j + 1
            );
        FaceList.push_back(F2);
        }

    // build faces surrounding the south pole

    for (int k=0; k<sampletheta; k++)
    {
    Vector3i FSP(
        (samplephi-1)*k,
        PointList.size()-1,
        (samplephi-1)*((k+1)%sampletheta));

    FaceList.push_back(FSP);

    Vector3i FNP(
        (samplephi-1)*((k+1)%sampletheta)+ samplephi-2,
        PointList.size()-2,
        (samplephi-1)*k + samplephi-2);
    FaceList.push_back(FNP);
    }

    //
    // Compute normals
    //

    for(int i=0; i<PointList.size(); i++) NormalList.push_back(Vector3d(0,0,0));

    for( int i=0; i<FaceList.size(); i++)
    {
    Vector3i F(FaceList[i]);

    Vector3d P0(PointList[F[0]]);
    Vector3d P1(PointList[F[1]]);
    Vector3d P2(PointList[F[2]]);

    Vector3d N( (P1-P0).cross(P2-P0));

    NormalList[F[0]] += N;
    NormalList[F[1]] += N;
    NormalList[F[2]] += N;

    }

    for(int i=0; i<NormalList.size(); i++) NormalList[i].normalize();
}




// GA For Rational supershapes 3D

double GA(std::vector <Vector3d > Data, int pop, std::vector<RationalSuperShape3D> &pool, RationalSuperShape3D  &OptimalSolution, int functionused){
    int ngenmax(10000);
    double avgerr(1000);
    int niteration=0;
    int avgerror =100000000;
    int nshuffle = pop*5;
    int i1,i2;
    double dum2;
    std::vector<double> distpool1,distpool2;
    int datasize=Data.size();
    RationalSuperShape3D AVG, VAR;
    double variance;
    extern double thresholdvariance;
    double err;
    int restart = 0;
    extern double      errth;

    //storage best solution

    double optimalerror(1e15);

    double relative_improvement = 1;

    //generate random population
    for (int i=0; i<pop; i++)
    {
        RationalSuperShape3D dum;
        dum.RandomInit();
        pool.push_back(dum);
    }

    //while( niteration < ngenmax && avgerror > avgerrorstop)
    for (int k=0; k<ITERATIONMAX && optimalerror > OPTIMALERRORSTOP && restart < MAXRESTART; k++)
    {
          //shuffle pool

          for (int i=0; i<nshuffle; i++) {
              i1 = (int) ((pop-1)*R1.uniform());
              i2 = (int) ((pop-1)*R1.uniform());
              if (i1 != i2)
              {
                     Permute(pool[i1], pool[i2]);
              }
            }

          //mutation

          for(int i=0; i<pop; i++)
          if(R1.uniform() < PROBA_MUTATE)
          {
             pool[i].Mutate();
          }

           //evaluate pool
           distpool1.clear();
           RationalSuperShape3D tmp;
           for( int i=0; i< pop; i++)
           {
                /*
                Vector2d T(pool[i].Get_xoffset(), pool[i].Get_yoffset());
                double R(pool[i].Get_thtoffset());
                double s = pool[i].Curv.norm();
                */

                tmp = pool[i];

                Vector3d T (tmp.Transformations[0], tmp.Transformations[1], tmp.Transformations[2]);
                Vector3d R (tmp.Transformations[3], tmp.Transformations[4], tmp.Transformations[5]);
                Vector3d S (tmp.Transformations[6], tmp.Transformations[7], tmp.Transformations[8]);

                double s = tmp.GetSurfaceArea();


                err = 0;
                for (int j=0; j<datasize; j++)
                {
                   Vector3d P(Data[j] - T);
                   Vector3d CanonP (P);
                   //Vector2d CanonP(P[0]*cos(R) + P[1]*sin(R), -P[0]*sin(R) + P[1]*cos(R));

                   //if( functionused == 0)
                   err += pow(tmp.ImplicitFunction(CanonP),2);

                   //if( functionused == 1)   err += pow(tmp.ImplicitFunction1(CanonP),2);
                   //if( functionused == 2)   err += pow(tmp.ImplicitFunction2(CanonP),2);
                   //if( functionused == 3)   err += pow(tmp.ImplicitFunction3(CanonP),2);
                   //if( functionused == 4)   err += pow(tmp.ImplicitFunction4(CanonP),2);


                }
                //err =  pow(err,pool[i].Get_q());
                distpool1.push_back(
                                    //err);
                                    err + s + sqrt( err*err + s*s));
                                    //err*err);
           }


           //crossover
           for(int i=0; i<pop; i+=2)
           {
                   RationalSuperShape3D F1,F2;
                   CrossOver(pool[i],pool[i+1],F1,F2);
                   pool.push_back(F1);
                   pool.push_back(F2);
           }

           //evaluate pool2

           for( int i=pop; i< 2*pop; i++)
           {
                err = 0;
                //Vector2d T(pool[i].Get_xoffset(), pool[i].Get_yoffset());
                //double R(pool[i].Get_thtoffset());
                //double s =  pool[i].Curv.norm();

                tmp = pool[i];

                Vector3d T (tmp.Transformations[0], tmp.Transformations[1], tmp.Transformations[2]);
                Vector3d R (tmp.Transformations[3], tmp.Transformations[4], tmp.Transformations[5]);
                Vector3d S (tmp.Transformations[6], tmp.Transformations[7], tmp.Transformations[8]);

                double s = tmp.GetSurfaceArea();


                for (int j=0; j<Data.size(); j++)
                {
                   Vector3d P(Data[j] - T);
                   Vector3d CanonP (P);

                   err += pow(tmp.ImplicitFunction(CanonP),2);


                }
                distpool1.push_back(
                                    //err*s);
                                    err + s + sqrt(err*err + s*s));
                                    //err);
           }


           // sorting arrays

           for( int i=0; i<pool.size(); i++)
           {
                for ( int j=0; j<pool.size(); j++)
                {
                    if(distpool1[i] < distpool1[j])
                    {
                        double tmp = distpool1[i];
                        distpool1[i] = distpool1[j];
                        distpool1[j] = tmp;

                        Permute(pool[i], pool[j]);

                    }
                }
           }


           //evacuate non adapted individuals

           pool.resize((int) (pop) );

           // store best element if needed
           if (distpool1[0] < optimalerror)
           {
              optimalerror = distpool1[0];
              OptimalSolution = pool[0];
           }

           //STATS

           avgerr = 0;
           for(int i=0; i<pop; i++)
           {
                   avgerr += distpool1[i];
           }

           avgerr /= pop;
           cout <<"AVG ERROR = "<<avgerr<<endl;
           cout<<"BEST ERROR FOUND="<<optimalerror<<endl;
           //OptimalSolution.Aff();



           //compute AVG
           AVG = ComputeAverage(pool);
           //Compute Variance
           VAR = ComputeVariance(pool, AVG);

           variance = 0;//VAR.norm();
           cout<<"Variance length="<<variance<<endl;

           logfile <<  k << " "<<optimalerror <<" "<< avgerr <<" "<<errth<<endl;

           //regenerate population if needed
           if(variance < thresholdvariance)
           {
               cout<<"system locally converged: REGENERATE POPULATION"<<endl;
               for (int i=0; i<pop; i++)
               {
                   pool[i].RandomInit();
                   //pool[i].Aff();
                   restart++;
               }
           }
    }

    distpool1.clear();
    distpool2.clear();

    return optimalerror;

}

double RationalSuperShape3D :: ImplicitFunction( Vector3d)
{
    return 0;
}


RationalSuperShape3D ComputeAverage(vector<RationalSuperShape3D> pop)
{
    RationalSuperShape3D AVG;

    AVG.Parameters1.clear();
    AVG.Parameters2.clear();
    AVG.Transformations.clear();

    for(int i=0; i<12; i++) {AVG.Parameters1.push_back(0); AVG.Parameters2.push_back(0);}
    for(int i=0; i<9; i++) AVG.Transformations.push_back(0);

    int n=pop.size();

    for(int j=0; j<n; j++)
    {
        RationalSuperShape3D tmp(pop[j]);

        for(int i=0; i<12; i++)
        {
            AVG.Parameters1[i] += tmp.Parameters1[i];
            AVG.Parameters2[i] += tmp.Parameters2[i];
        }

        for(int i=0; i<9; i++)
        {
            AVG.Transformations[i] += tmp.Transformations[i];
        }
    }

    for(int i=0; i<12; i++)
        {
            AVG.Parameters1[i] /= n;
            AVG.Parameters2[i] /= n;
        }

    for(int i=0; i<9; i++)
        {
            AVG.Transformations[i] /= n;
        }

    return AVG;


}


RationalSuperShape3D ComputeVariance(vector<RationalSuperShape3D> pop, RationalSuperShape3D AVG)
{

    RationalSuperShape3D VAR;
    VAR.Parameters1.clear();
    VAR.Parameters2.clear();
    VAR.Transformations.clear();

    for(int i=0; i<12; i++) {VAR.Parameters1.push_back(0); VAR.Parameters2.push_back(0);}
    for(int i=0; i<9; i++) VAR.Transformations.push_back(0);

    int n=pop.size();

           for(int j=0; j<n; j++)
           {
                RationalSuperShape3D tmp(pop[j]);

                for(int i=0; i<12; i++)
                    {
                    VAR.Parameters1[i] += pow(AVG.Parameters1[i] - tmp.Parameters1[i], 2);
                    VAR.Parameters2[i] += pow(AVG.Parameters2[i] - tmp.Parameters2[i], 2);
                    }

                for(int i=0; i<9; i++)
                    {
                    VAR.Transformations[i] += pow(AVG.Transformations[i] - tmp.Transformations[i], 2);
                    }
            }



            for(int i=0; i<12; i++)
                {
                VAR.Parameters1[i] /= n;
                VAR.Parameters2[i] /= n;
                }

            for(int i=0; i<9; i++)
                {
                VAR.Transformations[i] /= n;
                }
           //normalize Var

}




double RationalSuperShape2D :: DrDa(const double tht)
{
    double C = fabs( cos( Get_p() * tht * 0.25 / Get_q())) ;
    double S = fabs( sin( Get_p() * tht * 0.25 / Get_q())) ;
    double aCn2 = pow(C, Get_n2())/Get_a();
    double bSn3 = pow(S, Get_n3())/Get_b();

    return  aCn2 * pow( aCn2 + bSn3, -1./Get_n1() - 1.) / (Get_n1()*Get_a());

}

double RationalSuperShape2D :: DrDb(const double tht)
{
    double C = fabs( cos( Get_p() * tht * 0.25 / Get_q())) ;
    double S = fabs( sin( Get_p() * tht * 0.25 / Get_q())) ;

    double aCn2 = pow(C, Get_n2())/Get_a();
    double bSn3 = pow(S, Get_n3())/Get_b();

    return  bSn3 * pow( aCn2 + bSn3, -1./Get_n1() - 1.) / (Get_n1()*Get_b());
}

double RationalSuperShape2D :: DrDn1(const double tht)
{
    double R(radius(tht));
    double rn1 = pow(R, -Get_n1());

    if (R>EPSILON){
    //return log ( pow ( R, -1./Get_n1()))*R;
    return log(rn1) * R /(Get_n1()*Get_n1());
    }

    //cout <<"rhaaaa"<<endl;
    return 0;

}

double RationalSuperShape2D :: DrDn2(const double tht)
{
    double C = fabs( cos( Get_p() * tht * 0.25 / Get_q())) ;
    double S = fabs( sin( Get_p() * tht * 0.25 / Get_q())) ;

    double aCn2 = pow(C, Get_n2())/Get_a();
    double bSn3 = pow(S, Get_n3())/Get_b();

    if(C <EPSILON) return 0;
    return
        - pow( aCn2 + bSn3, -1./Get_n1() - 1.)
        *
        log(C)*aCn2 / Get_n1();

}

double RationalSuperShape2D :: DrDn3(const double tht)
{
    double C = fabs( cos( Get_p() * tht * 0.25 / Get_q())) ;
    double S = fabs( sin( Get_p() * tht * 0.25 / Get_q())) ;


    double aCn2 = pow(C, Get_n2())/Get_a();
    double bSn3 = pow(S, Get_n3())/Get_b();

    if(S < EPSILON) return 0;
    return
        - pow( aCn2 + bSn3, -1./Get_n1() - 1.)
        *
        log(S)*bSn3 / Get_n1();
}




vector < Vector2d > RationalSuperShape2D :: GenerateData (
                int n,                  //number of points generated
                double x0 , double y0 , //translation offset
                double tht0 ,           //rotation offset
                bool RandAngularValues ,//random angles or not
                double NoiseRate,       //radial noise percentage
                string filename        //file
                 )
{
    Matrix3d Tr, Rot;

        Tr << 1 , 0 , x0 ,
              0 , 1 , y0 ,
              0 , 0 , 1;

        Rot << cos(tht0) , -sin(tht0) , 0 ,
               sin(tht0) , cos(tht0) , 0 ,
               0 , 0 , 1;


    //
    //generate 2D data and export to data.txt
    //

char s[10];
sprintf(s, "data_%d.txt", ndata);
string rha(s);
ndata++;
    logfile.open(rha.c_str());
                 //filename.c_str());

    vector < Vector2d > FileData;

    double angle;
    for( int i=0; i<n;i++)    {
        if(RandAngularValues) angle = 2*PI*Get_q()*R1.uniform();
        else                  angle = 2*PI*Get_q()*i / double(n);

        double r = radius(angle) * (1. + (R1.uniform()*2. -1.)* NoiseRate);

        //compute point without tranform
        Vector3d P(  cos(angle) * r ,  sin(angle) * r , 1 );

        //apply translation and rotation
        Vector3d P2 = Tr * (Rot * P);
        //store
        Vector2d Ptrans(P2[0],P2[1]);
        FileData.push_back( Ptrans);
    }


    //sort data by ascenting x coordinates;

    Vector2d A, B;
        for(int i = 0; i<n-1; i++)
            for(int j = i+1; j< n; j++) {

                A = FileData[i];
                B = FileData[j];

                if( A[0] > B[0] )
                {
                    swap (FileData[i], FileData[j]);
                }
            }


        for(int i = 0; i<n; i++) {
        Vector2d P(FileData[i]);
        logfile << P[0]<<" "<< P[1] <<endl;
        }

    logfile.close();

    cout <<"Data generated: Point number="<<n<<" file written="<<rha<<endl;

    return FileData;
    }

vector < Vector2d > RationalSuperShape2D :: GenerateUniformData (
                int n,                  //number of points generated
                double x0 , double y0 , //translation offset
                double tht0 ,           //rotation offset
                bool RandAngularValues ,//random angles or not
                double NoiseRate,       //radial noise percentage
                string filename        //file
                 )
{
    Matrix3d Tr, Rot;

        Tr << 1 , 0 , x0 ,
              0 , 1 , y0 ,
              0 , 0 , 1;

        Rot << cos(tht0) , -sin(tht0) , 0 ,
               sin(tht0) , cos(tht0) , 0 ,
               0 , 0 , 1;


    //
    //generate 2D data and export to data.txt
    //




    ofstream datastream;
    datastream.open(filename.c_str());
                 //filename.c_str());

    vector < Vector2d > FileData;

    double angle;

    for( int i=0; i<n;i++)    {

        angle =  2.*PI*i / n;
        double r = radius(angle);
        //compute point without tranform
        Vector2d P(  cos(angle) * r ,  sin(angle) * r );

        FileData.push_back( P);
    }


    // now compute avg dist between consecutive points



    //respan the point list and add points to uniformize de point distribution


    for(int k=0; k<10; k++)
    {
        double d(0);

        for( int i=0; i<FileData.size();i++)    {
            d += (FileData[i] - FileData[(i+1)%n]).norm();
        }

        d /= FileData.size();
        //cout << "AVG D="<<d <<endl;

         vector< Vector2d > TmpData;
        //cout <<"avant"<<FileData.size()<<endl;

        for(int i=0; i<FileData.size(); i++)
        {
            Vector2d A(FileData[i]);
            Vector2d B(FileData[(i+1)%FileData.size()]);

            double ratio =  (A-B).norm() / d;

            TmpData.push_back(A);
            if (ratio > 1 ) // insert point at mid angle
            {
             //   cout << "ratio="<<ratio<<endl;
             //   system("pause");
             double angleA = atan2(A[1], A[0]);   if(angleA<0) angleA += 2*PI;
             double angleB = atan2(B[1], B[0]);   if(angleB<0) angleB += 2*PI;

             double angleC = (angleA+angleB)*0.5;
             double r = radius(angleC);

             Vector2d P(  cos(angleC) * r ,  sin(angleC) * r );

             TmpData.push_back(P);
             }
        }

        FileData.clear();
        for(int i=0; i< TmpData.size(); i++) FileData.push_back(TmpData[i]);

        //cout <<"apres"<<FileData.size()<<endl;

    }



    //add noise

    for(int i=0; i<FileData.size(); i++)
    {
        Vector2d P(FileData[i]);

//        cout << "Avant "<<endl<< P <<endl;


        Vector2d D(R1.uniform()* NoiseRate, R1.uniform()* NoiseRate);

        FileData[i]= P+D;

        //cout << "Apres "<<endl<< P <<endl;

        //system("pause");


    }



    //sort data by ascenting x coordinates;

    Vector2d A, B;
        for(int i = 0; i<FileData.size()-1; i++)
            for(int j = i+1; j< FileData.size(); j++) {

                A = FileData[i];
                B = FileData[j];

                if( A[0] > B[0] )
                {
                    swap (FileData[i], FileData[j]);
                }
            }


        for(int i = 0; i<FileData.size(); i++) {
        Vector2d P(FileData[i]);
        datastream << P[0]<<" "<< P[1] <<endl;
        }

    datastream.close();

    cout <<"Data generated: Point number="<<FileData.size()<<" file written="<<filename<<endl;

    return FileData;
    }



void RationalSuperShape2D :: GetPartialDerivatives(double tht, double &DrDa, double &DrDb, double &DrDn1, double &DrDn2, double &DrDn3)
{
    DrDa = DrDb = DrDn1 = DrDn2 = DrDn3 = 0;

    double C = fabs( cos( Get_p() * tht * 0.25 / Get_q())) ;
    double S = fabs( sin( Get_p() * tht * 0.25 / Get_q())) ;

    DrDa = DrDb = DrDn1 = DrDn2 = DrDn3 = 0;

    if( Get_a() == 0 || Get_b() == 0)
    {
        cerr <<"Problem detected in GetPartialDerivatives: a=0 OR b=0"<<endl;
        system("pause");
        return;
    }

    double aCn2 = pow(C, Get_n2())/Get_a();
    double bSn3 = pow(S, Get_n3())/Get_b();

    double R = 0;

    if (fabs(Get_n1()) < 1e-15 )
    {
        cerr <<"Problem detected in GetPartialDerivatives: n1 = 0"<<endl;
       // system("pause");
        return;
    }

     R = pow( aCn2 + bSn3, -1./Get_n1());

     DrDa = aCn2 * pow( aCn2 + bSn3, -1./Get_n1() - 1.) / (Get_n1()*Get_a());
     DrDb = bSn3 * pow( aCn2 + bSn3, -1./Get_n1() - 1.) / (Get_n1()*Get_b());


    if(C <= 1e-15 )
    {
        cerr <<"Problem detected in GetPartialDerivatives: Cos = 0"<<endl;
        return;
    }
    else
    DrDn2 =  - pow( aCn2 + bSn3, -1./Get_n1() - 1.)  * log(C)*aCn2 / Get_n1();

    if(S <= 1e-15 )
    {
        cerr <<"Problem detected in GetPartialDerivatives: Sin = 0"<<endl;
        return;
    }
    else
    DrDn3 =  - pow( aCn2 + bSn3, -1./Get_n1() - 1.)  * log(S)*bSn3 / Get_n1();

    if (aCn2 + bSn3 <= 1e-15)
    {
        //cerr <<"Problem in GetPartialDerivatives: aCn2 + bSn3 = 0 for n1=" << Get_n1() <<" n2=" << Get_n2() <<" n3="<<Get_n3()<<endl;
        cerr <<"aCn2="<<aCn2<<" bSn3"<<bSn3<<endl;
        return;
    }
    else
    DrDn1 = log ( aCn2 + bSn3 ) * R / (Get_n1()*Get_n1());


}



void RationalSuperShape2D :: RobustInit(vector< Vector2d > Data)
{

    vector <Vector2d> newdata ;
    for(int i=0; i<Data.size();i++) newdata.push_back(Data[i]);

    vector < int > Hist;
 Vector2d T(0,0) ; double r(0);
 bool stop(false);
 double histavg, oldhistavg;
    for(int it=0; it<30 && !stop; it++) // arbitrary number of iterations
    {
        //cout << it<<endl;
        //clean angular histogram
        Hist.clear();
        for(int i=0; i<Get_p(); i++) Hist.push_back(0);

        //init position and avg radius

        T<< 0,0;
        r=0;

        //compute center and fill angular histogram
        for (int i=0; i<newdata.size(); i++)
        {
            Vector2d P(newdata[i]);
            T+=P;

            double angle = atan2(P[1], P[0]); if(angle <0) angle += 2*PI;

            for(int index =0; index <Hist.size(); index++)
                if( index * (2*PI)/Get_p() <= angle && angle < (index+1) * (2*PI)/Get_p() )
                    Hist[index] =  Hist[index]+1;
        }

        //cout << " T="<<T[0]<<" "<<T[1]<<endl;
        T /= newdata.size();



        r=0;
        for (int i=0; i<newdata.size(); i++)    r += (newdata[i] - T).norm() ;
        r /=newdata.size();

        //cout << it<<" T="<<T[0]<<" "<<T[1]<<" R="<<r <<endl;

        //compute AVG value for HIST

        oldhistavg = histavg;
         histavg=0;

        for(int i=0; i < Hist.size(); i++)
        {
            histavg+=Hist[i];
        }
        histavg /= Hist.size();

        if(histavg == oldhistavg) stop = true;

        //cout << "Hist AVG="<<histavg<<endl;

        //now fill data for angular sectors inferior to AVG

        for(int i=0; i < Hist.size(); i++)
        {
            //go through angular sectors

            double amin = i*2*PI/(Get_p());

            for(int j=0; j<histavg-Hist[i]; j++)
            {
                //generate a random angle in [amin, amax];
                double angle = amin +  PI/Get_p();
                //R1.uniform()*2*PI/(n*Get_p());

                Vector2d flood(-T[0]+r*cos(angle), -T[1]+r*sin(angle));
                newdata.push_back(flood);
            }

        }
    }

    Set_xoffset(T[0]);
    Set_yoffset(T[1]);
    Set_a(r);
    Set_b(r);

    cout <<"ROBUST..."<< T[0] << " " << T[1] << "R="<< r <<endl;

//system("pause");
}


double RationalSuperShape2D :: XiSquare(
                                        const vector < Vector2d> Data,
                                        MatrixXd &alpha,
                                        VectorXd &beta,
                                        bool init_on,
                                        bool robust,
                                        int functionused,
                                        bool Normalization,
                                        bool update) {

   /*
    ChiSquare = XiSquare(Data,
                             alpha,
                             beta,
                             itnum == 0,    //init x0, y0, and tht0 or not?
                             true,          //robust or not?
                             functionused,             //implicitf cuntion1
                             Normalization,         //actiate normalization or not
                             true);         //update vectors

   */

   //cout <<"NORM ON IN DEBUT OPT2 :"<<Normalization<<endl;
    VectorXd dj;

    dj = VectorXd::Zero(8);

    Matrix3d Tr,Rot, dTrdx0, dTrdy0, dRotdtht0;

    int i,j,k;

    //derivatives of translation matrix are constant and can be computed once for all
    dTrdx0 << 0 , 0 , -1 , 0 , 0 , 0 ,  0 , 0 , 1;
    dTrdy0 << 0 , 0 , 0 ,  0 , 0 , -1 , 0 , 0 , 1;

    //functions pointer
    double (RationalSuperShape2D ::*pt2ConstMember)(const Vector2d P, vector<double> &Dffinal) = NULL;

    switch (functionused){
    case 1 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction1;      }break;
    case 2 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction2;      }break;
    case 3 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction3;      }break;
    default :  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction1;
    }

    vector<double> Df;
    if(init_on){
        if( robust) RobustInit(Data);
        else
        {
        Vector2d Center(0,0);
        for(i=0; i<Data.size(); i++)    Center+=Data[i];

        Center /= Data.size();

        double amin(1e30);
        for(i=0; i<Data.size(); i++)    amin = min(amin, (Data[i]-Center).norm());

        Set_a(amin);
        Set_b(amin);
        Set_xoffset(Center[0]);  Set_yoffset(Center[1]);
        }

    }

  	double a(Get_a()), b(Get_b()), R, x, y, tht, dthtdx, dthtdy,dthtdx0, dthtdy0, drdth,
        ChiSquare(1e15),
        f(0),h(0),
        df(0),
        n1(Get_n1()),n2(Get_n2()),n3(Get_n3()),
        p(Get_p()), q(Get_q()),
        x0(Get_xoffset()),y0(Get_yoffset()),tht0(Get_thtoffset()),
        dxdx0,dxdy0,dxdtht0, dydx0,dydy0,dydtht0, dthtdtht0;

		 //clean memory
    if(update)
    {
        alpha.setZero();
        beta.setZero();
    }




		//evaluate ChiSquare, components for the beta and matrix alpha

		ChiSquare=0;

		//First define inverse translation T-1
            Tr.setZero();
            Rot.setZero();
            dRotdtht0.setZero();

			Tr << 1 , 0 , -x0 ,
                  0 , 1 , -y0 ,
                  0 , 0 , 1;

            //Get partial derivatives

            //Then define inverse rotation, i.e. transposed rotation

            Rot << cos(tht0) , sin(tht0) , 0 ,
                  -sin(tht0) , cos(tht0) , 0 ,
                  0 , 0 , 1;
/*
                  Rot << 1 , 0 , 0 ,
                         0 , 1 , 0 ,
                         0 , 0 , 1;
*/

            //get partial derivatives
            dRotdtht0 << -sin(tht0) , cos(tht0) , 0 ,
                         -cos(tht0) , -sin(tht0) , 0 ,
                          0 , 0 , 1;
/*
                dRotdtht0 << 0 , 0 , 0 ,
                         0 , 0 , 0 ,
                          0 , 0 , 1;

*/
        for(i=0; i<Data.size(); i++){


				double DfDr;

            //global inverse transform is T * R

            Vector2d dum(Data[i]);
           	Vector3d dum2(dum[0],dum[1],1);

            //apply inverse transform
           	Vector3d dum3 ( Rot * (Tr * dum2));

           	//get partial derivatives for canonical point

           	Vector3d dPdx0 ( Rot * (dTrdx0 * dum2) );
           	Vector3d dPdy0 ( Rot * (dTrdy0 * dum2) );
           	Vector3d dPdtht0 (dRotdtht0 * (Tr *dum2) );

            Vector2d P(dum3[0],dum3[1]);

           	//simplify notations

           	dxdx0 = dPdx0[0];
           	dxdy0 = dPdy0[0];
           	dxdtht0 =  dPdtht0[0];

           	dydx0 = dPdx0[1];
           	dydy0 = dPdy0[1];
           	dydtht0 =  dPdtht0[1];

           	//cout << dxdx0  <<" "<<dxdy0<<" "<<dxdtht0<<" "<<dydx0<<" "<<dydy0<<" "<<dydtht0<<endl;


           	x = P[0]; y = P[1];

            // avoid division by 0 ==> numerical stability

            if (P.norm()<EPSILON) continue; // avoids division by zero

            tht = atan2(y,x); if( tht<0) tht+=2.*PI;

            //if ( fabs(sin (tht*p*0.25/q))< EPSILON || fabs(cos (tht*p*0.25/q))< EPSILON ) continue;

            R = radius(tht);

            //theta = Arctan(Y/X)
            dthtdx = -sin(tht) ;//-y / (x*x+y*y);
            dthtdy =  cos(tht); //x / (x*x+y*y);

            //partial derivatives of theta regarding x offset, y offset, and angular offset

            dthtdx0   = dthtdx * dxdx0  + dthtdy * dydx0;
            dthtdy0   = dthtdx * dxdy0  + dthtdy * dydy0;
            dthtdtht0 = dthtdx * dxdtht0+ dthtdy * dydtht0;

            //avoid non differentiable cases

            drdth = DrDtheta(tht);

			f = (*this.*pt2ConstMember)(P, Df); // call to the implicit function
			//logfile << "point "<<i<< "P="<<P[0]<<" "<<P[1]<<" :F="<<f <<endl;

			//cout <<"function computation and gradient ok"<<endl;

			//
			//compute elements beta[i][0] and alpha[i][j]
			//

			//==> requires partial derivatives!!

			DfDr = Df[2]; //Df/Dr stored at index 2 in array Df during the call to ImplicitFunction1-2-3

			// F1 = R-PL ==> DfDr = 1. ;
			// F2 = 1-PL/R ==> DfDr = PL/R² ;
			// F3 =  log ( R²/PSL) ==> DfDr = 2/R


			//GetPartialDerivatives(tht, DrDa, DrDb, DrDn1, DrDn2, DrDn3);
			//for(int toto=0; toto<8; toto++) dj[toto]=0;

			dj.setZero();

            //df/da = df/dr * dr/da
			dj[0] = DfDr * DrDa(tht) ;

			//df/db = df/dr * dr/db
			dj[1] = DfDr * DrDb(tht) ;

			//df/dn1 = df/dr * dr/dn1
			dj[2] = DfDr * DrDn1(tht);

			//df/dn2 = df/dr * dr/dn2
			dj[3] = DfDr * DrDn2(tht);

			//df/dn3 = df/dr * dr/dn3
			dj[4] = DfDr * DrDn3(tht);

            //df/dx0 = df/dr * dr/dtht *dtht/dx0
			dj[5]= DfDr * drdth*dthtdx0;

			//df/dy0 = df/dr * dr/dtht *dtht/dy0
			dj[6]= DfDr * drdth*dthtdy0;


			//df/dth0 = dfdr * dr/dtht * dtht/dtht0
			dj[7]= DfDr * drdth*dthtdtht0;

			//cout <<"Dj = "<<dj<<endl<<endl;
			//system("pause");

			double nablamagn =  Df[0]*Df[0] //df/dx
                                + Df[1]*Df[1]; //df/dy
                                 //dj.squaredNorm();

			h = f*pow(f*f + nablamagn,-0.5);

             /*+ dj[0]*dj[0] //df/da
             + dj[1]*dj[1] //df/db
             + dj[2]*dj[2] //df/dn1
             + dj[3]*dj[3] //df/dn2
             + dj[4]*dj[4] //df/dn3
             + dj[5]*dj[5] //df/dx0
             + dj[6]*dj[6] //df/dy0
             + dj[7]*dj[7] //df/dtht0
             */


            if(Normalization) ChiSquare += h*h;
            else ChiSquare += f*f;


            //cout <<"NORM ON IN DEBUT OPT2 AVANT UPDATE:"<<Normalization<<endl;
            //scale factor for the gradient if normalizing the function
            if(update){

            if(Normalization) {
                if (fabs(f)>EPSILON || nablamagn > EPSILON )
                    {
                    for (int idum=0; idum<8; idum++)
                    dj[idum] *= h * (1. - h*h) / f;
                    }
                    else
                    dj.setZero();

              //  cout<<" NORM"<<endl;
               // system("pause");

                    }

                //else dj.setZero();


			//summation of values for the gradient

            if(Normalization)
                for(k=0;k<8; k++)
                {
                    if(f != 0)
                    beta[k] -=  h * dj[k];
				}
            else
                for(k=0;k<8; k++)
				{
                    beta[k] -= f*dj[k];
				}
			//compute approximation of Hessian matrix

			for(k=0; k<8; k++)
				for(j=0; j<8; j++)
					alpha(k,j) +=  dj[k]*dj[j];
            }
		}//for all vertices

		return ChiSquare;
}




void RationalSuperShape2D :: Optimize(
    vector< Vector2d > Data,
    double &err ,
    bool Normalization,
    int functionused
    )
{




  	double ChiSquare(1e15), OldChiSquare(1e15),f(0),df(0),n1,n2,n3,p,q,x0,y0,tht0, dxdx0,dxdy0,dxdtht0, dydx0,dydy0,dydtht0, dthtdtht0;
	int itnum(0),i,j,k, small_improvement(0);


    logfile.open("Optimize.txt");


	bool STOP(false);
	double oldparams[15];//, dj[16];
  Vector2d G;
	double LAMBDA_INCR(10);
	double lambda(1000);




MatrixXd alpha;

alpha =  MatrixXd::Zero(8,8);

VectorXd beta, dj, oldbeta,  trial, sigma;

beta = VectorXd::Zero(8);
dj = VectorXd::Zero(8);
oldbeta = VectorXd::Zero(8);
trial = VectorXd::Zero(8);
sigma = VectorXd::Zero(8);

Matrix3d Tr,Rot, dTrdx0, dTrdy0, dRotdtht0;

//derivatives of translation matrix are constant and can be computed once for all
dTrdx0 << 0 , 0 , -1 , 0 , 0 , 0 ,  0 , 0 , 1;
dTrdy0 << 0 , 0 , 0 ,  0 , 0 , -1 , 0 , 0 , 1;






    //functions pointer
    double (RationalSuperShape2D ::*pt2ConstMember)(const Vector2d P, vector<double> &Dffinal) = NULL;

    switch (functionused){
    case 1 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction1;      }break;
    case 2 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction2;      }break;
    case 3 :{  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction3;      }break;
    default :  pt2ConstMember = &RationalSuperShape2D :: ImplicitFunction1;
    }

	cout<<"Optimize"<<endl<<endl;


    logfile<<"1 "<<lambda<<" "<<*this;

	double a, b, R, x, y, tht, dthtdx, dthtdy,dthtdx0, dthtdy0, drdth;
	 double h;

    vector<double> Df;
/*
    Vector2d Center(0,0);
    for(i=0; i<Data.size(); i++)
		{
		    Center+=Data[i];
        }
    Center /= Data.size();


    double amin(1e30);
    for(i=0; i<Data.size(); i++)
		{
		     amin = min(amin, (Data[i]-Center).norm());
        }

    Set_a(amin);
    Set_b(amin);

    Set_xoffset(Center[0]);
    Set_yoffset(Center[1]);

    //Set_xoffset(0);
    //Set_yoffset(0);
*/
    RobustInit(Data);


	for(itnum=0; itnum<1000 && STOP==false; itnum++)	{

		//SimplifyNotation(n1,n2,n3,m,N1,N2,N3,M);

		n1 = Get_n1();
		n2 = Get_n2();
		n3 = Get_n3();
		a = Get_a();
		b = Get_b();
		p = Get_p();
		q = Get_q();
		x0 = Get_xoffset();
		y0 = Get_yoffset();
		tht0 = Get_thtoffset();

		 //clean memory


        alpha.setZero();
        beta.setZero();


		//store oldparams

		for(i=0; i<Parameters.size(); i++) oldparams[i]=Parameters[i];

		//evaluate ChiSquare, components for the beta and matrix alpha

		ChiSquare=0;
		double DfDr;

		for(i=0; i<Data.size(); i++){

			//
			//evaluate ChiSquare
			//

			//First define inverse translation T-1
            Tr.setZero();
            Rot.setZero();
            dRotdtht0.setZero();

			Tr << 1 , 0 , -x0 ,
                  0 , 1 , -y0 ,
                  0 , 0 , 1;

            //Get partial derivatives

            //Then define inverse rotation, i.e. transposed rotation

            Rot << cos(tht0) , sin(tht0) , 0 ,
                  -sin(tht0) , cos(tht0) , 0 ,
                  0 , 0 , 1;

            //get partial derivatives
            dRotdtht0 << -sin(tht0) , cos(tht0) , 0 ,
                         -cos(tht0) , -sin(tht0) , 0 ,
                          0 , 0 , 1;

            //global inverse transform is T * R

            Vector2d dum(Data[i]);
           	Vector3d dum2(dum[0],dum[1],1);

            //apply inverse transform
           	Vector3d dum3 ( Rot * (Tr * dum2));

           	//get partial derivatives for canonical point

           	Vector3d dPdx0 ( Rot * (dTrdx0 * dum2) );
           	Vector3d dPdy0 ( Rot * (dTrdy0 * dum2) );
           	Vector3d dPdtht0 (dRotdtht0 * (Tr *dum2) );

            Vector2d P(dum3[0],dum3[1]);

           	//simplify notations

           	dxdx0 = dPdx0[0];
           	dxdy0 = dPdy0[0];
           	dxdtht0 =  dPdtht0[0];

           	dydx0 = dPdx0[1];
           	dydy0 = dPdy0[1];
           	dydtht0 =  dPdtht0[1];

           	//cout << dxdx0  <<" "<<dxdy0<<" "<<dxdtht0<<" "<<dydx0<<" "<<dydy0<<" "<<dydtht0<<endl;


           	x = P[0]; y = P[1];

            // avoid division by 0 ==> numerical stability

            if (P.norm()<EPSILON) continue; // avoids division by zero

            tht = atan2(y,x); if( tht<0) tht+=2.*PI;

            if ( fabs(sin (tht*p*0.25/q)) < EPSILON || fabs(cos (tht*p*0.25/q) < EPSILON) ) continue;

            R = radius(tht);

            //theta = Arctan(Y/X)
            dthtdx = -sin( tht) ;//y / (x*x+y*y);
            dthtdy =  cos(tht); //x / (x*x+y*y);

            //partial derivatives of theta regarding x offset, y offset, and angular offset

            dthtdx0   = dthtdx * dxdx0  + dthtdy * dydx0;
            dthtdy0   = dthtdx * dxdy0  + dthtdy * dydy0;
            dthtdtht0 = dthtdx * dxdtht0+ dthtdy * dydtht0;

            //avoid non differentiable cases



            drdth = DrDtheta(tht);

			f = (*this.*pt2ConstMember)(P, Df); // call to the implicit function
			h = f*pow(f*f + Df[0]*Df[0] + Df[1]*Df[1] ,-0.5);

            if(Normalization) ChiSquare += h*h;
            else ChiSquare += f*f;

			//cout <<"function computation and gradient ok"<<endl;

			//
			//compute elements beta[i][0] and alpha[i][j]
			//

			//==> requires partial derivatives!!

			DfDr = Df[2]; //Df/Dr stored at index 2 in array Df during the call to ImplicitFunction1-2-3

			// F1 = R-PL ==> DfDr = 1. ;
			// F2 = 1-PL/R ==> DfDr = PL/R² ;
			// F3 =  log ( R²/PSL) ==> DfDr = 2/R

			//df/da = df/dr * dr/da
			//GetPartialDerivatives(tht, DrDa, DrDb, DrDn1, DrDn2, DrDn3);
			dj.setZero();


			dj[0] = DfDr * DrDa(tht) ;

			//df/db = df/dr * dr/db
			dj[1] = DfDr * DrDb(tht) ;

			//df/dn1 = df/dr * dr/dn1
			dj[2] = DfDr * DrDn1(tht);

			//df/dn2 = df/dr * dr/dn2
			dj[3] = DfDr * DrDn2(tht);

			//df/dn3 = df/dr * dr/dn3
			dj[4] = DfDr * DrDn3(tht);



            //df/dx0 = df/dr * dr/dtht *dtht/dx0
			dj[5]= DfDr * drdth*dthtdx0;

			//df/dy0 = df/dr * dr/dtht *dtht/dy0
			dj[6]= DfDr * drdth*dthtdy0;


			//df/dth0 = dfdr * dr/dtht * dtht/dtht0
			dj[7]= DfDr * drdth*dthtdtht0;




            //scale factor for the gradient if normalizing the function
            if(Normalization) {
                if (f != 0.0)
                for (int idum=0; idum<8; idum++)
                    dj[idum] *= h * (1. - h*h) / f;
            }

			//summation of values for the gradient

            if(Normalization)
                for(k=0;k<8; k++)
                {
                    beta[k] -=  h*dj[k];
				}
            else
                for(k=0;k<8; k++)
				{
                    beta[k] -= f*dj[k];
				}
			//compute approximation of Hessian matrix

			for(k=0; k<8; k++)
				for(j=0; j<8; j++)
					alpha(k,j) +=  dj[k]*dj[j];
		}//for all vertices

        //cout <<"Hessian and Beta ok"<<endl;

		//
		// add Lambda to diagonla elements and solve the matrix
		//

        //Linearization of Hessian, cf Numerical Recepies
		for(k=0; k<8; k++)
		{
		    alpha(k,k) *=   1. + lambda;  //multiplicative factor to make diagonal dominant
		    alpha(k,k) +=   lambda;       //additive factor to avoid rank deficient matrix
		}

        //solve system
        alpha.ldlt().solveInPlace(beta);

        //cout << alpha <<endl;
        //cout << beta <<endl;
        //system("pause");


        //add displacement beta to current parameters
/*
        //keep a and b > 0
        //if(Parameters[0] +  beta[0] > 0 && Parameters[0] +  beta[0] < 100)
        Parameters[0] +=  beta[0];
        //if(Parameters[1] +  beta[1] > 0 && Parameters[1] +  beta[1] < 100)
        Parameters[1] +=  beta[1];


        // setting |n1|<0.005. leads to strong numerical instabilities (QNAN and others)
        // Setting n1, n2 or n3 < -5 also leads to QNAN error

        //if(fabs(Parameters[2] +  beta[2])>0.005 && Parameters[2] +  beta[2] > 0)
        Parameters[2] +=  beta[2];

        //if(Parameters[3] +  beta[3] > 0)
        Parameters[3] +=  beta[3];
		//if(Parameters[4] +  beta[4] > 0)
		Parameters[4] +=  beta[4];
*/

        //see what is going on with new parameters

		//coefficients a and b in [0.01, 100]

		Parameters[0] = max ( 0.001, min(100., Parameters[0] + beta[0]));
		Parameters[1] = max ( 0.001, min(100., Parameters[1] + beta[1]));

        // coefficients n1 in [1., 1000]
        // setting n1<1. leads to strong numerical instabilities

        Parameters[2] = max ( 1., min(1000., Parameters[2] + beta[2]));

		// coefficients n2,n3 in [0.001, 1000]

		Parameters[3] = max ( 0.001, min(1000., Parameters[3] + beta[3]));
		Parameters[4] = max ( 0.001, min(1000., Parameters[4] + beta[4]));

		// coefficients x0 and y0
        //truncate translation to avoid huge gaps
        beta[5] = min(0.01, max(-0.01,beta[5]));
        beta[6] = min(0.01, max(-0.01,beta[6]));
        beta[7] = min(PI/100, max(-PI/100,beta[7]));

		Parameters[9] += beta[5];
		Parameters[10] += beta[6];

        // rotational offset tht0

        Parameters[7] += beta[7];


		//
		//	Evaluate chisquare with new values
		//


		OldChiSquare=ChiSquare;

		double NewChiSquare=0;

		//BuildInverseTransformationMatrices(S, R, T, Matrices);

        x0 = Get_xoffset();
		y0 = Get_yoffset();
		tht0 = Get_thtoffset();

		//First define inverse translation T^-1

        Tr << 1 , 0 , -x0 ,
              0 , 1 , -y0 ,
              0 , 0 , 1;

        //Then define inverse rotation, i.e. transposed rotation

        Rot << cos(tht0) , sin(tht0) , 0 ,
              -sin(tht0) , cos(tht0) , 0 ,
               0 , 0 , 1;


		for(i=0; i<Data.size(); i++)
		{
            //inverse transform is Rot * Tr

            Vector2d dum(Data[i]);
           	Vector3d dum2(dum[0],dum[1],1);

            //apply inverse transform
           	Vector3d dum3 ( Rot * (Tr * dum2) );

           Vector2d P(dum3[0],dum3[1]);

           	x = dum3[0]; y = dum3[1];



            if (P.squaredNorm()<EPSILON) continue; // avoids division by zero

            tht = atan2(y,x); if( tht<0) tht+=2.*PI;
            //avoid non differentiable cases

            if ( fabs(sin (tht*p*0.25/q) < EPSILON) || fabs(cos (tht*p*0.25/q) < EPSILON) ) continue;

            //call to implicit function

			f = (*this.*pt2ConstMember)(P, Df);
			h = f*pow(f*f  + Df[0]*Df[0] + Df[1]*Df[1],-0.5);
            if(Normalization)  NewChiSquare += pow(h,2);
			else               NewChiSquare += pow(f,2);

		}


		//
		// check if better result
		//

		if(	NewChiSquare>OldChiSquare )			// new result sucks-->restore old params and try with lambda 10 times bigger
		{
		    //cout <<"Iteration failed"<<endl;
            logfile<<"0 "<<lambda<<" "<<*this;
			lambda *=LAMBDA_INCR;

			for(i=0; i<Parameters.size(); i++) Parameters[i]=oldparams[i];

            //cout <<"FAIL RESULT : "<<NewChiSquare<< " / " <<OldChiSquare << " / displacement="<<beta.norm()<<endl;


		}
		else    //successful iteration
		{
		    //cout <<"SUCCES"<<endl;

            // huge improvement, something may have been wrong
            // this may arise during the first iterations
            // n1 may literally explode, or tend to 0...
            // in such case, the next iteration is successful but leads to a local minimum
            // ==> it is better to verify the result with a smaller step
            // if indeed it was a correct iteration, then it will pass the next time

		    if (NewChiSquare <= 0.01*OldChiSquare
          //|| NewChiSquare >=0.999*OldChiSquare
          ) //99% improvement, impossible
		    {
		        logfile<<"2 "<<lambda<<" "<<*this;
		        lambda *=LAMBDA_INCR; // reduce the step within the search direction
                for(i=0; i<Parameters.size(); i++) Parameters[i]=oldparams[i]; // restore old parameters

            }
            else
            {
            //correct and realistic improvement

			//cout <<"SUCCESS RESULT : "<<NewChiSquare<< " / " <<OldChiSquare <<" Lamda="<<lambda<< " / displacement="<<beta.norm()<<endl;

            logfile<<"1 "<<lambda<<" "<<*this;
			lambda /=LAMBDA_INCR;


            }
		}

		if( /*beta.norm() < 1e-9 ||*/ lambda < EPSILON || lambda > 1e15) // very small displacement ==> local convergence
                {
                //cout<<"Local minimum reached"<<endl;
                STOP=true;
                }

	}	//end for(...


    //cout <<"TOTAL NUMBER OF ITERATIONS:"<<itnum<<endl;
    err = ChiSquare;
    //cout<<"FINAL SOLUTION="<<*this<<endl;
    logfile<<"1 "<<lambda<<" "<<*this;
    logfile.close();
}


void RationalSuperShape2D :: Optimize2(
    vector< Vector2d > Data,
    double &err ,
    bool Normalization,
    int functionused
    )
{

  	double NewChiSquare, ChiSquare(1e15), OldChiSquare(1e15),f(0),df(0),n1,n2,n3,p,q,x0,y0,tht0, dxdx0,dxdy0,dxdtht0, dydx0,dydy0,dydtht0, dthtdtht0;
	int i,j,k, small_improvement(0);


    logfile.open("Optimize2.txt");


	bool STOP(false);
	double oldparams[] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//, dj[16];

	double LAMBDA_INCR(10);
	double lambda(pow(LAMBDA_INCR, -12));

MatrixXd alpha, alpha2;

alpha =  MatrixXd::Zero(8,8);
alpha2 =  MatrixXd::Zero(8,8);

VectorXd beta, beta2, dj, oldbeta,  trial, sigma;

beta = VectorXd::Zero(8);
beta2 = VectorXd::Zero(8);
dj = VectorXd::Zero(8);
oldbeta = VectorXd::Zero(8);
trial = VectorXd::Zero(8);
sigma = VectorXd::Zero(8);

Matrix3d Tr,Rot, dTrdx0, dTrdy0, dRotdtht0;

//derivatives of translation matrix are constant and can be computed once for all


   // cout<<"Optimize2"<<endl<<endl;


    logfile<<"1 "<<lambda<<" "<<*this;

	double a, b, R, x, y, tht, dthtdx, dthtdy,dthtdx0, dthtdy0, drdth;
	 double h;

    vector<double> Df;

    int itnum = 0;
	for(itnum=0; itnum<1000 && STOP==false; itnum++)	{

		//store oldparams

		for(i=0; i<Parameters.size(); i++) oldparams[i]=Parameters[i];

		alpha.setZero();
        beta.setZero();
        alpha2.setZero();
        beta2.setZero();

        //cout <<"TRIAL:"<<*this<<endl;
        bool outofbounds(false);

        //cout <<"Norm on in Opt2 : "<<Normalization<<endl;
        ChiSquare = XiSquare(Data,
                             alpha,
                             beta,
                             itnum == 0,    //init x0, y0, and tht0 or not?
                             true,          //robust or not?
                             functionused,             //implicitf cuntion1
                             Normalization,         //actiate normalization or not
                             true);         //update vectors



        //cout <<"Hessian and Beta ok"<<endl;

		//
		// add Lambda to diagonal elements and solve the matrix
		//

        //Linearization of Hessian, cf Numerical Recepies
		for(k=0; k<8; k++)
		{
		    alpha(k,k) *=   1. + lambda;  //multiplicative factor to make diagonal dominant
		    alpha(k,k) +=   lambda;       //additive factor to avoid rank deficient matrix
		}


        logfile << alpha << endl<<endl;
        logfile << beta << endl<<endl;

        //solve system
        alpha.ldlt().solveInPlace(beta);

		//coefficients a and b in [0.01, 100]

        outofbounds =  Parameters[0] + beta[0] < 0.01 || Parameters[0] + beta[0] > 1000 ||
                       Parameters[1] + beta[1] < 0.01 || Parameters[1] + beta[1] > 1000 ||
                       Parameters[2] + beta[2] < 0.1    || Parameters[2] + beta[2]> 1000 ||
                       Parameters[3] + beta[3] < 0.001 || Parameters[3] + beta[3]> 1000 ||
                       Parameters[4] + beta[4] < 0.001 || Parameters[4] + beta[4]> 1000;


        logfile <<"beta apres"               <<endl<< beta <<endl;
        //system("pause");



        if( !outofbounds )
        {

        Set_a(Parameters[0]+beta[0]);
        Set_b(Parameters[1]+beta[1]);



        // coefficients n1 in [1., 1000]
        // setting n1<1. leads to strong numerical instabilities

        Set_n1( Parameters[2] + beta[2] );

		// coefficients n2,n3 in [0.001, 1000]

		Set_n2( Parameters[3] + beta[3]);
		Set_n3( Parameters[4] + beta[4]);

		// coefficients x0 and y0
        //truncate translation to avoid huge gaps

        beta[5] = min(0.01, max(-0.01,beta[5]));
        beta[6] = min(0.01, max(-0.01,beta[6]));
        beta[7] = min(PI/100., max(-PI/100.,beta[7]));



        Parameters[9] += beta[5];
		Parameters[10] += beta[6];

        // rotational offset tht0

        Parameters[7] += beta[7];


        }
        else{
           /* cout <<"OUT OF BOUNDS"<<endl;
            cout << bool(Parameters[0] + beta[0] > 0.01 && Parameters[0] + beta[0] < 100) << endl
                 << bool(Parameters[1] + beta[1] > 0.01 && Parameters[1] + beta[1] < 100) << endl
                 << bool(Parameters[2] + beta[2] > 1    && Parameters[2] + beta[2]< 1000) << endl
                 << bool(Parameters[3] + beta[3] > 0.001 && Parameters[3] + beta[3]< 1000) << endl
                 << bool(Parameters[4] + beta[4] > 0.001 && Parameters[4] + beta[4]< 1000) <<endl;
            cout <<"Param----"<<endl;
            cout <<*this<<endl;
            cout <<"beta----"<<endl;
            cout << beta <<endl;*/
            //system("pause");

           //      system("pause");


        }

		//
		//	Evaluate chisquare with new values
		//

        if (outofbounds)
        {
           // cout <<" AVANT "<<*this<<endl;
        }
		OldChiSquare=ChiSquare;

		 NewChiSquare=XiSquare(Data,
                             alpha2,
                             beta2,
                             false,    //init x0, y0, and tht0 or not?
                             false,          //robust or not?
                             functionused,             //implicitf cuntion1
                             Normalization,  //actiate normalization or not
                             false);

        if (outofbounds)
            {
            //cout <<" APRES "<<*this<<endl;
            //system("pause");
            }

        //cout <<"CHI RATIO"<<OldChiSquare <<" / "<<NewChiSquare <<endl;
        //cout <<*this<<endl;
		//
		// check if better result
		//

		if(	NewChiSquare>0.999*OldChiSquare )			// new result sucks-->restore old params and try with lambda 10 times bigger
		{
		    //cout <<"Iteration failed"<<endl;
            logfile<<"0 "<<lambda<<" "<<*this;
			lambda *=LAMBDA_INCR;

			for(i=0; i<Parameters.size(); i++) Parameters[i]=oldparams[i];

            //cout <<"FAIL RESULT : "<<NewChiSquare<< " / " <<OldChiSquare << " / displacement="<<beta.norm()<<endl;


		}
		else    //successful iteration
		{

            // huge improvement, something may have been wrong
            // this may arise during the first iterations
            // n1 may literally explode, or tend to 0...
            // in such case, the next iteration is successful but leads to a local minimum
            // ==> it is better to verify the result with a smaller step
            // if indeed it was a correct iteration, then it will pass the next time

		    if (NewChiSquare <= 0.01*OldChiSquare) //99% improvement, impossible
		    {
		        //cout <<"cas RARISSIME..."<<NewChiSquare<<" / "<<OldChiSquare<<endl;
		        logfile<<"2 "<<lambda<<" "<<*this;
		        lambda *=LAMBDA_INCR; // reduce the step within the search direction
                for(i=0; i<Parameters.size(); i++) Parameters[i]=oldparams[i]; // restore old parameters
                //system("pause");
            }
            else
            {
            //correct and realistic improvement

			//cout <<"SUCCESS RESULT : "<<NewChiSquare<< " / " <<OldChiSquare <<" Lamda="<<lambda<< " shape="<<*this<<endl;

            logfile<<"1 "<<lambda<<" "<<*this;

			lambda /=LAMBDA_INCR;
            }
        }

		if( /*beta.norm() < 1e-9 ||*/ /*lambda < EPSILON ||*/ lambda > 1e15 || NewChiSquare < 1e-5) // very small displacement ==> local convergence
                {

                //cout<<"Local minimum reached. Lambda="<<lambda<<endl;
                STOP=true;
                //system("pause");
                //break;
                }


	}	//end for(...


   // cout <<"TOTAL NUMBER OF ITERATIONS:"<<itnum<<endl;
    err = ChiSquare;
    cout<<"FINAL SOLUTION="<<*this<<endl;
    logfile<<"1 "<<lambda<<" "<<*this;
    logfile.close();
}

vector < Vector2d> RationalSuperShape2D :: InitRotationAndSymmetry( vector < Vector2d > Data , int framesize)
{

vector < Vector2d> CleanedData;
vector < Vector2d> SmoothData;
vector < Vector2d> OffsetData;

    // compute the radius for each point and store in an array r
    vector <double> r, angle;
    for (int i=0;  i< Data.size(); i++){

        Vector2d P(Data[i]);
        angle.push_back( atan2(P[1],P[0]) );
    }


 //bubble sort, not really efficient but acceptable for such small arrays
    for (int i=0;  i< Data.size() - 1; i++){
        for (int j=i+1;  j< Data.size() ; j++) {
           if(angle[j] < angle[i])
            {
                swap (angle[j] , angle[i]);
                Data[i].swap(Data[j]);
            }
        }
    }
    for (int i=0;  i< Data.size(); i++){
        Vector2d P(Data[i]);
//        cout<< "Sorted Data#"<< i+1<< "= " << Data[i] <<endl;
//        cout<< "Angle of data #"<< i+1<< "= " << angle[i] <<endl;
//        system("pause");
        r.push_back( (P).norm() ) ;
    }
    //system("pause");

//now compute avg radius and variance radius over N angular sectors

vector <bool> ShouldBeKept;
for(int j=0; j< Data.size(); j++)   ShouldBeKept.push_back(false);

//compute globalavgr and globalvarr
double globalavgr(0), globalvarr(0);
  for(int j=0; j< Data.size(); j++)
  {
      globalavgr += r[j];
  }

  globalavgr /= Data.size();

  for(int j=0; j< Data.size(); j++)
  {
      globalvarr += pow( globalavgr - r[j],2);
  }

  globalvarr /= Data.size();

// cout <<"GLOBAL AVG="<<globalavgr<< " GLOBAL SIGMA"<<sqrt(globalvarr)<<endl;



double s(1);
for(int j=0; j< Data.size(); j++)
  {

      if ( globalavgr - s*sqrt(globalvarr)<= r[j] && r[j]<= globalavgr + s*sqrt(globalvarr))
        {
            ShouldBeKept[j] = true;
            // cout << "POINT = "<< j+1 << endl;
            // cout<< Data[j]<< endl;
            // cout <<" R=" << r[j] << endl;
            // cout << globalavgr - s*sqrt(globalvarr) <<" "<< globalavgr + s*sqrt(globalvarr)<<endl;
            CleanedData.push_back(Data[j]);
        }
      else ShouldBeKept[j] = false;
  }

r.clear();
for(int j=0; j< CleanedData.size(); j++)
{
//    cout << "Cleaned Data#" << j+1 << " = "<< CleanedData[j] << endl;
//    system("pause");
    r.push_back( CleanedData[j].norm() ) ;

}

cout << "Number of points = "<< Data.size() << endl;
//system("pause");

//UP TO HERE? EVERYTHING WAS RIGHT


//HERE STARTS THE MESS...

    //now try to count the number of maxima and minima

    int nmax(0), nmin(0);
    bool firstminfound = false;
    glPointSize(5);


    // change CleanedData with SmoothData
    for (int i=framesize;  i< SmoothData.size() + framesize; i++){

    // Vector2d A (CleanedData[i-framesize]), B (CleanedData[i]), C(CleanedData[(i+framesize)% CleanedData.size()]);

    // double angle1 (acos(A.dot(B) / (A.norm() * B.norm()))), angle2(acos(B.dot(C) / (C.norm() * B.norm())));

    //if (true) //(fabs(angle1 - angle2) < PI / 5)
    {
        double ri = r[i%SmoothData.size()];

        bool ismax = true;
        for (int j=-framesize; j<framesize+1; j++)
        {
            double rj = r[(i+j)%SmoothData.size()];
            if ( rj > ri) ismax = false;
        }

        if(ismax)  {
            nmax++;
            glColor3f(1,0,0);
            glBegin(GL_POINTS);
            Vector2d Q(SmoothData[i%SmoothData.size()]);
            glVertex3f(Q[0],Q[1],0);
            glEnd();
//           cout << "Maximum radius = "<< ri << "at " << i%SmoothData.size() << endl;
//            system("pause");
            }

        bool ismin = true;
        for (int j=-framesize; j<framesize+1; j++)
        {
            double rj = r[(i+j)%SmoothData.size()];
            if ( rj < ri) ismin = false;
        }

        if(ismin) {

        /*    if(!firstminfound){
                Vector2d P(CleanedData[i]);
                //set rotation
                Set_thtoffset(-atan2(P[1], P[0]));

                cout <<" ROTATION=" << Get_thtoffset() <<endl;
                system("pause");
                firstminfound = true;
            } */
            nmin++;
            glColor3f(0,1,0);
            glBegin(GL_POINTS);
            Vector2d Q(SmoothData[i%SmoothData.size()]);
            glVertex3f(Q[0],Q[1],0);
            glEnd();
            cout << "Minimum radius = "<< ri << "at " << i%SmoothData.size() << endl;
//            system("pause");
            }

            //if (ismin || ismax) cout <<"LOCAL MAX AT=CleanedData["<<i%CleanedData.size()<<"] / "<<CleanedData.size()<<" = "<<CleanedData[i]<<endl;
    }
    }

    int m = max(nmin, nmax);

   // if (m == 3) m = 6;
   // if (m > 8)  m = 4;

    //Set_p(m);

    return CleanedData;


}


void RationalSuperShape2D :: SpecialDrawCurve(double R, double G, double B, const int width , const int sampling){
     // curve sampling

     //adjust sampling values to pass through the maximum radius values

     PointList.clear();


    glDisable(GL_LIGHTING);


     int sample = sampling;//(sampling - (sampling % (int)(Parameters[5]))) / (int)(Parameters[5]);

     double thtoffset(Get_thtoffset()), gap (2.*PI*Get_q() / (sample)), angle(0);


     for (int  j = 0; j < 1/*Parameters[6]/2*/; j++)
         {
         for( int i=0; i < sample; i++)
              {
                  double R(radius(angle));

                  Vector2d P(R*cos(angle + thtoffset) + Get_xoffset(), R*sin(angle + thtoffset) + Get_yoffset());

                  P *= 0.5*r;



                  P[0] += -ImRes[0]*0.5 +avg[0];
                  P[1] += ImRes[1]*0.5 -avg[1];



                  PointList.push_back(Vector3d(
                    P[0],
                    P[1],
                    0));
                  angle = (double) (i)*gap;
              }
          }

     //rendering
     double n = Parameters[6] * sample;

     glLineWidth(5);
     glColor3f(0,1,0);


     glBegin(GL_LINE_STRIP);
     for ( int i = 0; i < PointList.size(); i++)
        {
             glVertex3f(PointList[i][0],PointList[i][1],PointList[i][2]);
        }
     glEnd();

     PointList.clear();
}
