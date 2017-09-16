//
// Created by arch on 9/9/17.
//

#include "PolygonFactory.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Random_points_in_square_2< Point>  Point_generator;

Polygon PolygonFactory::Build(double radius, int size, unsigned int seed) {
    Polygon polygon;

    // create 50-gon and write it into a window:
    CGAL::random_polygon_2(size, std::back_inserter(polygon),
                           Point_generator(radius));

    return polygon;
}
