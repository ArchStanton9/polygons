#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include "PolygonSerializer.h"
#include "PolygonFactory.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;

int main() {
    PolygonSerializer serializer;
    PolygonFactory factory;

    Polygon p = factory.Build(50.0, 24, 42);

    serializer.Serialize("out.p", p);

    // Polygon p2 = serializer.Deserialize("out.p");
    std::cout << p << std::endl;

    return 0;
}