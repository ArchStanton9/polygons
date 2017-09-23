//
// Created by arch on 9/9/17.
//

#ifndef POLYGONS_POLYGONFACTORY_H
#define POLYGONS_POLYGONFACTORY_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>

template <class Kernel>
class PolygonFactory {

public:
    CGAL::Polygon_2<Kernel> Build(double radius, int size);
};

template <class Kernel>
CGAL::Polygon_2<Kernel> PolygonFactory<Kernel>::Build(double radius, int size) {
    CGAL::Polygon_2<Kernel> polygon;
    CGAL::random_polygon_2(size,
                           std::back_inserter(polygon),
                           CGAL::Random_points_in_square_2<CGAL::Point_2<Kernel>>(radius));

    return polygon;
}


#endif //POLYGONS_POLYGONFACTORY_H
