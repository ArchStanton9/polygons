#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <vector>
#include <boost/python.hpp>
#include <halton.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>

typedef CGAL::Simple_cartesian<double>              K;
typedef K::Triangle_2                               Triangle;
typedef K::Point_2                                  Point;
typedef CGAL::Random_points_in_square_2<Point>    Generator;

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
    
    PointSet(std::vector<PyPoint> points) {
        source = points;
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


PointSet GeneratePoints(int count) {
    assert(count > 0);

    std::vector<Point> points;
    std::copy_n(Generator(0.5), count, std::back_inserter(points));

    return PointSet(points);
}

PointSet GenerateHalton(int count) {
    assert(count > 0);

    Halton<> hx(2, 7), hy(3, 5);
    std::vector<PyPoint> points;
    
    while (count-- > 0) {
        auto point = PyPoint(hx() - 0.5, hy() - 0.5);
        points.emplace_back(point);
    }

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
    def("halton", GenerateHalton);

    def("yay", yay);
}