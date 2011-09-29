/////////////
//
//	useful.cpp
//
/////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <GL/glut.h>

#include "string.h"
#include "useful.h"
#include "Constante.h"
#include "scene.h"
#include "random-standalone.h"

#include "SuperFormula.h"

#include "Eigen/Core"
#include "Eigen/Geometry"
#include "Eigen/LU"
#include "Eigen/Cholesky"
#include "Eigen/SVD"
// import most common Eigen types
//USING_PART_OF_NAMESPACE_EIGEN
#include "Eigen/Dense"

#include <jpeglib.h>
#include <jerror.h>

using namespace std;
using namespace Eigen;

extern ofstream logfile;
extern Random R1;

extern GLuint TexNum[1];

void
ScreenShot(std::string Fichier)
{
  // Pour g�rer le buffer
  unsigned char *buffer;

  // Nom du fichier en sortie
  //	char Fichier[50] = Fichiersd.c_str();

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);
  int w = viewport[2];//Resolution_X;
  int h = viewport[3];//Resolution_Y;

  // Taille du buffer
  int buf_size = 18 + (w * h * 3); // HEADER_SIZE	==> 18

  // Variable pour un compteur
  int i;

  // Variable temporaire
  unsigned char temp;

  // Fichier en sortie
  FILE *Fichier_Screenshot_Out;
  int Nombre_Screenshot = 0;

  // Nom du fichier TGA en sortie
  //sprintf(Fichier,"Screenshot_%d.tga",Nombre_Screenshot);

  // Ouverture de ce fichier
  if (!(Fichier_Screenshot_Out = fopen(Fichier.c_str(), "wb")))
    return;

  // On incr�mente le nombre de screenshot
  Nombre_Screenshot++;

  // Alloaction de la m�moire pour lire depuis le frame buffer
  if (!(buffer = (unsigned char *) calloc(1, buf_size)))
    return;

  // Informations de l'en-t�te du fichier
  buffer[2] = 2; // Non-compress�
  buffer[12] = w & 255;
  buffer[13] = w >> 8;
  buffer[14] = h & 255;
  buffer[15] = h >> 8;
  buffer[16] = 24; // 24 bits par pixel

  // Lecture depuis le frame buffer
  glReadPixels(0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buffer + 18);

  // Conversion de RGB vers BGR
  for (i = 18; i < buf_size; i += 3)
    {
      temp = buffer[i];
      buffer[i] = buffer[i + 2];
      buffer[i + 2] = temp;
    }

  // Ecriture de l'en-t�te du fichier + l'image dans le fichier
  fwrite(buffer, sizeof(unsigned char), buf_size, Fichier_Screenshot_Out);

  // Fermeture du fichier
  fclose(Fichier_Screenshot_Out);

  // On lib�re le buffer
  free(buffer);

  //	cout <<"Fin export"<<endl;

}

Vector3d
DoubleToColor(const double d)
{

  if (d < 0)
    return Vector3d(0, 0, 0);
  if (0 <= d && d < 0.25)
    {
      return Vector3d(0, d * 4.0, 1);
    }

  if (0.25 <= d && d < 0.5)
    {
      return Vector3d(0, 1.0, 2.0 - 4.0 * d);
    }

  if (0.5 <= d && d < 0.75)
    {
      return Vector3d(4.0 * d - 2.0, 1.0, 0);
    }

  if (0.75 <= d && d <= 1.0)
    {
      return Vector3d(1.0, 4.0 - 4.0 * d, 0);
    }

  return Vector3d(1, 1, 1);

}

Vector3d
DoubleToColorDiscrete(const double d, double n)
{

  if (d < 0)
    return Vector3d(0, 0, 0);

  //Vector3d BaseColor(0,-1,-4);
  //Vector3d Increment(1.5,2,5);

  Vector3d BaseColor(0.25, -1, -4);
  Vector3d Increment(1.5, 2, 5);

  for (int i = 0; i < n; i++)
    if (0 + (double) i / n <= d && d <= double(i + 1) / n)
      {
        return BaseColor + (i / n) * Increment;
        //Vector3d(0.5-0.25*i/n , 0.75-0.1*i/n ,0.5-0.75*i/n);
      }

  return Vector3d(0.75, 0, 0);

}

Vector3d
DoubleToGreyDiscrete(const double d, double n)
{

  if (d < 0)
    return Vector3d(0, 0, 0);

  for (int i = 0; i < n; i++)
    if ((double) i / n <= d && d < double(i + 1) / n)
      {
        return Vector3d(i / n, i / n, i / n);
      }

  return Vector3d(0, 0, 0);

}

double
ColorToDouble(Vector3d RGB)
{

  int key = (int) (RGB[0] > 0) * 4 + (int) (RGB[1] > 0) * 2
      + (int) (RGB[2] > 0);
  double t;
  switch (key)
    {

  case 1:
    t = 0.0;
    break; //ok	--> (0,0,1)
  case 2:
    t = 0.5;
    break; //ok	-->	(0,1,0)
  case 4:
    t = 1;
    break; //ok	--> (1,0,0)
  case 3:
    {

      if (RGB[1] == 1)
        t = 0.5 - 0.25 * RGB[2]; //ok	-->	(0,1,X)
      else
        t = RGB[1] * 0.25; //ok	--> (0,X,1)
    }
    break;

  case 6:
    {

      if (RGB[1] == 1)
        t = RGB[0] * 0.25 + 0.5; //ok	--> (X,1,0)
      else
        t = 1 - 0.25 * RGB[1]; //ok	--> (1,X,0)
    }
    break;
  default:
    return 0;
    }
  return t;
}

//--
//
// StringToBinding
//
//--
// Convert a string to an attribute binding

//--
//
// BindingToString
//
//--
// Convert an attribute binding to a string

