cmake_minimum_required (VERSION 2.8.3) 

# Project Name
project(ihls_nhs)

FIND_PACKAGE(OpenCV REQUIRED)

# Sources
set( SOURCES    main.cpp
                ihls.cpp
                math_utils.cpp
                nhs.cpp )

# Target
add_executable( ${PROJECT_NAME} ${SOURCES} )
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )
