cmake_minimum_required (VERSION 2.8.3)

project( gui )

# with SET() command you can change variables or define new ones
# here we define SAMPLE_SRCS variable that contains a list of all .cpp files
# note that we don't need \ at the end of line
set( SAMPLE_SRCS
        ./gui.cpp
        ./main.cpp
)

# another list, this time it includes all header files that should be treated with moc
set( SAMPLE_MOC_HDRS
    ./gui.h
)

# some .ui files
set( SAMPLE_UIS
    ./gui.ui
    ./options.ui
)

# enable warnings
add_definitions( -Wall )

# by default only QtCore and QtGui modules are enabled
# other modules must be enabled like this:
# SET( QT_USE_QT3SUPPORT TRUE )
# SET( QT_USE_QTXML TRUE )

# this command finds Qt4 and OpenCV libraries and sets all required variables
FIND_PACKAGE( Qt4 REQUIRED )
FIND_PACKAGE(OpenCV REQUIRED)

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
include( ${QT_USE_FILE} )
# include_directories( ${OPENCV_INCLUDE_DIR} )


# this command will generate rules that will run rcc on all files from SAMPLE_RCS
# in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )

# this will run uic on .ui files:
QT4_WRAP_UI( SAMPLE_UI_HDRS ${SAMPLE_UIS} )

# and finally this will run moc:
QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

# we need this to be able to include headers produced by uic in our code
# (CMAKE_CURRENT_BINARY_DIR holds a path to the build directory, while INCLUDE_DIRECTORIES() works just like INCLUDEPATH from qmake)
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

# here we instruct CMake to build "sample" executable from all of the source files
add_executable( gui ${SAMPLE_SRCS} ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} ${SAMPLE_UI_HDRS} )

# last thing we have to do is to tell CMake what libraries our executable needs,
# luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
target_link_libraries( gui ${QT_LIBRARIES} )
