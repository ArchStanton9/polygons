// generic_polygon.hpp
// The code is based upon CGAL.
// "Generic polygon" here is a combination of a container with
// "polygons with holes" (Ph) and a "polygon set" (Ps) based upon Arrangement_2.
// These representations are actually independent thus we need to
// synchronize them when one is changed and we need another one.
// TODO: from contours -- we need to implement winding rule over Arrangement_2.
#ifndef GENERIC_POLYGON_HPP_INCLUDED_0PM75C
#define GENERIC_POLYGON_HPP_INCLUDED_0PM75C

#include <type_traits>
#include <utility>

#include <CGAL/Bbox_2.h>

#include "assertions.hpp"
#include "iterator.hpp"


//
// Auxiliaries
//

// Dirty const-removing hackery.
// I don't feel sorry though :)

/// This function applies f for each point, points may be changed in-place.
/// The arrangement structure is left unchanged, that means that
/// the function being applied must be continuous on the plane and save orientation.
template <class Ps, class F>
void ps_hackery_for_each_point(Ps & pset, F f)
{
  using Ar = typename Ps::Arrangement_2;
  using Vr = typename Ar::Vertex;
  using Pt = typename Ar::Point_2;
  
  Ar & ar = const_cast<Ar&>(pset.arrangement());
  auto const e = ar.vertices_end();
  for (auto vp = ar.vertices_begin(); vp != e; ++vp)
  {
    if (!vp->is_at_open_boundary())
      f(const_cast<Pt&>(vp->point()));
  }
}

/// Multiply each coordinate by -1.
template <class Ps>
void ps_hackery_negate(Ps & pset)
{
  ps_hackery_for_each_point(pset, 
    [](auto & p) { p = CGAL::ORIGIN + (CGAL::ORIGIN - p); });
}

template <class Ps>
void ps_hackery_scale(Ps &pset, double factor)
{
  using Pt = typename Ps::Polygon_2::Point_2;
  ps_hackery_for_each_point(pset,
    [factor](auto &p) { p = Pt(p.x() * factor, p.y() * factor); });
}

/// This function applies f for each point, points may be changed in-place.
/// The arrangement structure is left unchanged, that means that
/// the function being applied must be continuous on the plane and save orientation.
template <class Polygon, class F>
void p2_hackery_for_each_point(Polygon & p, F f)
{
  using Pt = typename Polygon::Point_2;
  auto const e = p.vertices_end();
  for (auto vp = p.vertices_begin(); vp != e; ++vp)
    f(const_cast<Pt&>(*vp));
}

/// This function applies f for each point, points may be changed in-place.
/// The arrangement structure is left unchanged, that means that
/// the function being applied must be continuous on the plane and save orientation.
template <class Ph, class F>
void ph_hackery_for_each_point(Ph & pwh, F f)
{
  p2_hackery_for_each_point(pwh.outer_boundary(), f);
  auto const e = pwh.holes_end();
  for (auto hp = pwh.holes_begin(); hp != e; ++hp)
    p2_hackery_for_each_point(*hp, f);
}

/// Multiply each coordinate by -1.
template <class Ph>
void ph_hackery_negate(Ph & pwh)
{
  ph_hackery_for_each_point(pwh, 
    [](auto & p) { p = CGAL::ORIGIN + (CGAL::ORIGIN - p); });
}

template <class Ph>
void ph_hackery_scale(Ph &pwh, double factor)
{
  using Pt = typename Ph::Polygon_2::Point_2;
  ph_hackery_for_each_point(pwh,
    [factor](auto &p) { p = Pt(p.x() * factor, p.y() * factor); });
}

//
// Generic polygon class
//

template <class PhContainer, class Ps>
class Generic_polygon
{
public:

  using Polygon_set = Ps;
  using Polygons_with_holes = PhContainer;
  using Polygon_with_holes = typename PhContainer::value_type;

  using Polygon = typename Polygon_with_holes::General_polygon_2;
  using Point_2 = typename Polygon::Point_2;
  
  static_assert(
    std::is_same_v<
      Point_2,
      typename Polygon_set::Arrangement_2::Point_2>);
  
  Generic_polygon() = default;
  Generic_polygon(Generic_polygon const &) = default;
  Generic_polygon(Generic_polygon &&) = default;
  Generic_polygon& operator=(Generic_polygon const &) = default;
  Generic_polygon& operator=(Generic_polygon &&) = default;
  
