# Introduction #
We would like to demonstrate the overview of openGL, but graphics is a big subject and openGL is designed to handle both 3D and 2D tasks, then there will be some additional complexity in 2D task e.g. 3D transformation matrix, matrix stacks, etc. If we are manipulating only 2D image, we suggest to use other library like OpenCV to handle the task. We would like to keep the wiki as short and clear as possible. If you are interested in deep detail, please kindly, in addition, see the resource we provide in "Reference" section.

In this page we try to cover OpenGL technical issues. [Open Graphics Library (OpenGL)](http://en.wikipedia.org/wiki/OpenGL) is a standard specification defining a cross-language, cross-platform API for writing applications that produce 2D and 3D computer graphics. In short, OpenGL is a standard (not a library) to make a same code to be able to use use in different machine, different language, different platform, etc.

There are many libraries that agree with this standard e.g. GLUT, GLEW. One library has its own advantage to the other, but the most basic one (which should be enough) is GLUT (OpenGL Utility Toolkit).

# What can GLUT do? #
GLUT provides most of APIs to present the information (image and 3D model) through screen, some connections with window, lighting, interface to render engine, interface to write shader, etc.
  * create window with mouse and keyboard handle to display something
  * place camera
  * drawing something (triangle, cube, sphere)
  * matrix operation (for transform object, camera projection, etc)
  * create light source
  * load and apply material
  * interface with user shader
  * render image
  * much more! :D

N.B. GLUI (OpenGL User Interface Library) which based on GLUT can provide you common controls such as buttons, checkboxes, radio buttons, and spinners to OpenGL applications. It is window- and operating system independent, relying on GLUT to handle all system-dependent issues, such as window and mouse management. However, GLUI is depricated to create GUI. Other library/framework like WxWidget, Qt, GTK are recommended.

<a href='Hidden comment: 
= Details =
Coming soon. :)

== Installation ==
In this section we cover how to install OpenGL in your machine step by step.


=== Code Blocks ===
Since it seems must of the people have been manipulated by using the Code Blocks, I tried first to install glut for that IDE.

# Download glut-3.7.6-bin from this link.
# Copy glut32.dll to c:\windows\system,
# Copy glut32.lib to C:\Program Files (x86)\Code Blocks\MinGW\lib (The path might be different on your machine, but very similar to this one, depending on the Windows you"re using.)
# Copy glut.h to c:\Program Files (x86)\Code Blocks\MinGW\include\GL.

It"s not yet fully done, more things need to be fixed in the code, will write it in the morning.

=== Eigen ==
In order to use Eigen, you just need to download and extract Eigen"s source code (see the wiki for download instructions). In fact, the header files in the Eigen subdirectory are the only files required to compile programs using Eigen. The header files are the same for all platforms. It is not necessary to use CMake or install anything.

*This is not related to OpenGL, but necessary to compile the code we got for the project, I"m going to migrate it tomorrow to somewhere more approperiate.*
'></a>

