//scene.cpp

#include <iostream>
#include "scene.h"

#include "Constante.h"
#include "useful.h"
#include "random-standalone.h"
#include "SuperFormula.h"

#include "Eigen/Geometry"

int n_screen = 0;
extern int arrayshapecursor;
using namespace std;
extern string shortfilename;
extern vector<RationalSuperShape2D> ArrayShape;
extern vector<Vector2d> Data;
extern scene ze_scene;
extern Random R1;

extern int id_toto, id_testshape;
//global variable for OpenGL display lists
extern vector<int> id_curves;
extern int id_data, id_curve, id_all, id_all2;

extern int id_imagelist;

int window_width = 640;
int window_height = 480;
int window_number = 0;
double view_angle = 45;
Vector3d model_position(0, 0, 0);
Vector3d model_scale(1, 1, 1);
Vector3d translations(0, 0, 0);
Vector3d rotations(0, 0, 0);

//Vector3d				Camera_Position(00,0,800);//7.5,9,8);
//Vector3d				//Camera_Position(6,8,10);
//Camera_Position

//(0,-6,6); //for viewing not normalized functions
//(0,-5,4); //for viewing normalized functions
//(0,0,10);
//(0,0,5);

extern Vector3d Camera_Target, Camera_Position;//(00,00,1);//-1,-1,4);
Vector2i previous_mouse_position(0, 0);
Vector3d previous_trackball_position(0, 0, 0);
Vector3d rotation_axis(0, 0, 0);
double rotation_angle(0);
float trackball_transform[4][4] =
  {
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 } };

extern vector<vector<double> > Evolution;
extern int cursor;

#define ACSIZE	8

void
scene::Create_Lighting()
{

  /*	GLfloat ambient[]={0.0, 0.0, 0.0, 1.0};
   GLfloat diffuse[]={1.0, 1.0, 1.0, 1.0};
   GLfloat specular[]={1.0, 1.0, 1.0, 1.0};
   GLfloat position[]={0 , 50 , 100};
   ;*/

  glEnable(GL_COLOR_MATERIAL);
  GLfloat specular[] =
    { 1.0, 1.0, 1.0, 1.0 };
  GLfloat shine[] =
    { 1 };
  glMaterialfv(GL_FRONT, GL_SHININESS, shine);
  glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

  /*
   glLightfv(GL_LIGHT0,GL_AMBIENT, ambient);
   glLightfv(GL_LIGHT0,GL_DIFFUSE, diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR, specular);
   glLightfv(GL_LIGHT0,GL_POSITION, position);
   */
  //	glEnable(GL_LIGHT0);


  GLfloat ambient2[] =
    { 0.0, 0.0, 0.0, 1.0 };
  GLfloat diffuse2[] =
    { 0.4, 0.4, 0.4, 1.0 };

  GLfloat specular2[] =
    { 1.0, 1.0, 1.0, 1.0 };
  GLfloat position2[] =
    { 0.0, 0.0, 100, 1.0 };
  GLfloat spot_direction2[] =
    { 0, 0.0, -1 };

  glLightfv(GL_LIGHT0, GL_AMBIENT, ambient2);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse2);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular2);
  glLightfv(GL_LIGHT0, GL_POSITION, position2);
  //glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 360.0);
  //glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction2);
  //glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);


  /*
   GLfloat specular3[]={0.0, 1.0, 0.0, 1.0};
   GLfloat position3[]={100.0, -50.0, 100.0, 1.0};
   GLfloat spot_direction3[] = {-1, 0.0, -1.0};

   glLightfv(GL_LIGHT1, GL_AMBIENT, ambient2);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse2);
   glLightfv(GL_LIGHT1, GL_SPECULAR, specular3);
   glLightfv(GL_LIGHT1, GL_POSITION, position3);
   glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction3);
   glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);

   GLfloat specular4[]={0.0, 0.0, 1.0, 1.0};
   GLfloat position4[]={0.0, 50.0, 100.0, 1.0};
   GLfloat spot_direction4[] = {0.0, -.5, -1.0};

   glLightfv(GL_LIGHT2, GL_AMBIENT, ambient2);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuse2);
   glLightfv(GL_LIGHT2, GL_SPECULAR, specular4);
   glLightfv(GL_LIGHT2, GL_POSITION, position4);
   glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 45.0);
   glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, spot_direction4);
   glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 2.0);
   */
  glEnable(GL_LIGHT0);
  //	glEnable(GL_LIGHT1);
  //	glEnable(GL_LIGHT2);

  //	glEnable(GL_LIGHTING);
}

