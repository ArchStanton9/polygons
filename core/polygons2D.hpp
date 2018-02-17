#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/minkowski_sum_2.h>
#include <CGAL/Aff_transformation_2.h>
#include <CGAL/aff_transformation_tags.h>

namespace Polygons2D {

    typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
    typedef CGAL::Point_2<Kernel>                             Point;
    typedef CGAL::Vector_2<Kernel>                            Vector;
    typedef CGAL::Line_2<Kernel>                              Line;
    typedef CGAL::Polygon_2<Kernel>                           Polygon;
    typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;
    typedef CGAL::Aff_transformation_2<Kernel>                Transformation;

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
        Pwh compl(bbox);

        polygon.reverse_orientation();
        compl.add_hole(polygon);

        return compl;
    }

    const Transformation reflection = Transformation(
        -1, 0,
        0, -1
    );

    Point reflect(Point point) {
        return point.transform(reflection);
    }

    Polygon reflect(Polygon polygon) {
        return transform(reflection, polygon);
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
}


