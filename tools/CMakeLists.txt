project(tools)

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


set(SOURCE_FILES
        PolygonSerializer.hpp
        PolygonFactory.hpp)

add_library(tools STATIC ${SOURCE_FILES})
set_target_properties(tools PROPERTIES LINKER_LANGUAGE CXX)