void
scene::InitFlags()
{

  //	for(int i=0; i<ListID.size();i++) ListFlag.push_back(true);
}

int
scene::Draw_Axis(double size)
{

  int id_list = glGenLists(1);
  glNewList(id_list, GL_COMPILE_AND_EXECUTE);

  glMatrixMode(GL_MODELVIEW);

  glPushMatrix();

  GLfloat ratio = size / 8.0;

  glColor3f(1.0, 0.0, 0.0);

  glLineWidth(2.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(size, 0.0, 0.0);
  glEnd();
  glLineWidth(1.0);

  glPushMatrix();
  glTranslatef(size, 0.0, 0.0);
  glPushMatrix();
  glRotatef(90.0, 0.0, 1.0f, 0.0f);
  glutSolidCone(1 * ratio, 3 * ratio, 10, 10);
  glPopMatrix();
  glTranslatef(size / 1.5, -1, 0.0);

  //Print3DMessage(0,0,"x");

  glPopMatrix();

  glColor3f(0.0, 1.0, 0.0);

  glLineWidth(2.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, size, 0.0);
  glEnd();
  glLineWidth(1.0);

  glPushMatrix();
  glTranslatef(0.0, size, 0.0);
  glPushMatrix();
  glRotatef(90.0, -1.0, 0.0f, 0.0f);
  glutSolidCone(1 * ratio, 3 * ratio, 10, 10);
  glPopMatrix();
  glTranslatef(-1, size / 1.5, 0.0);

  //Print3DMessage(0,0,"y");
  glPopMatrix();

  glColor3f(0.0f, 0.0f, 1.0f);

  glLineWidth(2.0);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, 0.0);
  glVertex3f(0.0, 0.0, size);
  glEnd();
  glLineWidth(1.0);

  glPushMatrix();
  glTranslatef(0.0, 0.0, size);
  glPushMatrix();
  glRotatef(90.0, 0.0, 0.0f, 1.0f);
  glutSolidCone(1 * ratio, 3 * ratio, 10, 10);
  glPopMatrix();
  glTranslatef(-1, -1, size / 1.5);
  //	Print3DMessage(0,0,"z");
  glPopMatrix();
  glPopMatrix();

  glEndList();
  return id_list;
}

scene::scene()
{

  //	PolySmooth=false;
  rotate = 0;
  Axis = 0;
  //Local_Light=1;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      Current_Matrix[i][j] = 0;

  Current_Matrix[0][0] = Current_Matrix[1][1] = Current_Matrix[2][2]
      = Current_Matrix[3][3] = 1;

  Vector3d COL_BLUE(0, 0, 1);
  Vector3d COL_CYAN(0, 1, 1);
  Vector3d COL_GREEN(0, 1, 0);
  Vector3d COL_YELLOW(1, 1, 0);
  Vector3d COL_RED(1, 0, 0);

  Color_Steps.push_back(COL_BLUE);
  Color_Steps.push_back(COL_CYAN);
  Color_Steps.push_back(COL_GREEN);
  Color_Steps.push_back(COL_YELLOW);
  Color_Steps.push_back(COL_RED);
}

scene::~scene()
{

}

void
Display_Menu(int i)
{

}

void
scene::Create_Palette()
{

  //Palette.push_back(Vector3d(0,128,0));
  //Palette.push_back(Vector3d(0,128,0));
  //Palette.push_back(Vector3d(0,128,0));
  //Palette.push_back(Vector3d(0,128,0));

  //Palette.push_back(Vector3d(128,128,255));
  //Palette.push_back(Vector3d(128,128,255));
  //Palette.push_back(Vector3d(128,128,255));
  //Palette.push_back(Vector3d(128,128,255));
  //Palette.push_back(Vector3d(128,128,255));

  Palette.push_back(Vector3d(255, 128, 128));
  Palette.push_back(Vector3d(128, 0, 64));
  Palette.push_back(Vector3d(255, 128, 0));
  Palette.push_back(Vector3d(0, 64, 128));
  Palette.push_back(Vector3d(0, 0, 255));
  Palette.push_back(Vector3d(128, 255, 128));
  Palette.push_back(Vector3d(0, 128, 255));
  Palette.push_back(Vector3d(0, 64, 128));
  Palette.push_back(Vector3d(255, 0, 255));
  Palette.push_back(Vector3d(128, 255, 0));
  Palette.push_back(Vector3d(255, 128, 64));
  Palette.push_back(Vector3d(0, 128, 128));
  Palette.push_back(Vector3d(128, 128, 0));
  Palette.push_back(Vector3d(64, 0, 64));
  Palette.push_back(Vector3d(128, 0, 0));
  Palette.push_back(Vector3d(0, 128, 0));
  Palette.push_back(Vector3d(255, 255, 0));
  Palette.push_back(Vector3d(0, 255, 64));
  Palette.push_back(Vector3d(128, 128, 192));
  Palette.push_back(Vector3d(128, 64, 64));

  for (int i = 0; i < Palette.size(); i++)
    Palette[i] /= 255.0;

}