void
RFunctions()
{

  int n = 800; // grid resolution

  //grid allocation
  double **Coord = new double*[n + 1];
  for (int i = 0; i < n + 1; i++)
    Coord[i] = new double[n + 1];

  //variables for 4 disks
  double f1, f2, f3, f4; // implicit field values
  Vector2d C1(1, 1), C2(1, -1), C3(-1, -1), C4(-1, 1);//centers
  double r1(1), r2(1.5), r3(2), r4(2.5);//radii

  //sampling and grid position
  double gap = 10.0 / (n - 1);
  Vector2d Pmin(-5, -5), Pmax(5, 5);
  double fmin(1e30), fmax(-1e30);

  // Rfunction evaluation

  glDisable(GL_LIGHTING);
  // for all the points of the grid
  for (int i = 0; i < n + 1; i++)
    for (int j = 0; j < n + 1; j++)
      {
        // get the coordinates
        Vector2d P(-5 + gap * i, -5 + gap * j);

        //Apply the implicit fields of the fours disks to the point
        f1 = r1 - (P - C1).norm();

        Vector2d Df1 = Vector2d(-(P[0] - C1[0]) / (P - C1).norm(), -(P[1]
            - C1[1]) / (P - C1).norm());

        f4 = r2 - (P - C2).norm();

        Vector2d Df4 = Vector2d(-(P[0] - C2[0]) / (P - C2).norm(), -(P[1]
            - C2[1]) / (P - C2).norm());

        f3 = r3 - (P - C3).norm();

        Vector2d Df3 = Vector2d(-(P[0] - C3[0]) / (P - C3).norm(), -(P[1]
            - C3[1]) / (P - C3).norm());

        f2 = r4 - (P - C4).norm();

        Vector2d Df2 = Vector2d(-(P[0] - C4[0]) / (P - C4).norm(), -(P[1]
            - C4[1]) / (P - C4).norm());

        //using binary Rfunction


        double t = 0;

        // (D1 v D2) v (D3 v D4)

        //U1 = union of disks 1 and 2
        double tmp1 = f1 + f2 + t * sqrt(f2 * f2 + f1 * f1);
        //U2 = union of disks 3 and 4
        Vector2d Dtmp1 = Vector2d(Df1[0] + Df2[0] + (Df1[0] * f1 + Df2[0] * f2)
            / sqrt(f1 * f1 + f2 * f2), Df1[1] + Df2[1] + (Df1[1] * f1 + Df2[1]
            * f2) / sqrt(f1 * f1 + f2 * f2));

        double tmp2 = f3 + f4 + t * sqrt(f3 * f3 + f4 * f4);

        Vector2d Dtmp2 = Vector2d(Df3[0] + Df4[0] + (Df3[0] * f3 + Df4[0] * f4)
            / sqrt(f3 * f3 + f4 * f4), Df3[1] + Df4[1] + (Df3[1] * f3 + Df4[1]
            * f4) / sqrt(f3 * f3 + f4 * f4));

        double f = tmp1 + tmp2 + t * sqrt(tmp1 * tmp1 + tmp2 * tmp2);

        Vector2d Df = Vector2d(Dtmp1[0] + Dtmp2[0] + (Dtmp1[0] * tmp1
            + Dtmp2[0] * tmp2) / sqrt(tmp1 * tmp1 + tmp2 * tmp2), Dtmp1[1]
            + Dtmp2[1] + (Dtmp1[1] * tmp1 + Dtmp2[1] * tmp2) / sqrt(tmp1 * tmp1
            + tmp2 * tmp2)

        );

        /*
         // D1 v ( D2 v ( D3 v D4 ) )

         //first step : (D3 v D4) --> Evaluation and gradient
         double tmp1 = f3 + f4 + sqrt(f3*f3+f4*f4);

         Vector2d Dtmp1 = Vector2d(
         Df3[0] + Df4[0] + (Df3[0]*f3 + Df4[0]*f4)/sqrt(f3*f3+f4*f4),
         Df3[1] + Df4[1] + (Df3[1]*f3 + Df4[1]*f4)/sqrt(f3*f3+f4*f4));

         //second step : D2 v (D3 v D4) --> Evaluation and gradient
         double tmp2 = f2 + tmp1 + sqrt(f2*f2+tmp1*tmp1);
         Vector2d Dtmp2 = Vector2d(
         Df2[0] + Dtmp1[0] + (Df2[0]*f2 + Dtmp1[0]*tmp1)/sqrt(f2*f2+tmp1*tmp1),
         Df2[1] + Dtmp1[1] + (Df2[1]*f2 + Dtmp1[1]*tmp1)/sqrt(f2*f2+tmp1*tmp1));

         //third step D1 v ( D2 v ( D3 v D4 ) ) --> Evaluation and gradient
         double tmp3 = f1 + tmp2 + sqrt(f1*f1+tmp2*tmp2);
         Vector2d Df = Vector2d(
         Df1[0] + Dtmp2[0] + (Df1[0]*f1 + Dtmp2[0]*tmp2)/sqrt(f1*f1+tmp2*tmp2),
         Df1[1] + Dtmp2[1] + (Df1[1]*f1 + Dtmp2[1]*tmp2)/sqrt(f1*f1+tmp2*tmp2));
         double f =  tmp3;
         */
        // using Nary Rfunction
        /*f =
         f1 + fabs(f1) + f2 + fabs(f2) + f3 + fabs(f3) + f4 + fabs(f4)
         -
         (fabs(f1) - f1 )*(fabs(f2) - f2 )*(fabs(f3) - f3 )*(fabs(f4) - f4);

         Df =  Vector2d (2*Df1[0] +  2*Df2[0] + 2*Df3[0] + 2*Df4[0]
         ,
         2*Df1[1] +  2*Df2[1] + 2*Df3[1] + 2*Df4[1]);

         */

        double Nf = tmp1;
        //f ;//* pow((f*f + Df.squaredNorm()), -0.5);


        //Final R-function =  U1 union U2
        Coord[i][j] = Nf;

        //P[0] + P[1] ;//- 0*sqrt( P[0]*P[0] + P[1]*P[1]);
        //max(f1,f2);


        //tmp1 + tmp2 + sqrt(tmp1*tmp1 + tmp2*tmp2);

        //update min and max
        fmin = min(fmin, Coord[i][j]);
        fmax = max(fmax, Coord[i][j]);

      }

  cout << "FMIN=" << fmin << " FMAX=" << fmax << endl;
  //
  // grid rendering
  //

  glBegin(GL_QUADS);

  // for each quad of the grid
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      {
        // get each corner
        // compute associated color
        // draw it

        Vector3d col1 = DoubleToColorDiscrete((Coord[i][j] - fmin) / (fmax
            - fmin));

        glColor3f(col1[0], col1[1], col1[2]);
        glVertex3f(-5 + i * gap, -5 + j * gap, Coord[i][j]);
        //glVertex3f( -5 + i*gap, -5+j*gap, 0);

        Vector3d col2 = DoubleToColorDiscrete((Coord[i + 1][j] - fmin) / (fmax
            - fmin));

        glColor3f(col2[0], col2[1], col2[2]);
        glVertex3f(-5 + (i + 1) * gap, -5 + j * gap, Coord[i + 1][j]);
        //glVertex3f( -5 + (i+1)*gap, -5+j*gap, 0);

        Vector3d col3 = DoubleToColorDiscrete((Coord[i + 1][j + 1] - fmin)
            / (fmax - fmin));

        glColor3f(col3[0], col3[1], col3[2]);
        glVertex3f(-5 + (i + 1) * gap, -5 + (j + 1) * gap, Coord[i + 1][j + 1]);
        //glVertex3f( -5 + (i+1)*gap, -5+(j+1)*gap, 0);

        Vector3d col4 = DoubleToColorDiscrete((Coord[i][j + 1] - fmin) / (fmax
            - fmin));

        glColor3f(col4[0], col4[1], col4[2]);
        glVertex3f(-5 + (i) * gap, -5 + (j + 1) * gap, Coord[i][j + 1]);
        //glVertex3f( -5 + (i)*gap, -5+(j+1)*gap,0);

      }
  glEnd();

  //
  //draw the 4 circles
  //

  Vector3d P;
  glColor3f(1, 0, 0);
  glLineWidth(3);

  // circle 1

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 100; i++)
    {
      P = Vector3d(C1[0], C1[1], 0) + Vector3d(r1 * cos(2 * i * PI / 100), r1
          * sin(2 * i * PI / 100), 0);
      glVertex3f(P[0], P[1], P[2]);
    }
  glEnd();

  // circle 2

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 100; i++)
    {
      P = Vector3d(C2[0], C2[1], 0) + Vector3d(r2 * cos(2 * i * PI / 100), r2
          * sin(2 * i * PI / 100), 0);
      glVertex3f(P[0], P[1], P[2]);
    }
  glEnd();

  // circle 3

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 100; i++)
    {
      P = Vector3d(C3[0], C3[1], 0) + Vector3d(r3 * cos(2 * i * PI / 100), r3
          * sin(2 * i * PI / 100), 0);
      glVertex3f(P[0], P[1], P[2]);
    }
  glEnd();

  // circle 4

  glBegin(GL_LINE_LOOP);
  for (int i = 0; i < 100; i++)
    {
      P = Vector3d(C4[0], C4[1], 0) + Vector3d(r4 * cos(2 * i * PI / 100), r4
          * sin(2 * i * PI / 100), 0);
      glVertex3f(P[0], P[1], P[2]);
    }
  glEnd();

}

