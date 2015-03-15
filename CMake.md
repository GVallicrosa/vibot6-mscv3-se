# Why CMake? #

  * Cross-platform build system.
  * [Out-of-source\_build\_trees](http://www.cmake.org/Wiki/CMake_FAQ#Out-of-source_build_trees)
  * [CPack](http://www.cmake.org/Wiki/CMake:Packaging_With_CPack) - _make package_
    * [CPack\_Package\_Generators](http://www.cmake.org/Wiki/CMake:CPackPackageGenerators)
      * TGZ, STGZ, TBZ2, TZ, ZIP
      * NSIS (Windows only)
      * RPM, DEV (Unix only)
      * DragNDrop, PackageMaker, OSXX11, Bundle (OSX only)
  * [Generators](http://www.cmake.org/cmake/help/cmake-2-8-docs.html#section_Generators)
    * MinGW Makefiles
    * NMake Makefiles
    * Unix Makefiles
    * Visual Studio 6, 7, 8, 9, 10
    * CodeBlocks - MinGW and Unix Makefiles
    * Eclipse CDT4 - MinGW, NMake and Unix Makefiles