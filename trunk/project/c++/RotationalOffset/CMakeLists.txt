cmake_minimum_required (VERSION 2.6) 

# Project Name
project(ihls_nhs)

# Sources
set( SOURCES    main.cpp
                RotationalOffset.cpp )

# Target
add_executable( RotationalOffset ${SOURCES} )