void
scene::AdjustDimension(const Vector3d Vmin, const Vector3d Vmax)
{

  Object_Move = (Vmax - Vmin) / 50.0;
  for (int i = 0; i < 3; i++)
    if (Object_Move[i] == 0)
      Object_Move[i] = 0.02;

  cout << "Object_Move = " << Object_Move[0] << " " << Object_Move[1] << " "
      << Object_Move[2] << " " << endl;
}

void
scene::Draw_Color_Bar(int size_x, int size_y, int x_init, int y_init)
{

  // store draw mode (light and polygon mode)


  GLint polygon_draw_mode[2];
  glGetIntegerv(GL_POLYGON_MODE, polygon_draw_mode);

  std::vector<Vector3d>::iterator it(Color_Steps.begin());
  Vector3d current, prev;

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glDisable(GL_LIGHTING);
  glPolygonMode(GL_FRONT, GL_FILL);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, viewport[2], 0, viewport[3]);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glTranslatef(x_init, y_init, 0);
  double coef_x(size_x), coef_y(size_y / double(Color_Steps.size() - 1));

  for (int i = 0; i < Color_Steps.size() - 1; i++)
    {
      prev = *it;
      it++;
      current = *it;

      glBegin(GL_QUADS);

      glNormal3d(0, 0, 1.0);
      glColor3d(prev[0], prev[1], prev[2]);
      glVertex3d(size_x, (i) * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(current[0], current[1], current[2]);
      glVertex3d(size_x, (i + 1) * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(current[0], current[1], current[2]);
      glVertex3d(0, (i + 1) * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(prev[0], prev[1], prev[2]);
      glVertex3d(0, (i) * coef_y, 0);

      glEnd();

      double t0 = (double) (i) / ((double) Color_Steps.size() - 1);
      double t1 = (double) (i + 1) / ((double) Color_Steps.size() - 1);

      glBegin(GL_QUADS);

      glNormal3d(0, 0, 1.0);
      glColor3d(t0, t0, t0);
      glVertex3d(2 * size_x + 10, i * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(t1, t1, t1);
      glVertex3d(2 * size_x + 10, (i + 1) * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(t1, t1, t1);
      glVertex3d(size_x + 10, (i + 1) * coef_y, 0);

      glNormal3d(0, 0, 1.0);
      glColor3d(t0, t0, t0);
      glVertex3d(size_x + 10, i * coef_y, 0);

      glEnd();
    }

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);

  //old polygon drawing mode restoration
  glPolygonMode(GL_FRONT, polygon_draw_mode[0] | polygon_draw_mode[1]);
}

void
Print3DMessage(int x, int y, const std::string message)
{

  glDisable(GL_LIGHTING);

  glMatrixMode(GL_MODELVIEW);
  glRasterPos2f(x, y);

  for (int i = 0; i < (int) message.size(); i++)
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, message[i]);

}

void
PrintMessage(int x, int y, const std::string message)
{

  GLboolean lights_on[1];
  glGetBooleanv(GL_LIGHTING, lights_on);

  glDisable(GL_LIGHTING);
  glPushMatrix();

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, window_width, 0, window_height);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glColor3f(0.0, 0.0, 0.0);
  glRasterPos2f(x, y);
  for (int i = 0; i < (int) message.size(); i++)
    {
      glutBitmapCharacter(GLUT_BITMAP_9_BY_15, message[i]);
    }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  //glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  //glMatrixMode(GL_MODELVIEW);

  if (lights_on)
    glEnable(GL_LIGHTING);
}

void
display()
{

  //cout <<"cursor="<<cursor<<endl;
  //cout <<"Evol size"<< Evolution.size()<<endl;

  GLboolean lights_on[1];
  glGetBooleanv(GL_LIGHTING, lights_on);

  GLint polygon_draw_mode[2];
  glGetIntegerv(GL_POLYGON_MODE, polygon_draw_mode);

  GLint viewport[4];
  glGetIntegerv(GL_VIEWPORT, viewport);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  ///////
  //
  //	Windows informations display (color bar, referential,...)
  //
  //////

  glDisable(GL_LIGHTING);

  if (lights_on)
    //		glEnable(GL_LIGHTING);

    glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Trackball Transformation
  glRotatef(rotation_angle, rotation_axis[0], rotation_axis[1],
      rotation_axis[2]);
  glMultMatrixf((GLfloat *) trackball_transform);
  glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *) trackball_transform);

  //Color Bar Draw
  //ze_scene.Draw_Color_Bar(30,300,30,170);


  /*
   //translation in windows coordinates
   glDisable(GL_LIGHTING);
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   glLoadIdentity();

   //local 3d projection to draw 3D referential(size=30 pixels)
   glOrtho(0,viewport[2],0,viewport[3],0,std::max(viewport[2],viewport[3]));
   //to avoid clip planes
   glTranslatef(50,90,-50);

   glPushMatrix();

   glMatrixMode(GL_MODELVIEW);


   //glCallList(ze_scene.Axis_Id_List);
   glPushMatrix();
   glLoadIdentity();
   glTranslatef(viewport[2]- 225,75,0);
   glScalef(50,50,50);
   //glCallList(id_data);

   glPopMatrix();

   glMatrixMode(GL_PROJECTION);

   glPopMatrix();

   glPopMatrix();
   */
  ///////
  //
  //	Surface display
  //
  //////


  //if(ze_scene.lights_on) glEnable(GL_LIGHTING);
  //	glPolygonMode(GL_FRONT,polygon_draw_mode[0]|polygon_draw_mode[1] );

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  //Vector3d center = -(toto.VMax + toto.VMin) *0.5;
  //Vector3d center = -(Surface.VMax + Surface.VMin) *0.5;
  Vector3d center(0, 0, 0);

  glTranslatef(translations[0], translations[1], translations[2]);
  glMultMatrixf((GLfloat *) trackball_transform);

  glEnable(GL_TEXTURE_2D);
  glCallList(id_imagelist);
  glDisable(GL_TEXTURE_2D);

  extern vector<int> Results;

  //glCallList(Results[cursor]);
  //glCallList(id_data);

  glDisable(GL_LIGHTING);
  ArrayShape[arrayshapecursor].SpecialDrawCurve(1, 0, 0, // red green blue coefs in [0,...,1]
      3, // curve width in pixels
      1000); // sampling rate to render the curve


  rotation_axis = Vector3d(0, 0, 0);
  rotation_angle = 0;

  glutSwapBuffers();

}

