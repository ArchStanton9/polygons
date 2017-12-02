#define BOOST_PYTHON_STATIC_LIB
#define BOOST_NUMPY_STATIC_LIB

#include <vector>
#include <math.h>
#include <boost/python.hpp>
#include <halton.hpp>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Simple_cartesian<double>          K;
typedef K::Triangle_2                           Triangle;
typedef K::Point_2                              Point;
typedef K::Angle                                Angle;
typedef CGAL::Delaunay_triangulation_2<K>       Triangulation;
typedef CGAL::Random_points_in_square_2<Point>  Generator;

using namespace boost::python;

#pragma region DataStructures

class PyPoint {
public:
    PyPoint() : PyPoint(0.0, 0.0) { }

    PyPoint(double x, double y) {
        this->x = x;
        this->y = y;
    }

    PyPoint(Point p) {
        x = p.x();
        y = p.y();
    }

    operator Point () { return Point(x,y); }

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

class PyTriangle{
public:
    PyTriangle() { }
    PyTriangle(PyPoint p1, PyPoint p2, PyPoint p3) {
        a = p1; b = p2; c = p3;
        source = Triangle(
            Point(a.x, a.y),
            Point(b.x, b.y),
            Point(c.x, c.y));
    }

    PyTriangle(Point p1, Point p2, Point p3) {
        a = PyPoint(p1);
        b = PyPoint(p2);
        c = PyPoint(p3);
        source = Triangle(p1, p2, p3);
    }

    PyPoint a, b, c;

    double area() {
        return source.area();
    }

    double angle() {
        auto a1 = calculate_angle(a, b, c);
        auto a2 = calculate_angle(b, c, a);
        auto a3 = calculate_angle(c, a, b);

        return std::min({a1, a2, a3});
    }

private:
    Triangle source;

    double calculate_angle(PyPoint p1, PyPoint p2, PyPoint p3) {
        K::Vector_2 v1(static_cast<Point>(p2), static_cast<Point>(p1));
        K::Vector_2 v2(static_cast<Point>(p2), static_cast<Point>(p3));
        auto rad = acos((v1 * v2) / sqrt(v1.squared_length() * v2.squared_length()));

        return rad * 180 / CGAL_PI;
    }
};


class PyTriangulation
{
public:
    PyTriangulation(PointSet points) {
        std::stringstream st;
        for (auto p : points) {
            st << p.x << " " << p.y << std::endl;
        }

        std::istream_iterator<Point> begin(st);
        std::istream_iterator<Point> end;
        Triangulation t;
        t.insert(begin, end);

        for (auto f = t.faces_begin(); f < t.faces_end(); f++) {
            auto p1 = f->vertex(0)->point();
            auto p2 = f->vertex(1)->point();
            auto p3 = f->vertex(2)->point();

            triangles.emplace_back(PyTriangle(p1, p2, p3));
        }
    }

    std::vector<PyTriangle>::iterator begin() {
        return triangles.begin();
    }

    std::vector<PyTriangle>::iterator end() {
        return triangles.end();
    }

private:
    std::vector<PyTriangle> triangles;
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
    class_<PyPoint, boost::shared_ptr<PyPoint>>(
        "Point", init<double, double>())
        .def_readwrite("x", &PyPoint::x)
        .def_readwrite("y", &PyPoint::y);

    class_<PyTriangle, boost::shared_ptr<PyTriangle>>(
        "Triangle", init<PyPoint, PyPoint, PyPoint>())
        .def_readwrite("a", &PyTriangle::a)
        .def_readwrite("b", &PyTriangle::b)
        .def_readwrite("c", &PyTriangle::c)
        .def("area", &PyTriangle::area)
        .def("angle", &PyTriangle::angle);

    class_<PointSet>("PointSet")
        .def("points", range(&PointSet::begin, &PointSet::end));

    class_<PyTriangulation>("Triangulation", init<PointSet>())
        .def("triangles", range(&PyTriangulation::begin, &PyTriangulation::end));

    def("random", GeneratePoints);
    def("halton", GenerateHalton);

    def("yay", yay);
}