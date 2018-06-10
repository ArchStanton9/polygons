cmake_minimum_required(VERSION 3.7)
project(polygons)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3 -Os")

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

# Creating entries for all C++ files with "main" routine
# ##########################################################

add_subdirectory(core)
# add_subdirectory(tests)
add_subdirectory(tools)
# add_subdirectory(noise)
# add_subdirectory(intersection)
# add_subdirectory(simplify)
# add_subdirectory(grid)
# add_subdirectory(wrapper)
# add_subdirectory(minkowski)
add_subdirectory(difference)
# add_subdirectory(game1)