void
RFunctions2()
{

  int n = 1000; // grid resolution

  //grid allocation
  double **Coord = new double*[n + 1];
  for (int i = 0; i < n + 1; i++)
    Coord[i] = new double[n + 1];

  //sampling and grid position

  Vector2d Pmin(-3, -3), Pmax(3, 3);
  double gap = (Pmax[0] - Pmin[0]) / (n - 1);
  double fmin(1e30), fmax(-1e30);

  // Rfunction evaluation

  RationalSuperShape2D RS;
  /*
   RS.Init(
   2,1,
   10 ,-1, 10,
   6,1
   );
   */

  /*
   //used in grapp paper
   RS.Init(
   1,1,
   0.5,3.5,3.5,
   5,3
   );
   */
  RS.Init(1, 1, -0.25, 3.5, 3.5, 7, 3);

  bool FlatOn = true;

  RS.DrawCurve(0, 1, 1, 3, 300);

  glDisable(GL_LIGHTING);
  // for all the points of the grid


  for (int i = 0; i < n + 1; i++)
    for (int j = 0; j < n + 1; j++)
      {
        // get the coordinates
        Vector2d P = Pmin + Vector2d(gap * i, gap * j);
        vector<double> G;

        //Apply the implicit fields of the fours disks to the point

        if (P.norm() != 0)
          {
            double f = RS.ImplicitFunction1(P, G);
            Vector2d Gradient(G[0], G[1]);
            double h = f * pow(f * f + Gradient.squaredNorm(), -0.5);

            Coord[i][j] = h;
            //if (G.norm()> 3) G = Vector2d(0,0);
            fmin = min(fmin, Coord[i][j]);
            fmax = max(fmax, Coord[i][j]);

            if (i % 30 == 0 && j % 30 == 0 /*&& i >= n/2-1 && j>=n/2-1*/)
              {
                //gradient of f

                Gradient.normalize();
                Gradient = 0.1 * Gradient;
                glBegin(GL_LINES);
                glColor3f(0, 0, 1);
                glVertex3f(P[0], P[1], 0);
                glColor3f(0, 1, 0);
                glVertex3f(P[0] + Gradient[0], P[1] + Gradient[1], 0);
                glEnd();

                //gradient of the normalized function

                /*
                 Vector2d H ;

                 H = Gradient*h*(1.-h*h)/f;



                 H.Normalize();
                 H = 0.1*H;
                 glBegin(GL_LINES);
                 glColor3f(0,0,1);
                 glVertex3f(P[0], P[1], 0);
                 glColor3f(0,1,0);
                 glVertex3f(P[0]+H[0], P[1]+H[1], 0);
                 glEnd();
                 */

              }
          }
      }

  cout << "FMIN=" << fmin << " FMAX=" << fmax << endl;

  cout << RS.radius(0) << " " << RS.radius(0) << endl;
  //
  // grid rendering
  //

  glBegin(GL_QUADS);

  // for each quad of the grid
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      {
        // get each corner
        // compute associated color
        // draw it

        Vector3d col1 = DoubleToColorDiscrete((Coord[i][j] - fmin) / (fmax
            - fmin));
        Vector3d col2 = DoubleToColorDiscrete((Coord[i + 1][j] - fmin) / (fmax
            - fmin));
        Vector3d col3 = DoubleToColorDiscrete((Coord[i + 1][j + 1] - fmin)
            / (fmax - fmin));
        Vector3d col4 = DoubleToColorDiscrete((Coord[i][j + 1] - fmin) / (fmax
            - fmin));

        Vector3d AVG = 0.25 * (col1 + col2 + col3 + col4);

        glColor3f(col1[0], col1[1], col1[2]);
        glColor3f(AVG[0], AVG[1], AVG[2]);
        if (FlatOn)
          glVertex3f(Pmin[0] + i * gap, Pmin[1] + j * gap, 0);
        else
          glVertex3f(Pmin[0] + i * gap, Pmin[1] + j * gap, Coord[i][j]);

        glColor3f(col2[0], col2[1], col2[2]);
        glColor3f(AVG[0], AVG[1], AVG[2]);
        if (FlatOn)
          glVertex3f(Pmin[0] + (i + 1) * gap, Pmin[1] + j * gap, 0);
        else
          glVertex3f(Pmin[0] + (i + 1) * gap, Pmin[1] + j * gap,
              Coord[i + 1][j]);

        glColor3f(col3[0], col3[1], col3[2]);
        glColor3f(AVG[0], AVG[1], AVG[2]);
        if (FlatOn)
          glVertex3f(Pmin[0] + (i + 1) * gap, Pmin[1] + (j + 1) * gap, 0);
        else
          glVertex3f(Pmin[0] + (i + 1) * gap, Pmin[1] + (j + 1) * gap, Coord[i
              + 1][j + 1]);

        glColor3f(col4[0], col4[1], col4[2]);
        glColor3f(AVG[0], AVG[1], AVG[2]);
        if (FlatOn)
          glVertex3f(Pmin[0] + (i) * gap, Pmin[1] + (j + 1) * gap, 0);
        else
          glVertex3f(Pmin[0] + (i) * gap, Pmin[1] + (j + 1) * gap, Coord[i][j
              + 1]);

      }
  glEnd();

}

