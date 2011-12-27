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


