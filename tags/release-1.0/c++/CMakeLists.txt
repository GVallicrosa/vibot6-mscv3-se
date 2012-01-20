# Top-Level CmakeLists.txt
cmake_minimum_required (VERSION 2.8.3) 

# Project Name
project(RoadSign)


# list of Sub-directories
set( SUBDIRECTORY   ihls_nhs
                    post_processing
                    RotationalOffset
                    gui )


# Add these 
foreach( subdir ${SUBDIRECTORY} )
    add_subdirectory( ${subdir} )
endforeach(subdir)


# Changing 'RUNTIME_OUTPUT_DIRECTORY' to each subdir
set(COMMON_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

foreach( subdir ${SUBDIRECTORY} )
    set_target_properties( ${subdir} PROPERTIES
                           RUNTIME_OUTPUT_DIRECTORY ${COMMON_RUNTIME_OUTPUT_DIRECTORY} )
endforeach(subdir)
