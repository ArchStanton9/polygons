cmake_minimum_required(VERSION 3.7)
project(Polygons)

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

    message(STATUS "This project requires the CGAL library, and will not be compiled.")
    return()

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

    message(STATUS "This project requires the Boost library, and will not be compiled.")

    return()

endif()

# include for local directory

configure_file(plot.sh plot.sh COPYONLY)
include_directories(src/tools/)

# include for local package



# Creating entries for all C++ files with "main" routine
# ##########################################################

include( CGAL_CreateSingleSourceCGALProgram )

set(CMAKE_CXX_STANDARD 14)

set(TOOLS_HEADERS
        src/tools/PolygonFactory.h
        src/tools/PolygonSerializer.h)

set(SIMPLIFY_SOURCE_FILES
        src/simplify/simplify.cpp)

set(INTERSECT_SOURCE_FILES
        src/intersection/intersection.cpp)

create_single_source_cgal_program(${INTERSECT_SOURCE_FILES})

create_single_source_cgal_program(${SIMPLIFY_SOURCE_FILES})