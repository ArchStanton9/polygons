//
// Created by arch on 9/23/17.
//

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>

#include <PolygonSerializer.hpp>
#include <PolygonFactory.hpp>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef CGAL::Polygon_2<Kernel>                           Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>                Pwh;

int main ()
{
    PolygonSerializer<Kernel> serializer;
    PolygonFactory<Kernel> factory;

    Polygon_2 a = factory.Build(10, 5);
    serializer.Serialize("a.p", a);

    Polygon_2 b = factory.Build(10, 5);
    serializer.Serialize("b.p", b);

    if ((CGAL::do_intersect (a, b)))
        std::cout << "The two polygons intersect in their interior." << std::endl;
    else {
        std::cout << "The two polygons do not intersect." << std::endl;
        return 0;
    }

    std::list<Pwh> intR;
    CGAL::intersection(a, b, std::back_inserter(intR));

    int n = 0;
    for (auto it = intR.begin(); it != intR.end(); ++it) {
        std::stringstream ss;
        ss << "c" << ++n << ".p";
        serializer.Serialize(ss.str(), *it);
    }

    return 0;
}
