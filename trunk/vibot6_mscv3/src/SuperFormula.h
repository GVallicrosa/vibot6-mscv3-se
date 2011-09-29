//
//// SuperFormula.h
//

#ifndef _SUPERFORMULA_
#define _SUPERFORMULA_

#include <assert.h>
#include <string>
#include <vector>
#include "Eigen/Core"
// import most common Eigen types
//USING_PART_OF_NAMESPACE_EIGEN
using namespace Eigen;

using namespace std;

class RationalSuperShape2D
{

public:

  // parameters of the rational supershape
  // Parameters[0] : a
  // Parameters[1] : b

  // Parameters[2] : n1
  // Parameters[3] : n2
  // Parameters[4] : n3

  // Parameters[5] : p
  // Parameters[6] : q

  // Parameters[7] : theta offset
  // Parameters[8] : phi offset // unused in 2D

  // Parameters[9] : x offset
  // Parameters[10] : y offset
  // Parameters[11] : z offset // unused in 2D

  vector<double> Parameters;

  double r;
  Vector2d avg;
  Vector2d ImRes;

  //data storage for display
  vector<Vector3d> PointList;
  vector<Vector3d> NormalList;
  vector<Vector3i> FaceList;

  //constructors an destructors
  RationalSuperShape2D();
  RationalSuperShape2D(double a, double b, double n1, double n2, double n3,
      double p, double q, double thtoffset = 0, double phioffset = 0,
      double xoffset = 0, double y_offset = 0, double zoffset = 0);
  virtual
  ~RationalSuperShape2D();
  void
  Init(double a, double b, double n1, double n2, double n3, double p, double q,
      double thtoffset = 0, double phioffset = 0, double xoffset = 0,
      double y_offset = 0, double zoffset = 0);
  void
  Init(double a, double b, double n1, double n2, double n3);
  void
  RandomInit();

  //computation
  //double ImplicitFunction0(const Vector2d P, bool op=1, int m=1);
  double
  ImplicitFunction1(const Vector2d P, vector<double> &Dffinal);
  double
  ImplicitFunction2(const Vector2d P, vector<double> &Dffinal);
  double
  ImplicitFunction3(const Vector2d P, vector<double> &Dffinal);
  /*double ImplicitFunction4(const Vector2d P, vector <double> &Df);
   double ImplicitFunction5(const Vector2d P, vector <double> &Df);
   */

  double
  DrDa(const double);
  double
  DrDb(const double);
  double
  DrDn1(const double);
  double
  DrDn2(const double);
  double
  DrDn3(const double);

  void
  GetPartialDerivatives(double tht, double &DrDa, double &DrDb, double &DrDn1,
      double &DrDn2, double &DrDn3);

  void
  Optimize(vector<Vector2d> , double &, bool Normalization = true,
      int functionused = 1);

  void
  Optimize2(vector<Vector2d> , double &, bool Normalization = true,
      int functionused = 1);

  void
  RobustInit(vector<Vector2d> );

  double
  radius(const double angle);
  inline Vector2d
  Point(double angle)
  {
    double r = radius(angle);
    return Vector2d(r * cos(angle), r * sin(angle));
  }
  ;

  inline double
  Get_a()
  {
    return Parameters[0];
  }
  ;
  inline double
  Get_b()
  {
    return Parameters[1];
  }
  ;

  inline double
  Get_n1()
  {
    return Parameters[2];
  }
  ;
  inline double
  Get_n2()
  {
    return Parameters[3];
  }
  ;
  inline double
  Get_n3()
  {
    return Parameters[4];
  }
  ;

  inline double
  Get_p()
  {
    return Parameters[5];
  }
  ;
  inline double
  Get_q()
  {
    return Parameters[6];
  }
  ;

  inline double
  Get_thtoffset()
  {
    return Parameters[7];
  }
  ;
  inline double
  Get_phioffset()
  {
    return Parameters[8];
  }
  ;

