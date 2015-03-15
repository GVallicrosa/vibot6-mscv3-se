# Introduction #
In this page we'll go through a step by step instruction on how to compile the code of project and how to run it.

# Linux #
An eclipse project is created for our source code, however no one is forced to use that IDE, if you don't like it. Since now we got a makefile for our project, you can simply run the code from terminal if you like to do so, or if you like some other IDEs, let say Code Blocks, you can export the project to that IDE and use our makefile to compile the code and run it. Here, I'll try to cover three different options, eclipse, terminal, and Code Blocks.

## Glut ##
You need to install glut library before you continue this tutorial. You can easily do it by running the following command.
```
sudo apt-get install glutg3 glutg3-dev
```

## Eclipse ##
  * Setting up eclipse
    1. Install eclipse-cdt on your machine by running the following command in the terminal (you can run the terminal by pressing the shortcut ctrl + alt + t).
```
sudo apt-get install eclipse-cdt
```
    1. Open your eclipse and go to the 'help' menu, and then click on 'install new software'.
    1. Enter this link in the 'work with' box and press enter. http://subclipse.tigris.org/update_1.6.x
    1. In the box below expand 'subclipse' and check the three 'required' options. (Subclipse (Required), Subversion Client Adapter (Required), Subversion JavaHL Native Library Adapter (Required)). And press on finish and follow the instruction to complete the installation
    1. Close your eclipse and run the following command in terminal.
```
sudo apt-get install libsvn-java
```
    1. Open your eclipse again and press 'window' menu, and submenu 'open perspective' and press on 'other'. When the menu is opened click on 'SVN Repository Exploring'.
    1. Right click on the 'SVN Repositories' perspective (usually located on the left of screen), and go to 'new'and then 'repository location'.
    1. Provide this link https://vibot6-mscv3-se.googlecode.com/svn/trunk/ and press finish.
    1. Expand the repository. When the contents of repository is loaded, right click on the 'vibot6\_mscv3' folder and press on 'checkout'
    1. Press 'finish'. Now the code will be checked out into your computer. It might take sometime, because of the size of images.
    1. When it's completed switch back to C/C++ perspective and voila you got the code.
  * Compiling on eclipse
    1. When you're in the C/C++ perspective, right click on the project and go to 'run as' and run it as 'local C/C++ application'.

## Terminal ##
  1. Check out the code by running following command.
```
svn checkout https://vibot6-mscv3-se.googlecode.com/svn/trunk/ vibot6-mscv3-se --username something@gmail.com
```
  1. Navigate to the vibot6\_mscv3 folder.
  1. Run this command.
```
make install
```
  1. When compilation is done, run this command.
```
./vibot6_mscv3
```

The application should be running now.

## Code Blocks ##
  1. Checkout the code as mentioned in the terminal version.
  1. Create an empty Code Blocks project and add all the files to the project you just created.
  1. Right click on the project you created and go to properties.
  1. In the 'project settings' tab, check the 'this is custom makefile', and give the path to the makefile in 'vibot6\_mscv3' folder, which just checked out.
  1. Run the code!
**I haven't tested it with Code Blocks, so I cant grantee that the instructions I wrote works 100% properly, so you might have to perform some additional steps. But, I can grantee that it should work in Code Blocks with the 'custom make file'.**

# Windows #
Follow the following steps to check out the code, compile and run in on your Windows machine.

  1. Download QT from here: http://qt.nokia.com/downloads/sdk-windows-cpp
  1. Do the installation for the file you just downloaded.
  1. Add the following string to the PATH, ''C:\Program Files (x86)\CMake 2.8\bin;C:\QtSDK\mingw\bin;C:\opencv-build\install\bin;C:\QtSDK\QtCreator\bin;'' (Of course, you have to change the folders accordingly if you have chosen another folders for your installations.)
  1. Download CMake from here: http://www.cmake.org/files/v2.8/cmake-2.8.7-win32-x86.exe
  1. Install CMake and make sure you choose this option: Add CMake to the system PATH for all users.

![http://www.laganiere.name/opencvCookbook/img/c1image15.png](http://www.laganiere.name/opencvCookbook/img/c1image15.png)

  1. Download opencv from here: http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.3.1/OpenCV-2.3.1-win-superpack.exe/download
  1. Install the opencv with the guideline from this page: http://www.laganiere.name/opencvCookbook/chap1.shtml
  1. I have installed my opencv under "C:\opencv-build", if you do the same then you don't need to change any settings later in project makefile.
  1. Open ''project\c++\gui\gui.pro'' with qtcreator and build it and run it.

# Mac #

To compile on mac, you can use cmake. You will need:

  * [MacPorts](http://www.macports.org/) - Software to easily install packages and libraries on Mac OS X, somewhat like Aptitude for Ubuntu
  * The [QT](http://qt.nokia.com/downloads/qt-for-open-source-cpp-development-on-mac-os-x) libraries for Mac OS X

Open up a terminal window (Applications/Utilities/Terminal.app). First, update your macports repositories:

```
sudo port selfupdate
```
Afterwards, you will need to install OpenCV (Although not necessary for this project, you can also install the python bindings in this step) and cmake:
```
sudo port install opencv
sudo port install cmake
```

Navigate to a folder where you want to have your local copy of the source, and check out the project:
```
svn checkout https://vibot6-mscv3-se.googlecode.com/svn/trunk/ vibot6-mscv3-se --username (your username)
cd project
cd c++
```
Now use cmake to compile it:
```
mkdir build
cd build
cmake ..
make
```

You're all set to start using the project! For example, try running the GUI:
```
./gui
```