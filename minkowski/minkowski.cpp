#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/minkowski_sum_2.h>

#include <PolygonSerializer.hpp>
#include <PolygonFactory.hpp>

#include <vector>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel>                             Point;
typedef CGAL::Polygon_2<Kernel>                           Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;


int main(){
    Polygon polyA;
    for (auto p : { Point(4, 0), Point(2, 1), Point(0, 0) }) {
        polyA.push_back(p);
    }

    Polygon polyB;
    for (auto p : { Point(14, 4), Point(10, 9), Point(5, 8), Point(4, 4) }) {
        polyB.push_back(p);
    }

    CGAL::set_pretty_mode(std::cout);
    std::cout << polyA << std::endl << polyB << std::endl;
    assert(CGAL::do_intersect(polyA, polyB) == false);

    Pwh sum = CGAL::minkowski_sum_2(polyA, polyB);
    std::cout << sum << std::endl;
    PolygonSerializer<Kernel> s;
    s.Serialize("polyA.p", polyA);
    s.Serialize("polyB.p", polyB);
    s.Serialize("sum.p", sum);
    std::cin.get();

    return 0;
}