  inline double
  Get_xoffset()
  {
    return Parameters[9];
  }
  ;
  inline double
  Get_yoffset()
  {
    return Parameters[10];
  }
  ;
  inline double
  Get_zoffset()
  {
    return Parameters[11];
  }
  ;

  inline double
  Set_a(const double a)
  {
    Parameters[0] = fabs(a);
  }
  ;
  inline double
  Set_b(const double b)
  {
    Parameters[1] = fabs(b);
  }
  ;

  inline double
  Set_n1(const double n1)
  {
    Parameters[2] = fabs(n1);
  }
  ;
  inline double
  Set_n2(const double n2)
  {
    Parameters[3] = fabs(n2);
  }
  ;
  inline double
  Set_n3(const double n3)
  {
    Parameters[4] = fabs(n3);
  }
  ;

  inline double
  Set_p(const double p)
  {
    Parameters[5] = p;
  }
  ;
  inline double
  Set_q(const double q)
  {
    Parameters[6] = q;
  }
  ;

  inline double
  Set_thtoffset(const double thtoffset)
  {
    Parameters[7] = thtoffset;
  }
  ;
  inline double
  Set_phioffset(const double phioffset)
  {
    Parameters[8] = phioffset;
  }
  ;

  inline double
  Set_xoffset(const double xoffset)
  {
    Parameters[9] = xoffset;
  }
  ;
  inline double
  Set_yoffset(const double yoffset)
  {
    Parameters[10] = yoffset;
  }
  ;
  inline double
  Set_zoffset(const double zoffset)
  {
    Parameters[11] = zoffset;
  }
  ;

  double
  CurveLength(int n = 50);
  void
  Simplify();

  //display functions
  void
  DrawCurve(double R = 0, double G = 0, double B = 0, const int width = 1,
      const int sampling = 50, const bool stippleon = false);
  void
  SpecialDrawCurve(double R, double G, double B, const int width,
      const int sampling);
  void
  DrawIntensity(const Vector2d UpCorner, const Vector2d BottomCorner,
      const int sampling = 100);
  void
  SetBuffer(int, double**);

  double
  MinDist(Vector2d P, int n = 100);

  Vector2d
  ClosestPoint(Vector2d P, int lvbrg_itmax = 1);

  void
  Aff();

  void
  Mutate();
  double
  DrDtheta(double tht);
  vector<Vector2d>
  GenerateData(int n = 100, double x0 = 0, double y0 = 0, double tht0 = 0,
      bool RandAngularValues = true, double noiserate = 0, string filename =
          "data.txt");

  vector<Vector2d>
  GenerateUniformData(int n = 100, double x0 = 0, double y0 = 0, double tht0 =
      0, bool RandAngularValues = true, double noiserate = 0, string filename =
      "data.txt");

  double
  XiSquare(const vector<Vector2d> Data, MatrixXd &alpha, VectorXd &beta,
      bool init_on = false, bool robust = true, int function_used = 1,
      bool Normalization = false, bool udpate = false);

  //export drawing into file
  //        bool ScalarFieldToPPM(const Vector2d UpCorner, const Vector2d BottomCorner, double resolution_x, double resolution_y,std::string filename, bool op, int m);

  vector<Vector2d>
  InitRotationAndSymmetry(vector<Vector2d> , int framesize = 3);
};

inline std::ostream&
operator<<(std::ostream& os, const RationalSuperShape2D& RS2D)
{
  for (int i = 0; i < RS2D.Parameters.size(); i++)
    os << RS2D.Parameters[i] << " ";
  os << endl;
  return os;
}
;

class RationalSuperShape3D
{

public:

  // parameters of the rational supershape 1 and 2
  // Parameters[0] : a
  // Parameters[1] : b

  // Parameters[2] : n1
  // Parameters[3] : n2
  // Parameters[4] : n3

  // Parameters[5] : p
  // Parameters[6] : q

  // Parameters[7] : theta offset
  // Parameters[8] : phi offset // unused in 2D

  // Parameters[9] : x offset
  // Parameters[10] : y offset
  // Parameters[11] : z offset // unused in 2D

