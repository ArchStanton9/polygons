#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <vector>
#include <boost/python.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>

typedef CGAL::Simple_cartesian<double>              K;
typedef K::Triangle_2                               Triangle;
typedef K::Point_2                                  Point;
typedef CGAL::Random_points_in_triangle_2<Point>    Generator;

using namespace boost::python;

#pragma region DataStructures

class PyPoint {
public:
    PyPoint(double x, double y) {
        this->x = x;
        this->y = y;
    }

    PyPoint(Point p) {
        x = p.x();
        y = p.y();
    }

    double x;
    double y;
};


class PointSet {
public:
    PointSet() { }
    PointSet(std::vector<Point> points) {
        std::transform(points.begin(), points.end(),
            std::back_inserter(source),
            [](Point p) {return PyPoint(p); });
    }

    std::vector<PyPoint>::iterator begin() {
        return source.begin();
    }

    std::vector<PyPoint>::iterator end() {
        return source.end();
    }

private:
    std::vector<PyPoint> source;
};

#pragma endregion

PointSet GeneratePoints() {
    Triangle triangle(Point(0.0, 0.0), Point(50.0, 0.0), Point(0.0, 50.0));
    std::vector<Point> points;
    std::copy_n(Generator(triangle), 55, std::back_inserter(points));

    return PointSet(points);
}

// for test
char const* yay() {
    return "Yay!";
}

BOOST_PYTHON_MODULE(wrapper) {
    class_<PointSet>("PointSet")
        .def("points", range(&PointSet::begin, &PointSet::end));

    class_<PyPoint, boost::shared_ptr<PyPoint>>(
        "Point", init<double, double>())
        .def_readwrite("x", &PyPoint::x)
        .def_readwrite("y", &PyPoint::y);

    def("random", GeneratePoints);

    def("yay", yay);
}