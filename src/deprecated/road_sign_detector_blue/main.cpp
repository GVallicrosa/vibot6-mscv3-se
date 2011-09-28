//main.cpp

#include <iostream>
#include <fstream>
#include "scene.h"
#include "Constante.h"
#include "useful.h"
#include "Stopwatch.h"
#include "random-standalone.h"
#include <time.h>
#include "string.h"
#include "SuperFormula.h"
#include <limits>

using namespace std;

string shortfilename ("Different0035");

//
//global variables
//

Stopwatch timer;
scene ze_scene;
int id_imagelist;

Random R1;  // in case you need a good random variable generator
int id_toto,id_testshape; // openGL display lists
ofstream logfile;       // in case you want to log

double a_scale_1,a_offset_1,b_scale_1,b_offset_1,
       n_scale_1,n_offset_1,
       tht_offset_1, tht_scale_1, phi_offset_1, phi_scale_1,
       x_offset_1, x_scale_1, y_offset_1, y_scale_1, z_offset_1, z_scale_1;

double RADIALNOISEPERCENTAGE,OPTIMALERRORSTOP, PROBA_MUTATE, RANDOMRADIUS, kmutate, thresholdvariance;


double a_scale_2,a_offset_2,b_scale_2,b_offset_2,
       n_scale_2,n_offset_2,
       tht_offset_2, tht_scale_2, phi_offset_2, phi_scale_2,
       x_offset_2, x_scale_2, y_offset_2, y_scale_2, z_offset_2, z_scale_2;

Vector3d Camera_Target, Camera_Position;
int p_scale_1,p_offset_1,q_scale_1,q_offset_1;
int p_scale_2,p_offset_2,q_scale_2,q_offset_2;

int ITERATIONMAX, nbdata,pop, MAXRESTART;
double errth;
//global variable for OpenGL display lists
vector <int> id_curves;
int id_data, id_curve, id_all, id_all2;

vector < RationalSuperShape2D > ArrayShape;
int arrayshapecursor(0);
vector < Vector2d > Data;

int cursor=0;
vector < vector < double > > Evolution;
vector <int> Results;

double p,thtoffset;

GLuint TexNum[1];


int main(int argc,char ** argv){


 timer.Start();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA |GLUT_DEPTH);





    glutInitWindowSize(800,800);
	glutInitWindowPosition(00,00);
    glutCreateWindow("YF_Viewer 1.1");
	glutReshapeFunc(reshape);
	glutMotionFunc(Motion);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutDisplayFunc(display);



    Init();
    Vector2d ImRes;    LoadJPG ( "Images\\Different0035.jpg", 0 , ImRes);

    Camera_Position[0] =0;
    Camera_Position[1] =0;
    Camera_Position[2] =max (ImRes[0],ImRes[1]);



    time_t now;
    time(&now);
    R1.randomize(now);



    double ErrorOfFit;

    // read data


    vector < Vector2d > SmoothData;
    vector < Vector2d > OffsetData;


    // open original full contour (in cartesian coordinate)
    string filename ("Contours\\Full\\");
    filename.append(shortfilename);
    filename.append(".jpg_contour3.txt");

    ifstream infile;
    infile.open(filename.c_str());

    // Test if file is open
	if( infile.is_open() == false )
	{
		cout<<"No contour file found"<<endl;
		return false;
	}

    string line, word;
    vector < string > node;

    Vector2d AVG (0,0);

    //process line by line
    while( getline( infile, line ) )
	{
        //process the line
        istringstream word_stream( line );

        node.clear();
        //read word by word and store in the array
		while( word_stream >> word )
		{
		    node.push_back(word);
		}

		if(node.size() != 2) {cout<<"Syntax error while readind data"<<endl;exit(0);}

        Vector2d P(0,0);
        for(int i=0; i<node.size(); i++)
        {
            P[i] =  atof(node[i].c_str()); // atof is bad, but it works...
        }

        Data.push_back(P);
        AVG += P;
        //cout << "Data point="<< P <<endl; //sampe sini datanya masih benar berupa koordinat kartesian
    }
    infile.close();
    //system("pause");

    AVG /= Data.size();

    cout << "AVG="<<endl<<AVG<<endl;
    //system("pause");

