#ifndef SIMPLIFY_HPP_INCLUDED_G5E32Q
#define SIMPLIFY_HPP_INCLUDED_G5E32Q

#include <algorithm>
#include <list>

#include "generic_polygon.hpp"

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Constrained_triangulation_plus_2.h>
#include <CGAL/Polyline_simplification_2/simplify.h>
#include <CGAL/Polyline_simplification_2/Squared_distance_cost.h>


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


template <class Kernel>
class Polygon_simplify
{
  
  using Vb  = CGAL::Polyline_simplification_2::Vertex_base_2<Kernel>;
  using Fb  = CGAL::Constrained_triangulation_face_base_2<Kernel>;
  using TDS = CGAL::Triangulation_data_structure_2<Vb, Fb>;
  using CDT = CGAL::Constrained_Delaunay_triangulation_2<Kernel, TDS, CGAL::Exact_predicates_tag>;
    // CGAL::Exact_predicates_tag ?
  using CT  = CGAL::Constrained_triangulation_plus_2<CDT>;
  using Cid = typename CT::Constraint_id;

  struct Component_ids
  {
    Cid outer;
    std::list<Cid> holes;
  };
  
  using Components = std::list<Component_ids>;

public:

  template <class GP, class Cost, class Stop>
  void operator()(GP & original, GP & simplified, Cost cost, Stop stop) const
  {
    CT ct;
    Components reflect;
    
    // Push original contours to ct.
    for (auto & component : original.polygons_with_holes_sync())
    {
      reflect.emplace_back();
      auto & cr = reflect.back(); // component reflection.
      cr.outer = ct.insert_constraint(component.outer_boundary());
      
      auto p = component.holes_begin(), e = component.holes_end();
      for (; p != e; ++p)
        cr.holes.push_back(ct.insert_constraint(*p));
    }
    
    // Simplify.
    CGAL::Polyline_simplification_2::simplify(ct, cost, stop);
    
    // Extract simplified contours from ct.
    auto & ph = simplified.polygons_with_holes_reset();
    ph.clear();
    auto fill_polygon = [&](Cid cid, auto & polygon)
    {
      auto p = ct.points_in_constraint_begin(cid),
           e = --ct.points_in_constraint_end(cid);
      
      for (; p != e; ++p)
        polygon.push_back(*p);
    };
    
    for (auto & component : reflect)
    {
      ph.emplace_back();
      auto & pc = ph.back(); // polygon component.
      
      fill_polygon(component.outer, pc.outer_boundary());
      for (auto & hole : component.holes)
      {
        pc.add_hole({});
        fill_polygon(hole, *std::prev(pc.holes_end()));
      }
    }
  }
  
  /// In-place polygon simplification.
  template <class GP, class Cost, class Stop>
  void operator()(GP & gp, Cost cost, Stop stop) const
  {
    (*this)(gp, gp, cost, stop);
  }
};

#endif//SIMPLIFY_HPP_INCLUDED_G5E32Q
