project(core)

find_package( CGAL QUIET COMPONENTS  )
find_package( Boost REQUIRED )

include( ${CGAL_USE_FILE} )

set(SOURCE_FILES
        polygons2D.hpp)

add_library(core STATIC ${SOURCE_FILES})
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
