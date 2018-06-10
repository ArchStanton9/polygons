#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/minkowski_sum_2.h>

#include <PolygonSerializer.hpp>

#include <sstream>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel>                             Point;
typedef CGAL::Polygon_2<Kernel>                           Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;

// conver bbox to polygon
Polygon get_bbox_polygon(Polygon polygon) {
    auto bbox = polygon.bbox();
    auto xmin = bbox.xmin();
    auto xmax = bbox.xmax();
    auto ymin = bbox.ymin();
    auto ymax = bbox.ymax();
    
    auto points = {
        Point(xmin, ymin),
        Point(xmax, ymin),
        Point(xmax, ymax),
        Point(xmin, ymax)
    };

    Polygon result;
    for (auto p : points) {
        result.push_back(p);
    }

    return result;
}

Pwh complement(Polygon polygon) {
    auto bbox = get_bbox_polygon(polygon);
    Pwh result(bbox);

    polygon.reverse_orientation();
    result.add_hole(polygon);

    return result;
}

Polygon reflect(Polygon polygon) {
    CGAL::Polygon_2<Kernel> reflection;
    for (auto i = polygon.vertices_begin(); i < polygon.vertices_end(); i++) {

        reflection.push_back(Point(-i->x(), -i->y()));
    }

    return reflection;
}


int main() {
    Polygon polyA;
    for (auto p : { Point(2, 2), Point(8, 2), Point(8, 8) }) {
        polyA.push_back(p);
    }

    Polygon polyB;
    for (auto p : { Point(0, 0), Point(1, 0), Point(1, 1), Point(0, 1) }) {
        polyB.push_back(p);
    }

    CGAL::set_pretty_mode(std::cout);
    std::cout << polyA << std::endl << polyB << std::endl;

    auto left = complement(polyA);
    auto right = reflect(polyB);

    std::cout << left << std::endl;
    std::cout << right << std::endl;

    Pwh result = CGAL::minkowski_sum_2(left, right);
    std::cout << result << std::endl;

    PolygonSerializer<Kernel> s;
    s.Serialize("a.p", polyA);
    s.Serialize("b.p", polyB);

    int i = 0;
    for (auto h = result.holes_begin(); h != result.holes_end(); h++) {
        std::ostringstream ss;
        ss << "h" << i << ".p";
        s.Serialize(ss.str(), *h);
    }

    // system("plot.sh");
    std::cin.get();
}