/*    // open new smoothed data (in polar coordinate)
    string filename2("Contours\\Smooth\\Cartesian\\");
    filename2.append(shortfilename);
    filename2.append(".jpg_contour1.txt");

    ifstream infile2;
    infile2.open(filename2.c_str());

    // Test if file is open
	if( infile2.is_open() == false )
	{
		cout<<"No Smooth contour file found"<<endl;
		return false;
	}

    string line2, word2;
    vector < string > node2;

    //process line by line
    while( getline( infile2, line2 ) )
	{
        //process the line
        istringstream word_stream2(line2);

        node2.clear();
        //read word by word and store in the array
		while( word_stream2 >> word2 )
		{
		    node2.push_back(word2);
		}

		if(node2.size() != 2) {cout<<"Syntax error while readind data"<<endl;exit(0);}

        Vector2d P2(0,0);
        for(int i=0; i<node2.size(); i++)
        {
            P2[i] =  atof(node2[i].c_str()); // atof is bad, but it works...
        }

        SmoothData.push_back(P2);

    }
    infile2.close();
*/

    string filename2("Contours\\Smooth\\Cartesian\\");
    filename2.append(shortfilename);
    filename2.append(".jpg_contour3.txt");

    ifstream infile2;
    infile2.open(filename2.c_str());

    // Test if file is open
	if( infile2.is_open() == false )
	{
		cout<<"No Smooth contour file found"<<endl;
		return false;
	}

    string line2, word2;
    vector < string > node2;

    //process line by line
    while( getline( infile2, line2 ) )
	{
        //process the line
        istringstream word_stream2(line2);

        node2.clear();
        //read word by word and store in the array
		while( word_stream2 >> word2 )
		{
		    node2.push_back(word2);
		}

		if(node2.size() != 2) {cout<<"Syntax error while readind data"<<endl;exit(0);}

        Vector2d P2(0,0);
        for(int i=0; i<node2.size(); i++)
        {
            P2[i] =  atof(node2[i].c_str()); // atof is bad, but it works...
        }

        SmoothData.push_back(P2);

    }
    infile2.close();


     // load symmetry and theta offset
    string filename3("Contours\\Offset\\");
    filename3.append(shortfilename);
    filename3.append(".jpg_contour3.txt");


    ifstream infile3;
    infile3.open(filename3.c_str());

    // Test if file is open
	if( infile3.is_open() == false )
	{
		cout<<"No Contour offset file found"<<endl;
		return false;
	}

    string line3, word3;
    vector < string > node3;
    while( getline( infile3, line3 ) )
	{
        istringstream word_stream3(line3);
        node3.clear();
		while(word_stream3 >> word3)
		{
		    node3.push_back(word3);
		}

		if(node3.size() != 2) {cout<<"Syntax error while readind data"<<endl;exit(0);}

        Vector2d P3(0,0);
        for(int i=0; i<node3.size(); i++)
        {
            P3[i] =  atof(node3[i].c_str()); // atof is bad, but it works...
        }

        OffsetData.push_back(P3);

    }
    infile3.close();
    p = (OffsetData[0])[0]; thtoffset = (OffsetData[0])[1];
    //Set_p(OffsetData[0]);  Set_thtoffset(OffsetData[1]);

/*
    Camera_Position[0] =0;
    Camera_Position[1] =0;
    Camera_Target[0] = 0;
    Camera_Target[1] = 0;
    Camera_Target[2] = 0;
*/

    //compute the avg radius to place the camera depth

    double r = 0;
    for (int i=0; i< Data.size(); i++)
    {
        r += (Data[i] -  AVG).norm();
        //cout << "Norm="<< (Data[i]-AVG).norm() <<endl;
        Data[i] = Data[i] - AVG; // back translation
        //cout << "Data point#" << i+1 << " = " << Data[i] <<endl;
        //system("pause");
    }
    r/= Data.size();
    cout << "Average radius="<< r <<endl;
//    system("pause");

    for (int i=0; i< Data.size(); i++)
    {

        Data[i] /= 0.5*r; // scale factor to see something

        (Data[i])[1] *= -1; // coordinate inversion to respect image axis
        //cout << "Data point#" << i+1 << "="<< Data[i] <<endl;
    }
//    system("pause");

    // from 3D point to image coordinate:
    // Let P(x,y)
    // do P = P * 0.5*r + AVG

    //Camera_Position[2] = 10; // good position to see something

    //call to the reshape function for preparing the rendering with correct camera/image parameters
  //  reshape(ImRes[0],ImRes[1]);

    //try to fit a gielis curve using 3,4,6, and 8 symmetries

    RationalSuperShape2D BEST;

    vector < double > Errors;

    for (int i=0; i<4; i++)
    {
    ErrorOfFit = 1e30;
    RationalSuperShape2D tmp;

    int ptmp;
    if (i==0) ptmp = 3;
    if (i==1) ptmp = 4;
    if (i==2) ptmp = 6;
    if (i==3) ptmp = 8;


    tmp.Init(
             1,1,   // and b
             2,2,2, // n1  to n3
             ptmp, 1, // and q
             thtoffset,0, // change thtoffset to manual PI/2
             0,0,0);

    vector < Vector2d> CleanedData = tmp.InitRotationAndSymmetry(Data, 1);
                                                                 //min ( 10, int(Data.size() *0.05)) );

    tmp.Optimize2(SmoothData, ErrorOfFit,
                  false, // activate normalization or not
                  1); // the potential function (use 1 or 2, 3 possible but not the most stable)

    tmp.r = r;
    tmp.avg = AVG;
    tmp.ImRes = ImRes;
    ArrayShape.push_back(tmp);
    Errors.push_back(ErrorOfFit);

    }

    string filename4 ("Results\\");
    filename4.append(shortfilename);
    filename4.append("(1).txt");
    ofstream txtresult (filename4.c_str());

    for (int i=0; i<4; i++)
    {
        txtresult << Errors[i] <<"\t\t"<<ArrayShape[i] <<endl;

    }

    txtresult.close();

    cout <<endl<<endl<<"NOW READING IMAGE"<<endl<<endl;


    //now render the image with the best fitted gielis curve

    //glDisable(GL_LIGHTING);

    glBindTexture ( GL_TEXTURE_2D, TexNum[0] );

    id_imagelist=glGenLists(1);
    glNewList(id_imagelist,GL_COMPILE_AND_EXECUTE);

glColor3f(1,1,1);


    glBegin ( GL_QUADS );
        glTexCoord2f ( 0, 0 ); glVertex3d (  -ImRes[0] * 0.5, ImRes[1] * 0.5, 0 );
        glTexCoord2f ( 0, 1 ); glVertex3d (  -ImRes[0] * 0.5,- ImRes[1] * 0.5, 0 );
        glTexCoord2f ( 1, 1 ); glVertex3d (   ImRes[0] * 0.5,- ImRes[1] * 0.5, 0 );
        glTexCoord2f ( 1, 0 ); glVertex3d (  ImRes[0] * 0.5,  ImRes[1] * 0.5, 0 );
    glEnd ( );




glEndList();
//glDisable ( GL_TEXTURE_2D );

    //if you want to run the "interactive mode" uncomment the following line
    glutMainLoop();
    return 0;
}
