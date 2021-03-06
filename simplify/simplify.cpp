#include <iostream>
#include <fstream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>

#include <PolygonFactory.hpp>
#include <PolygonSerializer.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon;

namespace PS = CGAL::Polyline_simplification_2;
typedef PS::Stop_below_count_ratio_threshold     Stop;
typedef PS::Squared_distance_cost                Cost;
typedef PS::Scaled_squared_distance_cost         ScaledCost;
typedef PS::Hybrid_squared_distance_cost<double> HybridCost;

int main(int argc, char* argv[]) {
	double threshold = argc == 2
		? std::stof(argv[1])
		: 0.4;

	std::cout << "threshold = " << threshold << std::endl;

	PolygonSerializer<K> serializer;
	Polygon p;

	if (!std::ifstream("input.p")) {
		// create and save random polygon if input file is not specified
		PolygonFactory<K> factory;
		p = factory.Build(50, 60);
		serializer.Serialize("input.p", p);
	}
	else {
		p = serializer.Deserialize("input.p");
	}

	Cost cost;
	p = PS::simplify(p, cost, Stop(threshold));
	serializer.Serialize("SD_cost.p", p);

	ScaledCost scaledCost;
	p = PS::simplify(p, scaledCost, Stop(threshold));
	serializer.Serialize("scaled_cost.p", p);

	HybridCost hybridCost(4);
	p = PS::simplify(p, hybridCost, Stop(threshold));
	serializer.Serialize("hybrid_cost.p", p);


	return 0;
}