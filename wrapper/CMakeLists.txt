project(wrapper)

# FIND_PACKAGE(PythonInterp)
FIND_PACKAGE(PythonLibs REQUIRED)
FIND_PACKAGE(Boost COMPONENTS python3 REQUIRED)

#include dirs
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS})
message(STATUS "INCLUDE Boost:'${Boost_INCLUDE_DIRS}'")
message(STATUS "INCLUDE Python:'${PYTHON_INCLUDE_DIRS}'")

include_directories(../external/)

# configure boost
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

# configure source files
set(SOURCE_FILES wrapper.cpp)
configure_file("__init__.py" "__init__.py" COPYONLY)

# building the library
add_library(wrapper SHARED ${SOURCE_FILES})
set_target_properties(wrapper PROPERTIES SUFFIX ".pyd")

# linking
TARGET_LINK_LIBRARIES(wrapper ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
message(STATUS "LINK Python:'${PYTHON_LIBRARIES}'")
message(STATUS "LINK Boost:'${Boost_LIBRARIES}'")