void
RpUnion(double f1, double f2, vector<double> Df1, vector<double> Df2,
    double &f, vector<double> &Df)
{
  assert(Df1.size() == Df2.size());
  Df.clear();

  f = f1 + f2 + sqrt(f1 * f1 + f2 * f2);

  if (f1 != 0 || f2 != 0) // function differentiable
    for (int i = 0; i < Df1.size(); i++)
      Df.push_back(Df1[i] + Df2[i] + (f1 * Df1[i] + f2 * Df2[i]) / sqrt(f1 * f1
          + f2 * f2));
  else
    //function not differentiable, set everything to zero
    for (int i = 0; i < Df1.size(); i++)
      Df.push_back(0);
}

void
RpIntersection(double f1, double f2, vector<double> Df1, vector<double> Df2,
    double &f, vector<double> &Df)
{
  assert(Df1.size() == Df2.size());
  Df.clear();

  f = f1 + f2 - sqrt(f1 * f1 + f2 * f2);

  if (f1 != 0 || f2 != 0) // function differentiable
    for (int i = 0; i < Df.size(); i++)
      Df.push_back(Df1[i] + Df2[i] - (f1 * Df1[i] + f2 * Df2[i]) / sqrt(f1 * f1
          + f2 * f2));
  else
    //function not differentiable, set everything to zero
    for (int i = 0; i < Df1.size(); i++)
      Df.push_back(0);

}

void
gaussj2(double **a, int n, double **b, int m)

//Linear equation solution by Gauss-Jordan elimination, equation (2.1.1) above. a[1..n][1..n]
//is the input matrix. b[1..n][1..m] is input containing the m right-hand side vectors. On
//output, a is replaced by its matrix inverse, and b is replaced by the corresponding set of solution
//vectors.

{
  int *indxc, *indxr, *ipiv;
  int i, icol, irow, j, k, l, ll;
  double big, dum, pivinv, tmp;
  //The integer arrays ipiv, indxr, and indxc are
  //used for bookkeeping on the pivoting.

  indxc = new int[n];
  indxr = new int[n];
  ipiv = new int[n];

  for (j = 0; j < n; j++)
    ipiv[j] = 0;

  for (i = 0; i < n; i++)
    { //This is the main loop over the columns to be	reduced.
      big = 0.0;
      for (j = 0; j < n; j++) //This is the outer loop of the search for a pivot element.
        if (ipiv[j] != 0)
          for (k = 0; k < n; k++)
            {
              if (ipiv[k] == 0)
                {
                  if (fabs(a[j][k]) >= big)
                    {
                      big = fabs(a[j][k]);
                      irow = j;
                      icol = k;
                      cout << j << " " << k << endl;
                    }
                }
            }
      ++(ipiv[icol]);

      cout << "pivot" << endl;

      //We now have the pivot element, so we interchange rows, if needed, to put the pivot
      //element on the diagonal. The columns are not physically interchanged, only relabeled:
      //indxc[i], the column of the ith pivot element, is the ith column that is reduced, while
      //indxr[i] is the row in which that pivot element was originally located. If indxr[i] .=
      //indxc[i] there is an implied column interchange. With this form of bookkeeping, the
      //solution b�s will end up in the correct order, and the inverse matrix will be scrambled
      //by columns.

      if (irow != icol)
        {
          for (l = 0; l < n; l++)
            {
              tmp = a[irow][l];
              a[irow][l] = a[icol][l];
              a[icol][l] = tmp;
            }
          for (l = 0; l < m; l++)
            {
              tmp = b[irow][l];
              b[irow][l] = b[icol][l];
              b[icol][l] = tmp;
            }
        }

      indxr[i] = irow; //We are now ready to divide the pivot row by the
      //pivot element, located at irow and icol.

      indxc[i] = icol;

      if (a[icol][icol] == 0.0)
        cout << "gaussj: Singular Matrix" << endl;
      pivinv = 1.0 / a[icol][icol];
      a[icol][icol] = 1.0;

      for (l = 0; l < n; l++)
        a[icol][l] *= pivinv;
      for (l = 0; l < m; l++)
        b[icol][l] *= pivinv;

      for (ll = 0; ll < n; ll++) //Next, we reduce the rows...
        if (ll != icol)
          {// ...except for the pivot one, of course.
            dum = a[ll][icol];
            a[ll][icol] = 0.0;
            for (l = 0; l < n; l++)
              a[ll][l] -= a[icol][l] * dum;
            for (l = 0; l < m; l++)
              b[ll][l] -= b[icol][l] * dum;
          }
    }

  //This is the end of the main loop over columns of the reduction. It only remains to unscramble
  //the solution in view of the column interchanges. We do this by interchanging pairs of
  //columns in the reverse order that the permutation was built up.

  for (l = n - 1; l >= 0; l--)
    {
      if (indxr[l] != indxc[l])
        for (k = 0; k < n; k++)
          {
            tmp = a[k][indxr[l]];
            a[k][indxr[l]] = a[k][indxc[l]];
            a[k][indxc[l]] = tmp;
          }
    } //And we are done.

  //	cout<<"FINITO"<<endl;
  delete ipiv;
  delete indxr;
  delete indxc;
}

