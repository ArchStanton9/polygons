#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/minkowski_sum_2.h>

#include <PolygonSerializer.hpp>
#include <polygons2D.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Point_2<Kernel>                             Point;
typedef CGAL::Polygon_2<Kernel>                           Polygon;
typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;

#pragma region Geometry difference

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

Polygon geometry_difference(Polygon polyA, Polygon polyB) {
    auto comp = complement(polyA);
    auto refl = reflect(polyB);

    std::cout << comp << std::endl;
    std::cout << refl << std::endl;

    Pwh result = CGAL::minkowski_sum_2(comp, refl);
    std::cout << result << std::endl;

    for (auto h = result.holes_begin(); h != result.holes_end(); h++) {
        Polygon hole = *h;
        if (hole.orientation() == CGAL::Orientation::POSITIVE)
            return hole;

        Polygon result;
        auto p = hole.vertices_end();
        while (p != hole.vertices_begin()) {
            --p;
            result.push_back(*p);
        }
        assert(result.orientation() == CGAL::Orientation::POSITIVE);

        return result;
    }
}

#pragma endregion


int main() {
    PolygonSerializer<Kernel> s;
    Polygon polyP = s.Deserialize("data/P.p");
    Polygon polyQ = s.Deserialize("data/Q.p");
    Polygon polyM = s.Deserialize("data/M.p");

    CGAL::set_pretty_mode(std::cout);
    std::cout << polyP << std::endl << polyQ << std::endl << polyM << std::endl;
    
    assert(polyP.orientation() == CGAL::Orientation::POSITIVE);
    assert(polyQ.orientation() == CGAL::Orientation::POSITIVE);
    assert(polyM.orientation() == CGAL::Orientation::POSITIVE);

    Polygon polyW = polyM;
    for (size_t i = 0; i < 5; i++) {
        auto sum = CGAL::minkowski_sum_2(polyW, polyP).outer_boundary();
        polyW = Polygons2D::geometry_difference(sum, polyQ);        

        std::cout << polyW << std::endl;
        std::ostringstream ss;
        ss << "data/" << "W" << i << ".p";
        s.Serialize(ss.str(), polyW);
    }

    std::cin.get();

    return 0;
} 