  Generic_polygon(Polygons_with_holes && ph)
    : _ph(std::move(ph)) { _ph_changed = true; }
  
  Generic_polygon(Polygons_with_holes const & ph)
    : _ph(ph) { _ph_changed = true; }
  
  Generic_polygon(Polygon_with_holes && ph)
  {
    _ph.emplace_back(std::move(ph));
    _ph_changed = true;
  }
  
  Generic_polygon(Polygon_with_holes const & ph)
  {
    _ph.emplace_back(ph);
    _ph_changed = true;
  }
  
  Generic_polygon(Polygon_set && ps)
    : _ps(std::move(ps)) { _ps_changed = true; }

  Generic_polygon(Polygon_set const & ps)
    : _ps(ps) { _ps_changed = true; }
  
 
  Polygons_with_holes const & polygons_with_holes_unsync() const
  {
    REQUIRE_FAST(!_ph_changed);
    return _ph;
  }
  
  Polygon_set const & polygon_set_unsync() const
  {
    REQUIRE_FAST(!_ps_changed);
    return _ps;
  }
  
  CGAL::Bbox_2 const & bbox_unsync() const
  {
    REQUIRE_FAST(_bbox_ready);
    return _bbox;
  }
  

  Polygons_with_holes const & polygons_with_holes_sync()
  {
    _update_ph();
    return _ph;
  }
  
  Polygon_set const & polygon_set_sync()
  {
    _update_ps();
    return _ps;
  }
  
  CGAL::Bbox_2 const & bbox_sync()
  {
    _update_bbox();
    return _bbox;
  }
  
  
  Polygons_with_holes & polygons_with_holes_modify()
  {
    _update_ph();
    _ph_changed = true;
    _bbox_ready = false;
    return _ph;
  }
  
  Polygon_set & polygon_set_modify()
  {
    _update_ps();
    _ps_changed = true;
    _bbox_ready = false;
    return _ps;
  }
  
  
  void clear()
  {
    _ph.clear();
    _ps.clear();
    _ph_changed = false;
    _ps_changed = false;
    
    _bbox = CGAL::Bbox_2{};
    _bbox_ready = true;
  }
  
  
  void reflect()
  {
    if (_ph_changed || !(_ph_changed || _ps_changed))
    {
      for (auto & p: _ph)
        ph_hackery_negate(p);
    }
    
    if (_ps_changed || !(_ph_changed || _ps_changed))
    {
      ps_hackery_negate(_ps);
    }
    
    if (_bbox_ready)
    {
      _bbox = CGAL::Bbox_2
        (
          -_bbox.xmax(), -_bbox.ymax(),
          -_bbox.xmin(), -_bbox.ymin()
        );
    }
  }

  void scale(double factor)
  {
    if (_ph_changed || !(_ph_changed || _ps_changed))
    {
      for (auto & p: _ph)
        ph_hackery_scale(p, factor);
    }
    
    if (_ps_changed || !(_ph_changed || _ps_changed))
    {
      ps_hackery_scale(_ps, factor);
    }
    
    if (_bbox_ready)
    {
      _bbox = CGAL::Bbox_2
        (
          _bbox.xmax() * factor, _bbox.ymax() * factor,
          _bbox.xmin() * factor, _bbox.ymin() * factor
        );
    }
  }
  
private:

  Polygons_with_holes _ph;
  Polygon_set _ps;

  CGAL::Bbox_2 _bbox;

  bool _ph_changed = false;
  bool _ps_changed = false;
  bool _bbox_ready = false;
  
  void _update_ps()
  {
    REQUIRE_FAST(!_ph_changed || !_ps_changed);
    if (_ph_changed)
    {
      _ps.clear();
      for (auto & p: _ph)
        _ps.join(p);
      _ph_changed = false;
    }
  }
  
  void _update_ph()
  {
    REQUIRE_FAST(!_ph_changed || !_ps_changed);
    if (_ps_changed)
    {
      _ph.clear();
      _ps.polygons_with_holes(std::back_inserter(_ph));
      _ps_changed = false;
    }
  }
  
  void _update_bbox()
  {
    if (!_bbox_ready)
    {
      _update_ph();
      _bbox = CGAL::Bbox_2{};
      for (auto & p: _ph)
        _bbox += p.bbox();
      _bbox_ready = true;
    }
  }
};


#endif//GENERIC_POLYGON_HPP_INCLUDED_0PM75C