bool
gaussj_new(double **a, int n, double**b)
{

  double big, dum, pivinv;
  int i, icol, irow, j, k, l, ll, m;
  int indxc[132], indxr[132], ipiv[132];

  m = 1;

  for (j = 0; j < n; j++)
    {
      ipiv[j] = 0;
    }

  for (i = 0; i < n; i++)
    {
      big = 0.0;
      for (j = 0; j < n; j++)
        {
          if (ipiv[j] != 1)
            {
              for (k = 0; k < n; k++)
                {
                  if (ipiv[k] == 0)
                    {
                      if (fabs(a[j][k]) >= big)
                        {
                          big = fabs(a[j][k]);
                          irow = j;
                          icol = k;
                        }
                    }
                  else if (ipiv[k] > 1)
                    {
                      printf("pause 1 in GAUSSJ - singular matrix\n");

                      return (false);
                      /* exit(1); */
                    }
                }
            }
        }

      ipiv[icol] = ipiv[icol] + 1;
      if (irow != icol)
        {
          for (l = 0; l < n; l++)
            {
              dum = a[irow][l];
              a[irow][l] = a[icol][l];
              a[icol][l] = dum;
            }
          for (l = 0; l < m; l++)
            {
              dum = b[irow][l];
              b[irow][l] = b[icol][l];
              b[icol][l] = dum;
            }
        }

      indxr[i] = irow;
      indxc[i] = icol;
      if (a[icol][icol] == 0.0)
        {
          //printf("pause 2 in GAUSSJ - singular matrix\n");
          return false;
          a[icol][icol] = 1e-30;
        }

      pivinv = 1.0 / a[icol][icol];
      a[icol][icol] = 1.0;
      for (l = 0; l < n; l++)
        a[icol][l] = a[icol][l] * pivinv;
      for (l = 0; l < m; l++)
        b[icol][l] = b[icol][l] * pivinv;
      for (ll = 0; ll < n; ll++)
        {
          if (ll != icol)
            {
              dum = a[ll][icol];
              a[ll][icol] = 0.0;
              for (l = 0; l < n; l++)
                a[ll][l] = a[ll][l] - a[icol][l] * dum;
              for (l = 0; l < m; l++)
                b[ll][l] = b[ll][l] - b[icol][l] * dum;
            }
        }
    }

  for (l = n - 1; l >= 0; l--)
    {
      if (indxr[l] != indxc[l])
        {
          for (k = 0; k < n; k++)
            {
              dum = a[k][indxr[l]];
              a[k][indxr[l]] = a[k][indxc[l]];
              a[k][indxc[l]] = dum;
            }
        }
    }
  return (true);
}

bool
gaussj_new_updated(double **a, int n, double**b)
{

  double big, dum, pivinv;
  int i, icol, irow, j, k, l, ll, m;
  int *indxc = new int[n], *indxr = new int[n], *ipiv = new int[n];

  m = 1;

  for (j = 0; j < n; j++)
    {
      ipiv[j] = 0;
    }

  for (i = 0; i < n; i++)
    {

      big = 0.0;
      for (j = 0; j < n; j++)
        {

          if (ipiv[j] != 1)
            {

              for (k = 0; k < n; k++)
                {

                  if (ipiv[k] == 0)
                    {

                      if (fabs(a[j][k]) >= big)
                        {

                          big = fabs(a[j][k]);
                          irow = j;
                          icol = k;
                        }
                    }
                  else if (ipiv[k] > 1)
                    {
                      printf("pause 1 in GAUSSJ - singular matrix\n");
                      return (false);
                      /* exit(1); */
                    }
                }
            }
        }

      ipiv[icol] = ipiv[icol] + 1;
      if (irow != icol)
        {

          for (l = 0; l < n; l++)
            {
              dum = a[irow][l];
              a[irow][l] = a[icol][l];
              a[icol][l] = dum;
            }
          for (l = 0; l < m; l++)
            {
              dum = b[irow][l];
              b[irow][l] = b[icol][l];
              b[icol][l] = dum;
            }
        }

      indxr[i] = irow;
      indxc[i] = icol;

      if (a[icol][icol] == 0.0)
        {
          printf("pause 2 in GAUSSJ - singular matrix\n");
          a[icol][icol] = 1e-6;
        }

      pivinv = 1.0 / a[icol][icol];
      a[icol][icol] = 1.0;

      for (l = 0; l < n; l++)
        a[icol][l] = a[icol][l] * pivinv;
      for (l = 0; l < m; l++)
        b[icol][l] = b[icol][l] * pivinv;

      for (ll = 0; ll < n; ll++)
        {
          if (ll != icol)
            {
              dum = a[ll][icol];
              a[ll][icol] = 0.0;
              for (l = 0; l < n; l++)
                a[ll][l] = a[ll][l] - a[icol][l] * dum;
              for (l = 0; l < m; l++)
                b[ll][l] = b[ll][l] - b[icol][l] * dum;
            }
        }
    }

  for (l = n - 1; l >= 0; l--)
    {
      if (indxr[l] != indxc[l])
        {
          for (k = 0; k < n; k++)
            {
              dum = a[k][indxr[l]];
              a[k][indxr[l]] = a[k][indxc[l]];
              a[k][indxc[l]] = dum;
            }
        }
    }

  delete indxc;
  delete indxr;
  delete ipiv;

  return (true);
}

