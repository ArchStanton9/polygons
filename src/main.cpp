#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>

#include "PolygonSerializer.h"
#include "PolygonFactory.h"

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Point_2<K> Point;
typedef CGAL::Polygon_2<K> Polygon;

namespace PS = CGAL::Polyline_simplification_2;
typedef PS::Stop_below_count_ratio_threshold Stop;
typedef PS::Squared_distance_cost            Cost;

int main(int argc, char* argv[]) {
    if (argc != 2)
        return -1;

    double threshold = std::stof(argv[1]);

    std::cout << "threshold = " << threshold << std::endl;

    PolygonSerializer serializer;
    Polygon p = serializer.Deserialize("input.p");
    std::cout << p << std::endl;

    Cost cost;
    p = PS::simplify(p, cost, Stop(threshold));

    serializer.Serialize("output.p", p);

    return 0;
}