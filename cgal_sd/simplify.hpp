#ifndef SIMPLIFY_HPP_INCLUDED_G5E32Q
#define SIMPLIFY_HPP_INCLUDED_G5E32Q

#include <algorithm>
#include <list>
#include <CGAL/Polyline_simplification_2/simplify.h>

#include "generic_polygon.hpp"

namespace PS = CGAL::Polyline_simplification_2;
using Stop = PS::Stop_above_cost_threshold;
using Cost = PS::Scaled_squared_distance_cost;


template <class Gp>
void simplify_gp(Gp & polygon, double ratio) {
    using Pwh = typename Gp::Polygon_with_holes;
    using Polygon = typename Gp::Polygon;

    auto cost = Cost();
    auto stop = Stop(ratio);
    
    std::list<Pwh> parts;
    auto & set = polygon.polygon_set_modify();
    set.polygons_with_holes(std::back_inserter(parts));
    set.clear();

    for(auto & p : parts)
    {
        auto exterior = CGAL::Polyline_simplification_2::simplify(
            p.outer_boundary(), cost, stop);

        std::list<Polygon> holes;   
        for(auto h = p.holes_begin(); h != p.holes_end(); h++)
        {
            auto hole = CGAL::Polyline_simplification_2::simplify(*h, cost, stop);
            holes.push_back(hole);
        }
        
        set.insert(Pwh(exterior, holes.begin(), holes.end()));
    }
}

#endif//SIMPLIFY_HPP_INCLUDED_G5E32Q