void
IEEE_DrawingForRGC()
{
  RationalSuperShape2D RS;
  RS.Init(1, 1, -0.15, 1, 3.5, 8, 3);

  int n = 4;

  double rdum(2.5), thtdum(0.15);
  Vector3d P(1.5, 1.5, 0);

  for (int j = 0; j < n; j++)
    {
      thtdum += 2. * PI / n;
      P = Vector3d(rdum * cos(thtdum), rdum * sin(thtdum), 0);
      double x = P[0], y = P[1];

      double tht(atan2(y, x)), thtbase(tht);
      if (tht < 0)
        thtbase += 2. * PI;

      RS.DrawCurve(0.2, 0.2, 0.2, 2, 1000);
      glPointSize(8);
      glBegin(GL_POINTS);
      //compute all intersections and associated partial derivatives


      glColor3f(0, 0, 1);
      glVertex3f(P[0], P[1], P[2]);

      glColor3f(1, 0, 0);

      for (int i = 0; i < RS.Get_q(); i++)
        {
          tht = thtbase + i * 2. * PI;

          double R = RS.radius(tht);
          glVertex3f(R * cos(tht), R * sin(tht), 0);
        }

      glEnd();

      glColor3f(0, 0, 0);
      glLineWidth(1);
      //    glEnable(GL_LINE_STIPPLE);
      glLineStipple(10, 0xAAAA);

      glBegin(GL_LINES);
      glVertex3f(P[0], P[1], P[2]);
      glVertex3f(0, 0, 0);
      glEnd();

      //  glDisable(GL_LINE_STIPPLE);
    }

}

void
Nature()
{

  int n = 800; // grid resolution

  //grid allocation
  double **Coord = new double*[n + 1];
  for (int i = 0; i < n + 1; i++)
    Coord[i] = new double[n + 1];

  //variables for 4 disks
  double f1, f2, f3, f4; // implicit field values
  Vector2d C1(1, 1), C2(1, -1), C3(-1, -1), C4(-1, 1);//centers
  double r1(1), r2(1.5), r3(2), r4(2.5);//radii

  //sampling and grid position
  double gap = 10.0 / (n - 1);
  Vector2d Pmin(-3, -3), Pmax(3, 3);
  double fmin(1e30), fmax(-1e30);

  // Rfunction evaluation
  RationalSuperShape2D RS1;
  double m1 = 3, q1 = 1;
  RS1.Init(2, 2, 10, 10, 10, m1, q1);

  /*
   //for nbature paper
   RS1.Init(
   2,2,
   0.5,1.5,1.5,
   5,2
   );
   */
  RationalSuperShape2D RS2;
  double m2 = 6, q2 = 1;
  RS2.Init(1.5, 1.5, -0.25, 0.4, 0.4, m2, q2);

  vector<double> Ddum;

  glDisable(GL_LIGHTING);
  // for all the points of the grid
  for (int i = 0; i < n + 1; i++)
    for (int j = 0; j < n + 1; j++)
      {
        // get the coordinates
        Vector2d P(-5 + gap * i, -5 + gap * j);

        //Apply the implicit fields of the fours disks to the point
        f1 = RS1.ImplicitFunction2(P, Ddum);
        f2 = RS2.ImplicitFunction2(P, Ddum);

        double t = sqrt(2);

        double f = f1 + f2 + t * sqrt(f2 * f2 + f1 * f1);

        //Final R-function =  U1 union U2
        Coord[i][j] = f;

        //update min and max
        fmin = min(fmin, Coord[i][j]);
        fmax = max(fmax, Coord[i][j]);

      }

  cout << "FMIN=" << fmin << " FMAX=" << fmax << endl;

  //
  // grid rendering
  //

  glBegin(GL_QUADS);

  // for each quad of the grid
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      {
        // get each corner
        // compute associated color
        // draw it

        Vector3d col1;
        if (Coord[i][j] < 0)
          col1 = (Coord[i][j] / fmin) * Vector3d(1, 1, 1);
        else
          col1 = DoubleToColorDiscrete(Coord[i][j] / fmax);
        glColor3f(col1[0], col1[1], col1[2]);
        glVertex3f(-5 + i * gap, -5 + j * gap, 0);

        Vector3d col2;
        if (Coord[i + 1][j] < 0)
          col2 = (Coord[i + 1][j] / fmin) * Vector3d(1, 1, 1);
        else
          col2 = DoubleToColorDiscrete(Coord[i + 1][j] / fmax);
        glColor3f(col2[0], col2[1], col2[2]);
        glVertex3f(-5 + (i + 1) * gap, -5 + j * gap, 0);

        Vector3d col3;
        if (Coord[i + 1][j + 1] < 0)
          col3 = (Coord[i + 1][j + 1] / fmin) * Vector3d(1, 1, 1);
        else
          col3 = DoubleToColorDiscrete(Coord[i + 1][j + 1] / fmax);
        glColor3f(col3[0], col3[1], col3[2]);
        glVertex3f(-5 + (i + 1) * gap, -5 + (j + 1) * gap, 0);

        Vector3d col4;
        if (Coord[i][j + 1] < 0)
          col4 = (Coord[i][j + 1] / fmin) * Vector3d(1, 1, 1);
        else
          col4 = DoubleToColorDiscrete(Coord[i][j + 1] / fmax);
        glColor3f(col4[0], col4[1], col4[2]);
        glVertex3f(-5 + (i) * gap, -5 + (j + 1) * gap, 0);

      }
  glEnd();

  RS1.DrawCurve(1, 1, 1, 1, 1000);
  RS2.DrawCurve(1, 1, 1, 1, 1000);

}

