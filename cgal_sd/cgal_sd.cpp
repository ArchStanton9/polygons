#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <boost/python.hpp>

using namespace boost::python;

// for test
char const* yay() {
    return "Yay!";
}

BOOST_PYTHON_MODULE(wrapper) {
    def("yay", yay);
}