# Some famous functions #
  * Must know
    * **glutInit(&argc, argv);** - initialize the GLUT library, e.g. talk to OS, check for hardware compatibility, etc. If in doubt, throw &argc, argv in. :)
    * **glutInitDisplayMode(GLUT\_DOUBLE | GLUT\_RGBA | GLUT\_DEPTH);** - configure display mode, e.g. color, double buffer, depth buffer, other buffer. If in doubt, use GLUT\_DOUBLE | GLUT\_RGB | GLUT\_DEPTH. :)
    * **glutCreateWindow("YF\_Viewer 1.1");** - create the window with that name
    * **glEnable(GL\_DEPTH\_TEST);** - turn on/off some options. There are too many options to mention. Most of time you need GL\_DEPTH\_TEST, so GL ckecks for occlusion of objects in scene.
    * **glutDisplayFunc(drawScene);** - bind the callback function name 'drawScene'. When the window is drawn, the function is called. Please see "How to draw?".
    * **glutReshapeFunc(reshape);** - bind the callback function name 'reshape'. When the window is resized, the function is called.
    * **glutMainLoop();** - enter the main loop which never returns until the termination of application. The application will wait for event from user and process the event with the binded callback function.
  * Good to know
    * **glutInitWindowSize(800,800);** - set the window size in pixel.
    * **glutInitWindowPosition(00,00);** - set window position in pixel.
    * **glutMotionFunc(motion);** - bind the callback function name 'motion'. When the mouse is moved within the window while some mouse buttons are pressed, the function is called.
    * **glutPassiveMotionFunc(passiveMotion);** - bind the callback function name 'passiveMotion'. When the mouse is moved within the window while no mouse buttons are pressed, the function is called.
    * **glutMouseFunc(mouse);** - bind the callback function name 'mouse'. When the mouse is clicked, the function is called.
    * **glutKeyboardFunc(keyboard);** - bind the callback function name 'keyboard'. When the key is pressed, the function is called. This function detect only 'char' keys.
    * **glViewport(0, 0, w, h)** - specify the lower left corner of the viewport rectangle and resolution of the viewport. This viewport is used to convert from 3D coordinate to pixel coordinate. If in doubt put "glViewport(0, 0, w, h);" where w, h are the width and hight of the window.
    * **gluPerspective(angle, (float)w/h, 1.0, 200.0)** - set a perspective projection matrix (camera perspective). The arguments are angle in degree for angle of view, aspect ratio, "near plane" distance, "far plane" distance. Please see "How to draw?" for more information about "near plane" and "far plane".
    * **glutPostRedisplay()** - mark the current window to be repainted.
  * Rare, but still they are in the project...
    * **glutSpecialFunc(special);** - bind the callback function name 'special'. When the key is pressed, the function is called. This function detect any keys  like F1, F2, etc.
    * **gluOrtho2D(left, right, top, bottom)** - set a orthogonal projection matrix. The arguments are left and right vertical clipping plane. Top and bottom horizontal clipping plane. Normally, in graphics and 3D, we work with perspective projection (gluPerspective(...)).
    * **glutBitmapCharacter(GLUT\_BITMAP\_9\_BY\_15, char)** - (desperately) render a bitmap 'char' character using OpenGL where, in this case, a character will fit in 9\*15 pixel box.


These functions should be call in order; glutInit(...) > glutInitDisplayMode(...) > init window (if neccessary) > glutCreateWindow(...) > set callback functions (glutDisplayFunc(...) and glutReshapeFunc(...) must be specified. The rest is not mandatory).

Every callback function has it own pre-defined argument list. For example, with glutKeyboardFunc(keyboard);, keyboard function has to be something like "void keyboard(unsigned char key, int x, int y);". Please see the references.

# How to draw? #

To draw something, you must specify display callback function of a window by calling "glutDisplayFunc(displayFunctionName);", and the function should look like "void displayFunctionName(){...}". We will draw (code) our object in this function.

## Axes and Frustum ##
First of all, we need to talk about axes before going further. We work on Euclidean space. +X axis points to your right. (yes, that way). +Y points up and +Z comes out of the screen.

In graphics, "frustum" is a word to call the 3D space that can be seen by camera. However, the spaces that are very near and very far from the camera are clipped by "near plane" and "far plane", because if some object is very close to the camera it will occlude everything in the scene and we do not need to waste time rendering the very far object. So the 3D space is clipped into a square frustum volume.

## Transformation matrix ##
Transformation matrix in is a 4\*4 matrix to transform the vertices/objects. "Transform" here means rotating, translating, scaling all contain in one matrix. It is use as an operation to move, rotate, scale any vertices/objects. If you are questioning why 4\*4, please see "Homogeneous coordinates".

## Stack ##
OpenGL operations are based on stacks of matrix. There are many stacks e.g. projection stack, model stack, texture stack, etc. User have to specify which one that we are going to use. In this case, we are going to draw some objects in the scene, then we have to use a stack called GL\_MODELVIEW by calling "glMatrixMode(GL\_MODELVIEW);".

