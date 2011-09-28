//*********************************************************************/
//
//					constante.h
//
//*********************************************************************/

#ifndef _CONSTANTE_
#define _CONSTANTE_

//math constants
#define PI 3.14159265358979323846
#define ZERO 1e-9
#define EPSILON 1e-9
#define ITERATION_MAX 10
#define NEGATIVE -1
#define POSITIVE 1
#define FLAT_TRIANGLE_AREA 0.0001

//Display Modes
#define NO_DISPLAY_PARAM			0000010
#define NORMAL_DISPLAY_MODE			0000020
#define FACE_NORMAL_DISPLAY_MODE	0000021
#define RGB_DISPLAY_MODE			0000030
#define FULL_DISPLAY_MODE			0000040
#define ONLY_VERTEX					1000000
#define FACE_NORMAL					1000100
#define FACE_NORMAL_RGB				1000200
#define VERTEX_NORMAL				1000300
#define VERTEX_NORMAL_RGB			1000400

//Boolean operations
#define NO_OPERATION				-1
#define BOOLEAN_UNION				200
#define BOOLEAN_INTERSECTION		201

//Primitives handled
#define NOT_PRIMITIVE				-1
#define TYPE_SUPERSHAPE				300
#define TYPE_SUPERQUADRIC			301
#define TYPE_DUPINCYCLIDE			302
#define TYPE_CSG_TREE				303

// R_FUNCTIONS values

#define R_FUNCTION_ALPHA			1
#define R_FUNCTION_M				4
#define R_FUNCTION_P				2

#define SampleMax					2

#endif