void
OBB(vector<Vector2d> Data, Vector2d &T, Vector2d &R, Vector2d &S)
{
  T << 0, 0;
  R << 0, 0;
  S << 1e15, 1e15;

  Vector2d dum(1e15, 1e15);

  for (int i = 0; i < Data.size(); i++)
    {
      dum = Data[i];
      S[0] = min((double) (fabs(dum[0])), S[0]);
      S[1] = min((double) (fabs(dum[1])), S[1]);
    }

  cout << "S AVANT=" << S << endl;

  S << 0, 0;

  //Computer Center
  for (int i = 0; i < Data.size(); i++)
    {
      T += Data[i];
    }
  T /= Data.size();

  cout << "CENTER =" << T << endl;

  //create covariance matrix
  Matrix2d mat;
  mat << 0, 0, 0, 0;

  for (int i = 0; i < Data.size(); i++)
    {
      Vector2d P(Data[i]);

      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++)
          mat(j, k) += (P[j] - T[j]) * (P[k] - T[k]);
    }

  mat(0, 0) -= T[0] * T[0];
  mat(1, 1) -= T[1] * T[1];
  mat(0, 1) -= T[0] * T[1];
  mat(1, 0) -= T[1] * T[0];

  mat /= Data.size() - 1;

  //Get eigenvalues of matrix

  SelfAdjointEigenSolver<Matrix2d> eigensolver(mat);

  Matrix2d test = eigensolver.eigenvectors();

  cout << mat << endl;

  cout << "The eigenvalues of A are:\n" << eigensolver.eigenvalues() << endl;
  cout << "Here's a matrix whose columns are eigenvectors of A "
      << "corresponding to these eigenvalues:\n" << test << endl;

  //normalize base vectors
  /*  double n = test.row(0).norm();
   test /= n;*/

  cout << "after Normalization mat=" << endl << test << endl;

  if (fabs(test.determinant() - 1.) > 1e-6)
    {
      cout << " Determinant should be 1 after normalization!!" << endl;
      system("pause");
    }

  //transpose matrix

  test.transposeInPlace();

  cout << "after transposition mat=" << endl << test << endl;
  S << 1e15, 1e15;
  for (int i = 0; i < Data.size(); i++)
    {

      dum = test * Data[i];
      S[0] = min((double) (fabs(dum[0])), S[0]);
      S[1] = min((double) (fabs(dum[1])), S[1]);
    }

  cout << "Max scale after test=" << endl << S << endl;

}

/*
 double** GetBoundingBoxTransformation(std::vector<Vector3d> PointList){

 int	i(0),j(0),nrot(0);

 double	max_eigen (-1000000000.0),EigenValues[3];

 double **TransformationMatrix;	//matrix 4x4
 double **CovarianceMatrix;		//matrix 3x3
 double **EigenVectors;			//idem

 Vector3d Average(0,0,0),Variance(0,0,0),Covariance(0,0,0);

 //allocation of the 4x4 matrices (use of class Matrix later)


 TransformationMatrix = new double*[4];
 for(  j=0; j<4; j++ ) 	TransformationMatrix[j] = new double[4];

 //allocation for the 3x3 matrices


 CovarianceMatrix = new double*[3];
 EigenVectors = new double*[3];
 for(  j=0; j<3; j++ ) {

 CovarianceMatrix[j] = new double[3];
 EigenVectors[j] = new double[3];
 }

 //  cout<<"allocation OK"<<endl;
 // 	find	center of mass, cov, var and min and max extensions
 int no(PointList.size());
 for(i = 0; i < no; i++) {

 Vector3d P(PointList[i]);
 Average		+=	P;
 }
 Average		/= (double)(no);


 for(i = 0; i < no; i++) {

 Vector3d P(PointList[i]-Average);
 Variance	+=	Vector3d(P[0]*P[0], P[1]*P[1], P[2]*P[2]);
 Covariance	+=	Vector3d(P[0]*P[2], P[1]*P[2], P[0]*P[1]);
 }

 // 	center of mass coordinates, covariances and variances




 cout<<"Average= "<<Average<<endl<<endl;

 Variance	-= Vector3d( Average[0]*Average[0], Average[1]*Average[1], Average[2]*Average[2]);
 Covariance	-= Vector3d( Average[0]*Average[2], Average[1]*Average[2], Average[0]*Average[2]);
 Variance	/= (double)(no);
 Covariance	/= (double)(no);

 //  cout<<"Average, Varinace and Covariance OK"<<endl;
 // 	arrange the numbers into matrix

 CovarianceMatrix[0][0] = Variance[0];		CovarianceMatrix[0][1] = Covariance[2];		CovarianceMatrix[0][2] = Covariance[0];
 CovarianceMatrix[1][0] = Covariance[2];		CovarianceMatrix[1][1] = Variance[1];		CovarianceMatrix[1][2] = Covariance[1];
 CovarianceMatrix[2][0] = Covariance[0];		CovarianceMatrix[2][1] = Covariance[1];		CovarianceMatrix[2][2] = Variance[2];

 //  cout<<"storage OK"<<endl;
 // 	compute eigenvectors, eigenvalues

 int ErrorValue = EigenValueDecomposition (CovarianceMatrix, 3, EigenValues, EigenVectors, &nrot);
 //  cout<<"EigenVlaue decomposition OK"<<endl;

 //  Along eigen vector with max eigen value is the smallest moment of inertia
 //  Get the index of the maximum EigenValue

 if( determinant(EigenVectors) < 0)   {
 printf("determinant not positive\n");
 //need to invert one vector!!!
 //sounds odd due to inversion ealier
 }

 //if the biggest inertia moment (ie the biggest rotation axis!) is not along the z axis then we permute

 if(EigenValues[0] > max_eigen)       { max_eigen = EigenValues[0]; i = 0; }
 if(EigenValues[1] > max_eigen)       { max_eigen = EigenValues[1]; i = 1; }
 if(EigenValues[2] > max_eigen)       { max_eigen = EigenValues[2]; i = 2; }

 logfile<<endl;
 for(int t=0; t<3; t++)		{

 for(int s=0; s<3; s++)	logfile<<EigenVectors[t][s]<<" ";
 logfile<<endl;
 }

 logfile<<endl<<max_eigen<<" Eigen MAX en"<<i<<endl;
 //
 //        if (i != 2)	{
 //
 //		for (j = 0; j < 3; j++)         {
 //
 //				double tmp=EigenVectors[j][2];
 //				EigenVectors[j][2] = EigenVectors[j][i];
 //				EigenVectors[j][i] = tmp;
 //			}
 //
 //			// we do 1 permutation of basis vector --> determinant =-1 --> multiply by -1 the permuted vector
 //			for (j = 0; j < 3; j++) EigenVectors[j][i] = -EigenVectors[j][i];
 //		}

 logfile<<endl;
 for(int t1=0; t1<3; t1++)	{

 for(int s1=0; s1<3; s1++)	logfile<<EigenVectors[t1][s1]<<" ";
 logfile<<endl;
 }


 //copying the eigenVectors into the Transformation Matrix
 for(i=0;i<3;i++)
 for(j=0;j<3;j++)
 TransformationMatrix[i][j]=EigenVectors[i][j];
 //cout<<"copying OK"<<endl;
 TransformationMatrix[0][3] = Average[0];
 TransformationMatrix[1][3] = Average[1];
 TransformationMatrix[2][3] = Average[2];
 TransformationMatrix[3][3] = 1.0;
 TransformationMatrix[3][0] = TransformationMatrix[3][1] = TransformationMatrix[3][2] = 0.0;

 //cout<<"last storage OK"<<endl;
 //desallocation of Inverse Matrix, Covariance Matrix and EigenVectors


 for( j=0; j<3; j++ ) {

 delete CovarianceMatrix[j];
 delete EigenVectors[j];
 }

 delete CovarianceMatrix;
 delete EigenVectors;

 //cout<<"desallocation OK"<<endl;
 return TransformationMatrix;
 } */

