#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "PolygonSerializer.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;

int main() {
    PolygonSerializer serializer;

    Polygon p;
    p.push_back(Point(32, 12));
    p.push_back(Point(34, 22));
    p.push_back(Point(55, 18));
    p.push_back(Point(11, 2));

    serializer.Serialize("out.p", p);

    Polygon p2 = serializer.Deserialize("out.p");
    std::cout << p << std::endl << p2;

    return 0;
}