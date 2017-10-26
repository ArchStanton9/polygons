#include <boost\python.hpp>

char const* yay()
{
  return "Yay!";
}

BOOST_PYTHON_MODULE(PyCGAL)
{
  using namespace boost::python;
  def("yay", yay);
}