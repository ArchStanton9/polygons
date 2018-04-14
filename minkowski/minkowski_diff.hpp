#include <CGAL/minkowski_sum_2.h>

template <class Kernel>
static CGAL::Point_2<Kernel> reflect(CGAL::Point_2<Kernel> point) {
    return CGAL::Point_2<Kernel>( - point.x(), - point.y());
}

template <class Kernel>
static CGAL::Polygon_with_holes_2<Kernel> minkowski_diff(
    CGAL::Polygon_2<Kernel> left, CGAL::Polygon_2<Kernel> right) {

    CGAL::Polygon_2<Kernel> temp;
    for (auto i = right.vertices_begin(); i < right.vertices_end(); i++) {
        temp.push_back(reflect(*i));
    }

    return CGAL::minkowski_sum_2(left, temp);
}

