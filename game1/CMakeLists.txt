project(game1)

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

include_directories(../tools/)
include_directories(../core/)

# include for local package


# Creating entries for all C++ files with "main" routine
# ##########################################################

include( CGAL_CreateSingleSourceCGALProgram )

set(SOURCE_FILES game.cpp)
configure_file(plot.sh plot.sh COPYONLY)
configure_file(plot_w.sh plot_w.sh COPYONLY)

create_single_source_cgal_program(${SOURCE_FILES})