Then, you, dear readers, are going to doubt why stack. Because in most case of graphic thingies, relative value is used e.g. relative position from other object, orientation of parent object, etc. Relative value is easier to manipulate by stack.

## Some basic functions for drawing ##
  * **glMatrixMode(GL\_MODELVIEW);** - call this to tell openGL that we are going to work with modelview stack (object/model in the scene).
  * **glClear(GL\_COLOR\_BUFFER\_BIT);** - to clear buffer. Some other buffer can be clear, but, for GL\_COLOR\_BUFFER\_BIT, every pixels are paint with background color which can be specified with "glClearColor(...);"
  * **glLoadIdentity()** - replace the matrix on the top of stack with identity matrix. In short, use this to clear any transformation made before.
  * **glBegin(GL\_TRIANGLES)** - tell openGL that we are going to add some vertices soon (by calling "glVertex3f(...)" and, with GL\_TRIANGLES, every 3 calls of "glVertex3f(...);", they found a triangle. There are many other arguments that you can pass to draw lines, rectangle, etc.
  * **glVertex3f(1.f, 2.f, 3.f)** - transform a vertex {x, y, z} = {1.f, 2.f, 3.f} with the transformation matrix on the top of stack and add it in the scene.
  * **glEnd();** - tell openGL that we have finished adding vertices. Every "glBegin(...);" must have its corresponding "glEnd();".
  * **glutSwapBuffers();** - In case of double buffer is used, swap the working buffer (back buffer) with the buffer of the current window.
  * **glPushMatrix()** - copy the matrix on the top of stack and place it on the stack (after the call, two top matrices are identical).
  * **glPopMatrix();** - remove the matrix on the top of stack.
  * **glMultMatrixf(m)** - multiply the matrix on the top of stack with matrix 4\*4 m.
  * **glRotatef(angle, 0.0, 0.0, 1.0);** - add rotation information to transformation matrix on the top of stack (multiply it with rotation matrix). In this case, it is rotation around z axis by 'angle' degree.
  * **glTranslatef(0.0, 5.0, 0.0);** - add translation information to transformation matrix on the top of stack (multiply it with translation matrix). In this case, it is the translation in y axis for 5.0 unit.
  * **gluLookAt(eyeX, eyeY, eyeZ, centerX, cecterY, centerZ, upX, upY, upZ)** - it is like moving a camera (eye) to {eyeX, eyeY, eyeZ} and look at the specific point (center) by specifying which direction is up direction (normally +y is up). However, it happens that openGL has no interface to actually move camera. In fact, objects are move to the static camera instead. It is relativity! So, this function multiply the transformation matrix on top of stack with the transformation matrix to move an object in front of camera. Please note that camera stay at {0.0, 0.0, 0.0} all the time and cannot be moved.
  * **glColor3f(1.0, 0.0, 0.0);** - specify the current color. When glVertex3d() is called the new vertex has this color which is totally red in this case.
  * **glNormal3d(0.0, 1.0, 0.0);** - specify the current normal. When glVertex3d() is called the new vertex has this normal which is pointing to +Y in this case.

## Some advance commands for drawing (which exist in the project) ##
  * glPolygonOffset - coming soon
  * Display list
    * **glGenLists(num)** - create a contiguous empty "display lists" with 'num' elements. This function return 'id' for glNewList(...). "Display list" is a list of openGL command like glVertex3f(x, y, z), glTexCoord2f (x, y), etc. Why display list? We can store the long commands to draw some complex object and use it to draw the same thing many times.
    * **glNewList(id, GL\_COMPILE\_AND\_EXECUTE)** - tell openGL to start adding the next openGL command into the display list. The second argument is for specifying if the command should be added to the list with or without execution e.g. GL\_COMPILE\_AND\_EXECUTE add the command to the list and also execute the command.
    * **glEndList()** - stop adding the commands into the display list. This function must be coresponding with glNewList(...).
    * **glCallList(id)** - execute the display list specified by 'id'.
  * Material & lighting
    * glBindTexture - coming soon
    * glMaterialf - coming soon
    * glLightfv - coming soon
    * **glShadeModel(GL\_FLAT)** - select flat or smooth shading.
  * Rasterisation
    * **glPolygonMode(GL\_FRONT, GL\_FILL);** - select a polygon rasterization mode. In this case, we specify those polygons with normals point toward the camera (front facing polygon) to have their interior filled.
    * glRasterPos2f - coming soon
## Example ##
Here is an example of display callback function to draw 2 diagonal lines. The green one is rotated clockwise by rotAngle degree and the blue one is rotated counter clock wise by rotAngle degree. Please note that if there is no rotation at all both lines will be prependicular to each other and form a x's.

```
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);

   glColor3f (0.0, 1.0, 0.0);
   glPushMatrix();
   glRotatef(-rotAngle, 0.0, 0.0, 0.1);
   glBegin (GL_LINES);
      glVertex2f (-0.5, 0.5);
      glVertex2f (0.5, -0.5);
   glEnd ();
   glPopMatrix();

   glColor3f (0.0, 0.0, 1.0);
   glPushMatrix();
   glRotatef(rotAngle, 0.0, 0.0, 0.1);
   glBegin (GL_LINES);
      glVertex2f (0.5, 0.5);
      glVertex2f (-0.5, -0.5);
   glEnd ();
   glPopMatrix();

   glFlush();

}
```

Another example:
```
/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*
 *  aargb.c
 *  This program draws shows how to draw anti-aliased lines. It draws
 *  two diagonal lines to form an X; when 'r' is typed in the window, 
 *  the lines are rotated in opposite directions.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>

static float rotAngle = 0.;

/*  Initialize antialiasing for RGBA mode, including alpha
 *  blending, hint, and line width.  Print out implementation
 *  specific info on line width granularity and width.
 */
void
init(void)
{
  GLfloat values[2];
  glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
  printf("GL_LINE_WIDTH_GRANULARITY value is %3.1f\n", values[0]);

  glGetFloatv(GL_LINE_WIDTH_RANGE, values);
  printf("GL_LINE_WIDTH_RANGE values are %3.1f %3.1f\n", values[0], values[1]);

  glEnable( GL_LINE_SMOOTH);
  glEnable( GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);

  // The thickness of line. The higher the number the thicker the line is.
  glLineWidth(1.5);

  // Settign the background color (the colour that is used when buffer is cleared.)
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

/* Draw 2 diagonal lines to form an X
 */
void
display(void)
{
  glClear( GL_COLOR_BUFFER_BIT);

  // glColor3 variants specify new red, green, and blue values explicitly and 
  // set the current alpha value to 1.0 (full intensity) implicitly
  glColor3f(0.0, 1.0, 0.0);
  // Push the matrix, modify it, draw stuff, then pop the matrix back off again.
  // That way any code that comes after it isn't affected by the matrix changes that you made.
  glPushMatrix();
  // multiply the current matrix by a rotation matrix.
  glRotatef(-rotAngle, 0.0, 0.0, 0.1);

  // glBegin and glEnd delimit the vertices that define a primitive or a group of like primitives.
  // GL_LINES: Treats each pair of vertices as an independent line segment.
  glBegin( GL_LINES);
    {
      // Specifies two vertices.
      glVertex2f(-0.5, 0.5);
      glVertex2f(0.5, -0.5);
    }
  glEnd();
  glPopMatrix();

  glColor3f(0.0, 0.0, 1.0);
  glPushMatrix();
  glRotatef(rotAngle, 0.0, 0.0, 0.1);
  glBegin(GL_LINES);
    {
      glVertex2f(0.5, 0.5);
      glVertex2f(-0.5, -0.5);
    }
  glEnd();
  glPopMatrix();

  // Different OpenGL implementations buffer commands in several different locations, 
  // including network buffers and the graphics accelerator itself.
  // The glFlush function empties all these buffers,
  glFlush();
}

void
reshape(int w, int h)
{
  // Transforming device coordinates to window coordinates
  glViewport(0, 0, w, h);
  // Specifies which matrix stack is the target for subsequent matrix operations.
  // Three values are accepted: GL_MODELVIEW, GL_PROJECTION, and GL_TEXTURE.
  // The initial value is GL_MODELVIEW.
  glMatrixMode( GL_PROJECTION);
  // Replace the current matrix with the identity matrix.
  glLoadIdentity();
  if (w <= h)
    {
      // gluOrtho2D sets up a two-dimensional orthographic viewing region.
      gluOrtho2D(-1.0, 1.0, -1.0 * (GLfloat) h / (GLfloat) w, 1.0 * (GLfloat) h
          / (GLfloat) w);
    }
  else
    {
      gluOrtho2D(-1.0 * (GLfloat) w / (GLfloat) h, 1.0 * (GLfloat) w
          / (GLfloat) h, -1.0, 1.0);
    }
  glMatrixMode( GL_MODELVIEW);
  glLoadIdentity();
}

void
keyboard(unsigned char key, int x, int y)
{
  switch (key)
    {
  // If r lowercase or upper case is pressed, add 20 to the rotation agnle.
  case 'r':
  case 'R':
    rotAngle += 20.;
    // If rotation angle has reached 360, reset to 0.
    if (rotAngle >= 360.)
      {
        rotAngle = 0.;
      }
    // Mark the normal plane of current window as needing to be redisplayed.
    // The next iteration through glutMainLoop, the window's display callback
    // will be called to redisplay the window's normal plane.
    glutPostRedisplay();
    break;
  case 27: /*  Escape Key  */
    exit(0);
    break;
  default:
    break;
    }
}

/*  Main Loop
 *  Open window with initial window size, title bar, 
 *  RGBA display mode, and handle input events.
 */
int
main(int argc, char** argv)
{
  // We're not passing any argument to glutInit, sicne the argv is empty.
  // Initialising the GLUT. It must be always called, before calling any 
  // other GLUT functions.
  glutInit(&argc, argv);

  // Initialising the display mode. e.g. GLUT_SINGLE, GLUT_DOUBLE, GLUT_DEPTH
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

  // Setting the windows size
  glutInitWindowSize(200, 200);

  // The passed argument is going to be displayed as the window name.
  glutCreateWindow(argv[0]);

  init();
  // Callback functions
  // The reshape callback is triggered when a window is reshaped.
  // glViewport(0,0,width,height) is called if nothing is provided.
  glutReshapeFunc(reshape);

  // Sets the keyboard callback for the current window.
  glutKeyboardFunc(keyboard);

  // When GLUT determines that the normal plane for the window needs to be 
  // redisplayed, the display callback for the window is called.
  glutDisplayFunc(display);

  // enters the GLUT event processing loop. This routine should be called at 
  // most once in a GLUT program. Once called, this routine will never return.
  // It will call as necessary any callbacks that have been registered.
  glutMainLoop();

  return 0;
}
```

# Tutorial #
  * [Tutorial by University of Colorado](http://www.cs.uccs.edu/~semwal/indexGLTutorial.html)
  * [This tutorial](http://www.videotutorialsrock.com) is quite old, but has a complete description, video, example code, etc. It will be good for the starters. Hope you enjoy it! :D
  * [A few sample codes](http://www.opengl.org/resources/code/samples/redbook/)

# References #
  1. [OpenGL official website](http://www.opengl.org/)
  1. [OpenGL in Wikipedia](http://en.wikipedia.org/wiki/OpenGL)
  1. [VideoTutorialsRock](http://www.videotutorialsrock.com)