void
reshape(int w, int h)
{

  extern scene ze_scene;
  glPushMatrix();
  glViewport(0, 0, w, h);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  //	glOrtho(
  //		ze_scene.Vmin[0],ze_scene.Vmax[0],
  //		ze_scene.Vmin[1],ze_scene.Vmax[1],
  //		ze_scene.Vmin[2],ze_scene.Vmax[2]
  //		);


  gluPerspective(
  //45
      60, //view_angle
      (float) w / h, 1, 1000
  //1000.0
  );

  //static Vector3d seen_point = (toto.VMax + toto.VMin)/ 2.0;
  //static Vector3d seen_point = (Surface.VMax + Surface.VMin)/ 2.0;
  //static Vector3d seen_point = (Curve.VMax + Curve.VMin)/ 2.0;

  gluLookAt(

  //temporaire pour SPLINE

      //5,3,13,
      //0,0,10,


      Camera_Position[0], Camera_Position[1], Camera_Position[2],
      //0,0,20,

      0, 0, 0,//Camera_Target[0], Camera_Target[1], Camera_Target[2],
      //0,0,0,
      //0,0,2*toto.VMax.Max(),
      //seen_point[0],seen_point[1],seen_point[2],		//seen point

      0, 1, 0); //Up Vector

  glMatrixMode(GL_MODELVIEW);

  glutPostRedisplay();

}

