project(tests)

find_package( CGAL QUIET COMPONENTS  )
find_package( Boost REQUIRED )

include( ${CGAL_USE_FILE} )
include( CGAL_CreateSingleSourceCGALProgram )
include_directories(../core)

set(SOURCE_FILES
        tests.cpp)

create_single_source_cgal_program(${SOURCE_FILES})
