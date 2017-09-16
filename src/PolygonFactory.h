//
// Created by arch on 9/9/17.
//

#ifndef POLYGONS_POLYGONFACTORY_H
#define POLYGONS_POLYGONFACTORY_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/point_generators_2.h>
#include <CGAL/random_polygon_2.h>


class PolygonFactory {

public:
    CGAL::Polygon_2<CGAL::Exact_predicates_inexact_constructions_kernel> Build(double radius, int size, unsigned int seed);
};


#endif //POLYGONS_POLYGONFACTORY_H