vector<Vector2d>
GenerateOutlier(vector<Vector2d> Data, double outlier_percentage,
    double radiusratio)
{
  vector<Vector2d> NewData;

  //compute average
  Vector2d C(0, 0);
  for (int i = 0; i < Data.size(); i++)
    {
      C += Data[i];
      NewData.push_back(Data[i]);
    }
  C /= Data.size();

  //compute max distance from center
  double maxdist = 0;
  for (int i = 0; i < Data.size(); i++)
    maxdist = max(maxdist, (C - Data[i]).norm());

  //now render random points within the disk centered around C with a radius R = maxdist*radiusratio
  double R, tht;
  for (int i = 0; i < Data.size() * outlier_percentage; i++)
    {
      R = (R1.uniform()) * maxdist * (radiusratio);
      tht = R1.uniform() * 2 * PI;
      Vector2d P(C[0] + R * cos(tht), C[1] + R * sin(tht));
      NewData.push_back(P);
    }

  return NewData;
}

vector<Vector2d>
ReadData2D(string filename)
{

  // read data from file data.txt
  ifstream datafile;
  datafile.open(filename.c_str());

  // Test if file is open
  if (datafile.is_open() == false)
    {
      cout << "No file found" << endl;
      exit(0);
    }
  cout << "OK" << endl;
  vector<Vector2d> Data;
  string line, word;
  vector<string> node;

  //process line by line
  while (getline(datafile, line))
    {
      //process the line
      istringstream word_stream(line);

      node.clear();
      //read word by word and store in the array
      while (word_stream >> word)
        {
          node.push_back(word);
        }

      if (node.size() != 2)
        {
          cout
              << "Syntax error while readind data : only two coordinates per line are supported"
              << endl;
          exit(0);
        }

      Vector2d P(0, 0);
      for (int i = 0; i < node.size(); i++)
        {
          P[i] = atof(node[i].c_str()); // atof is bad, but it works...
        }

      Data.push_back(P);

    }
  datafile.close();

  return Data;
}

vector<Vector2d>
RemoveData(vector<Vector2d> Data, Vector3d Plane)
{

  vector<Vector2d> FinalData;

  for (int i = 0; i < Data.size(); i++)
    {
      Vector2d P(Data[i]);
      if (Plane[0] * P[0] + Plane[1] * P[1] + Plane[2] > 0)
        FinalData.push_back(P);
    }

  return FinalData;

}

int
LoadJPG(char*filename, int nb, Vector2d &ImageRes)
{
  struct jpeg_decompress_struct cinfo; // les infos du fichiers
  struct jpeg_error_mgr jerr; // les erreurs
  GLubyte *ligne; // une ligne (?)
  GLubyte *image; // l'image sous forme de donn�es
  int ImageSize; // Taille de l'image

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo);

  // Lit le fichier et son header, puis le d�compresse
  FILE *fichier = fopen(filename, "rb");
  jpeg_stdio_src(&cinfo, fichier);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  ImageRes[0] = cinfo.image_width;
  ImageRes[1] = cinfo.image_height;

  cout << cinfo.image_width << " " << cinfo.image_height << endl;

  ImageSize = cinfo.image_width * cinfo.image_height * 3;
  image = (GLubyte *) malloc(ImageSize);
  ligne = image;

  while (cinfo.output_scanline < cinfo.output_height)
    {
      ligne = image + 3 * cinfo.image_width * cinfo.output_scanline;
      jpeg_read_scanlines(&cinfo, &ligne, 1);
    }

  jpeg_finish_decompress(&cinfo);
  jpeg_destroy_decompress(&cinfo);

  // Traitements de la texture par OpenGL
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenTextures(nb, &TexNum[nb]);

  glBindTexture(GL_TEXTURE_2D, TexNum[nb]); // Pointe la texture
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  // glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
  //  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, cinfo.image_width, cinfo.image_height,
      0, GL_RGB, GL_UNSIGNED_BYTE, image);

  fclose(fichier);
  free(image);
  return TexNum[nb];
}