  std::vector<double> Parameters1; // parameters for first 2D supershape
  std::vector<double> Parameters2; // parameters for first 2D supershape
  std::vector<double> Transformations;// translations, rotations, scales, others


  //data storage for display
  std::vector<Vector3d> PointList;
  std::vector<Vector3d> NormalList;
  std::vector<Vector3i> FaceList;

  //constructors an destructors
  RationalSuperShape3D();
  RationalSuperShape3D(double a1, double b1, double n11, double n12,
      double n13, double p1, double q1, double thtoffset1, double phioffset1,
      double xoffset1, double y_offset1, double zoffset1, double a2, double b2,
      double n21, double n22, double n23, double p2, double q2,
      double thtoffset2, double phioffset2, double xoffset2, double y_offset2,
      double zoffset2);
  void
  Init(double a1, double b1, double n11, double n12, double n13, double p1,
      double q1, double thtoffset1, double phioffset1, double xoffset1,
      double y_offset1, double zoffset1, double a2, double b2, double n21,
      double n22, double n23, double p2, double q2, double thtoffset2,
      double phioffset2, double xoffset2, double y_offset2, double zoffset2);

  virtual
  ~RationalSuperShape3D();

  inline double
  Get_a1()
  {
    return Parameters1[0];
  }
  ;
  inline double
  Get_a2()
  {
    return Parameters2[0];
  }
  ;

  inline double
  Get_b1()
  {
    return Parameters1[1];
  }
  ;
  inline double
  Get_b2()
  {
    return Parameters2[1];
  }
  ;

  inline double
  Get_n11()
  {
    return Parameters1[2];
  }
  ;
  inline double
  Get_n21()
  {
    return Parameters2[2];
  }
  ;

  inline double
  Get_n12()
  {
    return Parameters1[3];
  }
  ;
  inline double
  Get_n22()
  {
    return Parameters2[3];
  }
  ;

  inline double
  Get_n13()
  {
    return Parameters1[4];
  }
  ;
  inline double
  Get_n23()
  {
    return Parameters2[4];
  }
  ;

  inline double
  Get_p1()
  {
    return Parameters1[5];
  }
  ;
  inline double
  Get_p2()
  {
    return Parameters2[5];
  }
  ;

  inline double
  Get_q1()
  {
    return Parameters1[6];
  }
  ;
  inline double
  Get_q2()
  {
    return Parameters2[6];
  }
  ;

  inline double
  Get_thtoffset1()
  {
    return Parameters1[7];
  }
  ;
  inline double
  Get_thtoffset2()
  {
    return Parameters2[7];
  }
  ;

  inline double
  Get_phioffset1()
  {
    return Parameters1[8];
  }
  ;
  inline double
  Get_phioffset2()
  {
    return Parameters2[8];
  }
  ;

  inline double
  Get_xoffset1()
  {
    return Parameters1[9];
  }
  ;
  inline double
  Get_xoffset2()
  {
    return Parameters2[9];
  }
  ;

  inline double
  Get_yoffset1()
  {
    return Parameters1[10];
  }
  ;
  inline double
  Get_yoffset2()
  {
    return Parameters2[10];
  }
  ;

  inline double
  Get_zoffset1()
  {
    return Parameters1[11];
  }
  ;
  inline double
  Get_zoffset2()
  {
    return Parameters2[11];
  }
  ;

  inline double
  Set_a1(const double a)
  {
    Parameters1[0] = a;
  }
  ;
  inline double
  Set_a2(const double a)
  {
    Parameters2[0] = a;
  }
  ;

  inline double
  Set_b1(const double b)
  {
    Parameters1[1] = b;
  }
  ;
  inline double
  Set_b2(const double b)
  {
    Parameters2[1] = b;
  }
  ;

  inline double
  Set_n11(const double n1)
  {
    Parameters1[2] = n1;
  }
  ;
  inline double
  Set_n21(const double n1)
  {
    Parameters2[2] = n1;
  }
  ;

