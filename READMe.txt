In this page we have summarised how you can download the source code of our project and how to compile the source codes under different environments. For further information please check https://sites.google.com/site/mcvibot2011sep/download .

----------------------------------------------

 * Matlab

We have tested our code under three different operating systems: Windows, Linux, Mac OS. The procedure for all operating systems in the same.
  - Download the Matlab source code here.
  - Unzip the contents of the zip file.
  - Open Matlab and change the "Current folder" to the folder you have extracted the code.
  - Run "mainGUI.m" from the command line.

----------------------------------------------

 * C++

We have tested our code under three different operating systems: Windows, Linux, Mac OS. Unlike the Matlab version the procedure to compile and run the C++ code is different according to your operating systems. We have written a short guideline for each operating system on how to compile the code.

 ## Linux

  - Download the C++ source code here.
  - Install the prerequisite packages
   + sudo apt-get install build-essential libqt4-dev-tools cmake libopencv2.3-dev
  - Compilation 
   + Cmake
    - Compile the projectCompile the project with the following steps
    - cd project/c++
    - mkdir build
    - cd build
    - cmake ..
    - make
    - Run the application: "./bin/gui".
   + Second alternative is to use QT Creator.
    - Install QT Creator "sudo apt-get install qtcreator".
    - Open ''project\c++\gui\gui.pro'' with qtcreator and build it and run it.

----------------------------------------------

 ## Windows

  - Download QT from here: http://qt.nokia.com/downloads/sdk-windows-cpp and install it.
  - Add the following string to the PATH, "C:\Program Files (x86)\CMake 2.8\bin;C:\QtSDK\mingw\bin;C:\opencv-build\install\bin;C:\QtSDK\QtCreator\bin;" (Of course, you have to change the folders accordingly if you have chosen another folders for your installations.) (In some computers adding this to path is also necessary "C:\QtSDK\Desktop\Qt\4.7.4\mingw\bin;".
  - Download CMake from here: http://www.cmake.org/files/v2.8/cmake-2.8.7-win32-x86.exe .
  - Install CMake and make sure you choose this option: Add CMake to the system PATH for all users.
  - Download opencv from here: http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.3.1/OpenCV-2.3.1-win-superpack.exe/download .
  - Install the opencv with the guideline from this page: http://www.laganiere.name/opencvCookbook/chap1.shtml
  - Depending on where you have installed your opencv, you have to change the Makefile. It's very easy, just open "gui.pro" file which is under "gui" folder. Change the parameter "OPENCV_DIR", which is in line 56, to the "install" folder of your opencv. For instace, I installed my opencv in "C:\\opencv-build", therefore I changed the parameters like this: OPENCV_DIR = "C:\\opencv-build\\install".
  - Open ''project\c++\gui\gui.pro'' with qtcreator and build it and run it.

----------------------------------------------

 ## Mac OS.

  - Download and install MacPorts (software to easily install packages and libraries on Mac OS X)
  - Download and install QT libraries for Mac OS X
  - Open up a terminal window (Applications/Utilities/Terminal.app).
  - First, update your macports repositories:   "sudo port selfupdate"
  - Afterwards, you will need to install OpenCV (Although not necessary for this project, you can also install the python bindings in this step) and cmake:  "sudo port install opencv"
  - Install CMake:   "sudo port install cmake"
  - Navigate to a folder where you want to have your local copy of the source, and check out the project:   "svn checkout https://vibot6-mscv3-se.googlecode.com/svn/trunk/ vibot6-mscv3-se --username (your username)"
  - Compile the project with the following steps (same as linux)
  - cd project/c++
  - mkdir build
  - cd build
  - cmake ..
  - make
  - Run the application:    "./bin/gui"

----------------------------------------------

 * How to work with GUI?

Check the examples:

 - Matlab    MatlabGUI
 - C++       QtGui

----------------------------------------------


