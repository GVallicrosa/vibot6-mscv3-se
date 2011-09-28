//Isosurface.h
#include "VectorN.h"

typedef struct {
   /*XYZ*/
  Vector3d p[3];
} TRIANGLE;

typedef struct {
   /*XYZ*/
   Vector3d p[8];
   double val[8];
} GRIDCELL;

Vector3d VertexInterp(double isolevel, Vector3d p1, Vector3d p2, double valp1, double valp2);
