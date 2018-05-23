// minkowski.hpp
// Wrapper for CGAL Minkowski sum (and implements difference) 
// for generic polygons on the plane.
#ifndef MINKOWSKI_HPP_INCLUDED_5QP48D
#define MINKOWSKI_HPP_INCLUDED_5QP48D

#include <algorithm>

#include <CGAL/Origin.h>
#include <CGAL/Arrangement_2.h>
#include <CGAL/Polygon_set_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/minkowski_sum_2.h>

#include "generic_polygon.hpp"


//
// Minkowski sum
//

/// Parameters a and b are not actually changed, but their ph sets are syncronized.
template <class Gp>
void minkowski_sum(Gp & a, Gp & b, Gp & sum)
{
  sum.clear();
  auto & r = sum.polygon_set_modify();
  auto const & ap = a.polygons_with_holes_sync();
  auto const & bp = b.polygons_with_holes_sync();
  
  // TODO: OpenMP parallel sum then aggregate join?
  for (auto & alpha: ap)
  {
    for (auto & beta: bp)
    {
      r.join(CGAL::minkowski_sum_2(alpha, beta));
    }
  }
}


//
// Minkowski difference
//

/// A - B = A + (-B) where unary - is "reflect",
/// -(-B) = B, we call b.reflect twice.
template <class Gp>
void minkowski_difference(Gp & a, Gp & b, Gp & sum)
{
  b.reflect();
  minkowski_sum(a, b, sum);
  b.reflect();
}


//
// Geometric difference
//

/// Geometric difference is an inverse of Minkowski sum.
/// Actually the true inverse in general case is not possible.
/// The implementation adds an outer frame to a in order to
/// perform this operation via Minkowski difference,
/// then this frame is removed, so a is not actually changed after the operation.
template <class Gp>
void geometric_difference(Gp & a, Gp & b, Gp & sum)
{
  // Compute the outer frame for a.
  using Pt = typename Gp::Point_2;
  auto abox = a.bbox_sync();
  
  typename Gp::Polygon frame;

  auto const
    x0 = abox.xmin(), x1 = abox.xmax(),
    y0 = abox.ymin(), y1 = abox.ymax();
  auto const
    dx = 0.25 * (x1 - x0), dy = 0.25 * (y1 - y0);
  auto const
    X0 = x0 - dx, X1 = x1 + dx,
    Y0 = y0 - dy, Y1 = y1 + dy;

  frame.push_back(Pt(X0, Y0));
  frame.push_back(Pt(X1, Y0));
  frame.push_back(Pt(X1, Y1));
  frame.push_back(Pt(X0, Y1));

  // Add the frame to a.
  auto & aps = a.polygon_set_modify();
  aps.symmetric_difference(frame);
  
  // Compute the difference.
  minkowski_difference(a, b, sum);

  auto & sps = sum.polygon_set_modify();
  sps.complement();
  sps.intersection(frame);
  
  // Restore a.
  aps.symmetric_difference(frame);
}

#endif//MINKOWSKI_HPP_INCLUDED_5QP48D
