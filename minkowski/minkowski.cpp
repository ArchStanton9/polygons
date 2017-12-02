#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/minkowski_sum_2.h>

#include <PolygonSerializer.hpp>
#include <minkowski_diff.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel>                             Point;
typedef CGAL::Polygon_2<Kernel>                           Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;

int main(){
    Polygon polyA;
    for (auto p : { Point(1, 1), Point(3, 1), Point(3, 4), Point(1, 4) }) {
        polyA.push_back(p);
    }

    Polygon polyB;
    for (auto p : { Point(0, 2), Point(2, 2), Point(0, 3) }) {
        polyB.push_back(p);
    }

    CGAL::set_pretty_mode(std::cout);
    std::cout << polyA << std::endl << polyB << std::endl;

    Pwh sum = CGAL::minkowski_sum_2(polyA, polyB);
    assert(!sum.has_holes());
    std::cout << sum << std::endl;

    Pwh diff = minkowski_diff(polyA, polyB);
    assert(!diff.has_holes());
    std::cout << diff << std::endl;

    PolygonSerializer<Kernel> s;
    s.Serialize("polyA.p", polyA);
    s.Serialize("polyB.p", polyB);
    s.Serialize("sum.p", sum);
    s.Serialize("diff.p", diff);
    std::cin.get();

    return 0;
}