void
Motion(int x, int y)
{

  static Vector3d current_position;

  if (ze_scene.rotate)
    {

      current_position = TrackballMapping(x, y); // Map the mouse position to a logical

      //
      // Rotate about the axis that is perpendicular to the great circle connecting the mouse movements.
      //

      rotation_axis = previous_trackball_position.cross(current_position);
      rotation_angle = 90.0
          * (current_position - previous_trackball_position).norm() * 1.5;
      previous_trackball_position = current_position;
    }

  if (ze_scene.translate)
    {

      if ((y - ze_scene.oldy) > 0)
        translations[2] += ze_scene.Object_Move[2];
      else
        translations[2] -= ze_scene.Object_Move[2];

      ze_scene.oldx = x;
      ze_scene.oldy = y;
    }

  glutPostRedisplay();
}

void
Mouse(int button, int state, int x, int y)
{

  ze_scene.rotate = 0;
  if (button == GLUT_LEFT_BUTTON)
    {
      if (state == GLUT_DOWN)
        {
          ze_scene.rotate = 1;
          previous_trackball_position = TrackballMapping(x, y);
        }
      else
        ze_scene.rotate = 0;
    }
  else if (button == GLUT_RIGHT_BUTTON)
    {
      if (state == GLUT_DOWN)
        {
          //glutAttachMenu(button);
          ze_scene.translate = 1;
          ze_scene.oldx = x;
          ze_scene.oldy = y;
        }
      else
        {
          ze_scene.translate = 0;
          ze_scene.oldx = x;
          ze_scene.oldy = y;
        }
    }
}

void
Special(int key, int x, int y)
{

  glMatrixMode(GL_PROJECTION);

  switch (key)
    {
  case 100:
    translations[0] -= ze_scene.Object_Move[0];

    break;
  case 101:
    translations[1] += ze_scene.Object_Move[0];

    break;
  case 102:
    translations[0] += ze_scene.Object_Move[1];

    break;
  case 103:
    translations[1] -= ze_scene.Object_Move[1];

    break;
    }

  glutPostRedisplay();
  glMatrixMode(GL_MODELVIEW);
}

void
Keyboard(unsigned char key, int x, int y)
{

  double gap = 10.0;

  switch (key)
    {

  case 'f':
    {
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    break;

  case 'l':
    {
      glLineWidth(5.0);
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    break;

  case 'p':
    {
      char s[100];
      sprintf(s, "Screen.tga");
      string f2(s);
      ScreenShot(f2);
    }
    break;

  case 'i':
    {
      if (ze_scene.lights_on == 0)
        ze_scene.lights_on = 1;
      else
        ze_scene.lights_on = 0;
    }
    break;

  case '+':
    {
      if (arrayshapecursor < 3)
        arrayshapecursor++;
    }
    break;
  case '-':
    {
      if (arrayshapecursor > 0)
        arrayshapecursor--;
    }
    break;
  case 'd':
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    break;
    //case 's' : glPolygonMode(GL_FRONT,GL_FILL);break;
    //case 't' : {cursor++; cursor = min( (unsigned int)cursor, Evolution.size()-2);} break;
    //case 'g' : {cursor--; cursor = max(cursor, 0);} break;

  case 's':
    {//format and export opengl buffer into .tga image
      char s[100];
      sprintf(s, "Contours\\Results\\FittingResult_%s(1)_%dx.tga",
          shortfilename.c_str(), arrayshapecursor);

      string f2(s);
      ScreenShot(f2);
    }
    break;

    }

  glutPostRedisplay();
}

Vector3d
TrackballMapping(int x, int y)
{

  static const double PI_2 = 3.141592653589793238462643383 / 2.0;

  Vector3d v(0, 0, 0);
  static GLint params[4];

  glGetIntegerv(GL_VIEWPORT, params);

  v[0] = (2.0 * x - params[2]) / params[2];
  v[1] = (params[3] - 2.0 * y) / params[3];

  double d = v.norm();
  if (d > 1.0)
    d = 1.0;

  v[2] = cos((PI_2) * d);

  v.normalize();
  return v;
}

void
Init()
{

  //glPolygonMode(GL_FRONT,GL_FILL);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(1.0, 1.0);

  glEnable(GL_NORMALIZE);
  glEnable(GL_COLOR_MATERIAL);

  glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  glShadeModel(GL_FLAT);

}
