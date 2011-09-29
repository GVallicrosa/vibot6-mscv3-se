//*********************************************************************/
//
//					Scene.h
//
//*********************************************************************/

#ifndef _VIEWER_
#define _VIEWER_

#include <GL/glut.h>
#include <vector>
#include "Eigen/Core"
#include "Eigen/LU"

// import most common Eigen types
using namespace Eigen;

using namespace std;

class scene
{
public:

  scene();
  virtual
  ~scene();

  void
  InitFlags();
  void
  AdjustDimension(const Vector3d Vmin, const Vector3d Vmax);
  void
  Draw_Color_Bar(int size_x, int size_y, int x_init, int y_init);
  void
  Create_Lighting();

  int
  Draw_Axis(double size); //dessine un repere
  int lights_on;
  int oldx, oldy;

  float Current_Matrix[4][4];// = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};

  int rotate, translate; // flag de rotation
  int Axis;
  int Axis_Id_List;

  Vector3d Object_Move;
  Vector3d Vmax, Vmin;

  vector<Vector3d> Palette;
  vector<Vector3d> Color_Steps;

  void
  Create_Palette();

};

void
display(void);
void
reshape(int, int);
void
PrintMessage(int x, int y, const std::string message);
void
Print3DMessage(int x, int y, const std::string message);
void
Mouse(int, int, int, int);
void
Motion(int, int);
void
Keyboard(unsigned char, int, int);
void
Special(int, int, int);
int
verif_extension(char*);
void
menu1(int i);
void
Create_Lighting();
Vector3d
TrackballMapping(int x, int y);
void
Init();

#endif
