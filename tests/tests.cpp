#define BOOST_TEST_MODULE CoreTests

#include "polygons2D.hpp"
#include "boost\test\included\unit_test.hpp"
#include <vector>

using namespace std;
using namespace Polygons2D;

struct TestFixture {
    TestFixture() { CGAL::set_pretty_mode(std::cout); }
    ~TestFixture() { }
};

BOOST_GLOBAL_FIXTURE(TestFixture);

BOOST_AUTO_TEST_CASE(poly_save_orentation_after_reflection) {
    Polygons2D::Polygon polyA;
    for (auto p : { Point(-1, -1), Point(1, -1), Point(1, 1), Point(-1, 1) }) {
        polyA.push_back(p);
    }

    if (polyA.is_clockwise_oriented())
        BOOST_FAIL("input poly is clockwise oriented");

    BOOST_TEST(!reflect(polyA).is_clockwise_oriented());
}


BOOST_AUTO_TEST_CASE(can_reflect_point) {
    auto point = Point(2, 3);
    auto reflection = reflect(point);

    std::cout << reflection;
    BOOST_TEST(point.x() == -reflection.x());
    BOOST_TEST(point.y() == -reflection.y());
}


BOOST_AUTO_TEST_CASE(can_reflect_polygon) {
    Polygons2D::Polygon polyA;
    std::vector<Point> points = { Point(-1, -1), Point(1, -1), Point(2, 2) };
    for (auto p : points) {
        polyA.push_back(p);
    }

    if (polyA.is_clockwise_oriented())
        BOOST_FAIL("input poly is clockwise oriented");

    Polygons2D::Polygon polyB = reflect(polyA);
    std::vector<Point> result = std::vector<Point>(polyB.vertices_begin(), polyB.vertices_end());

    BOOST_ASSERT(points.size() == result.size());

    for (size_t i = 0; i < points.size(); i++) {
        auto before = points[i];
        auto after = result[i];
        BOOST_TEST(before.x() == -after.x());
        BOOST_TEST(before.y() == -after.y());
    }
}