  inline double
  Set_n12(const double n2)
  {
    Parameters1[3] = n2;
  }
  ;
  inline double
  Set_n22(const double n2)
  {
    Parameters2[3] = n2;
  }
  ;

  inline double
  Set_n13(const double n3)
  {
    Parameters1[4] = n3;
  }
  ;
  inline double
  Set_n23(const double n3)
  {
    Parameters2[4] = n3;
  }
  ;

  inline double
  Set_p1(const double p)
  {
    Parameters1[5] = p;
  }
  ;
  inline double
  Set_p2(const double p)
  {
    Parameters2[5] = p;
  }
  ;

  inline double
  Set_q1(const double q)
  {
    Parameters1[6] = q;
  }
  ;
  inline double
  Set_q2(const double q)
  {
    Parameters2[6] = q;
  }
  ;

  inline double
  Set_thtoffset1(const double thtoffset)
  {
    Parameters1[7] = thtoffset;
  }
  ;
  inline double
  Set_thtoffset2(const double thtoffset)
  {
    Parameters2[7] = thtoffset;
  }
  ;

  inline double
  Set_phioffset1(const double phioffset)
  {
    Parameters1[8] = phioffset;
  }
  ;
  inline double
  Set_phioffset2(const double phioffset)
  {
    Parameters2[8] = phioffset;
  }
  ;

  inline double
  Set_xoffset1(const double xoffset)
  {
    Parameters1[9] = xoffset;
  }
  ;
  inline double
  Set_xoffset2(const double xoffset)
  {
    Parameters2[9] = xoffset;
  }
  ;

  inline double
  Set_yoffset1(const double yoffset)
  {
    Parameters1[10] = yoffset;
  }
  ;
  inline double
  Set_yoffset2(const double yoffset)
  {
    Parameters2[10] = yoffset;
  }
  ;

  inline double
  Set_zoffset1(const double zoffset)
  {
    Parameters1[11] = zoffset;
  }
  ;
  inline double
  Set_zoffset2(const double zoffset)
  {
    Parameters2[11] = zoffset;
  }
  ;

  void
  RandomInit();
  void
  Simplify();
  void
  Mutate();
  double
  GetSurfaceArea()
  {
    return 0;
  }
  ;
  double
  ImplicitFunction(Vector3d);

  Vector2d
  radius(const double theta, const double phi);

  void
  DrawSurface(bool buildsurface = false, bool drawpoints = false,
      bool drawfullfaces = true, bool drawwirefaces = true, bool drawnormals =
          false, double R = 0.9, double G = 0.9, double B = 0.9,
      const int width = 1, const int sampling = 50);

  void
  BuildSurface(const int sampling = 50);

};
void
IsosurfaceViewer(RationalSuperShape2D RSref, Vector3d StarPoint,
    Vector3d EndPoint, double tolerance = 1e-3, int n = 50);
void
CrossOver(RationalSuperShape2D g1, RationalSuperShape2D g2,
    RationalSuperShape2D &f1, RationalSuperShape2D &f2);
void
CrossOver(RationalSuperShape3D g1, RationalSuperShape3D g2,
    RationalSuperShape3D &f1, RationalSuperShape3D &f2);
void
Permute(RationalSuperShape2D &RSS1, RationalSuperShape2D &RSS2);
void
Permute(RationalSuperShape3D &RSS1, RationalSuperShape3D &RSS2);

RationalSuperShape3D
ComputeAverage(vector<RationalSuperShape3D> );
RationalSuperShape3D
ComputeVariance(vector<RationalSuperShape3D> , RationalSuperShape3D AVG);

double
GA(std::vector<Vector2d> Data, int pop,
    std::vector<RationalSuperShape2D> &pool,
    RationalSuperShape2D &OptimalSolution, int functionused);
double
GA(std::vector<Vector3d> Data, int pop,
    std::vector<RationalSuperShape3D> &pool,
    RationalSuperShape3D &OptimalSolution, int functionused);
#endif

