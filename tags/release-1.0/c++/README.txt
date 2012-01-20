How to compile using cmake?

    > mkdir build
    > cd build
    > cmake ..
    > make


For debug (optional),

    > mkdir build_debug
    > cd build_debug
    > cmake -DCMAKE_BUILD_TYPE=Debug ..
    > make

-------------------------------------


After compilation, the "project/c++/build/bin/" directory will have:

bin/
├── gui
├── ihls_nhs
├── post_processing
└── RotationalOffset


-------------------------------------

More info about CMake:
    * http://code.google.com/p/vibot6-mscv3-se/wiki/CMake
    * http://www.cmake.org/cmake/help/documentation.html


=====================================

How to compile the GUI using qmake?

There are two:

1) (with QtCreator)
    Open 'project/c++/gui/gui.pro' with QtCreator, then press "Run"


2) (with the console)
    > cd project/c++/
    > mkdir gui-build
    > cd gui-build
    > qmake ../gui
    > make

-------------------------------------


Note:
  _ Tested with g++ 4.5.1, Qt 4.7.1 and OpenCV 2.3.1

