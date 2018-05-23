// polygon_io.hpp
/**
 * @file polygon_io.hpp
 * @author Kuvshinov D.R.
 * @brief Read/write CGAL polygon with holes.
 * 
 * Formats (text):
 * 
 * 1. CGAL:
 *    * file -> components_size:int  components:array<component, components_size>
 *    * component -> outer_boundary:contour  holes_size:int  holes:array<contour, holes_size>
 *    * contour -> vertices_size:int  vertices:array<vertex, vertices_size>
 *    * vertex -> x:float  y:float
 *    * any delimiter -> any space
 * 
 * 2. Contours (sizes are optional -- controlled via two boolean flags):
 *    * file -> [contours_size:int]  contours:array<contour [, contours_size]>
 *    * contour -> [vertices_size:int]  vertices:array<vertex [, vertices_size]>
 *    * vertex -> x:float  y:float
 *    * if no vertices_size then contours delimiter -> empty line
 *    * any (other) delimiter -> any space
 * 
 */
#ifndef POLYGON_IO_HPP_INCLUDED_KZ43L9
#define POLYGON_IO_HPP_INCLUDED_KZ43L9

#include <iterator>
#include <istream>
#include <ostream>
#include <iomanip>

#include "bit_flag_type.hpp"


//
// Reading CGAL format.
//

/// Read vertices_size and vertices.
template <class Contour>
std::istream& cgal_read_contour
  (std::istream & input, Contour & contour)
{
  std::size_t vertices = 0;
  if (input >> vertices)
  {
    contour.clear();
    typename Contour::FT x, y;
    while (vertices-- && (input >> x >> y))
      contour.push_back({x, y});
  }
  
  return input;
}


/// Read one polygon with holes in CGAL format.
template <class PolygonWithHoles>
std::istream& cgal_read_polygon
  (std::istream & input, PolygonWithHoles & polygon)
{
  polygon.clear();
  if (!cgal_read_contour(input, polygon.outer_boundary()))
    return input;
  
  std::size_t holes = 0;
  if (input >> holes)
  {
    while (holes--)
    {
      polygon.add_hole(typename PolygonWithHoles::General_polygon_2{});
      auto it = std::prev(polygon.holes_end());
      if (!cgal_read_contour(input, *it))
        break;
    }
  }
  
  return input;
}


/// Read the data using CGAL format.
template <class PolygonWithHoles, class OutPolyIt>
OutPolyIt cgal_read_polygons
  (std::istream & input, OutPolyIt out)
{
  std::size_t components = 0;
  if (input >> components)
  {
    PolygonWithHoles current;
    while (components-- && cgal_read_polygon(input, current))
      *out++ = std::move(current);
  }
  
  return out;
}


//
// Writing CGAL format.
//

/// Write a contour using CGAL format.
template <class Contour>
std::ostream& cgal_write_contour
  (std::ostream & output, Contour const & contour)
{
  if (output << contour.size() << "\n\n")
  {
    auto p = contour.vertices_begin();
    auto const e = contour.vertices_end();
    while (p != e && 
      (output 
        << std::setw(24) << p->x() 
        << std::setw(24) << p->y() << '\n'))
      ++p;      
  }
  return output << '\n';
}


/// Write a polygon with holes using CGAL format.
template <class PolygonWithHoles>
std::ostream& cgal_write_polygon
  (std::ostream & output, PolygonWithHoles const & polygon)
{
  if (cgal_write_contour(output, polygon.outer_boundary())
    && output << polygon.number_of_holes() << "\n")
  {
    auto p = polygon.holes_begin();
    auto const e = polygon.holes_end();
    while (p != e && cgal_write_contour(output, *p))
      ++p;
  }
  return output << '\n';
}


/// Write the data using CGAL format.
template <class FwdPolyIt>
std::ostream& cgal_write_polygons
  (std::ostream & output, FwdPolyIt from, FwdPolyIt to)
{
  if (output << std::distance(from, to) << "\n\n")
    while (from != to && cgal_write_polygon(output, *from))
      ++from;
  return output << '\n';
}


/// Write the data using CGAL format.
template <class PolyContainer> inline
std::ostream& cgal_write_polygons
  (std::ostream & output, PolyContainer const & polygons)
{
  using std::begin;
  using std::end;
  return cgal_write_polygons(output, begin(polygons), end(polygons));
}



/// Contours format flags
/**
 * If Read_contours_size is set then contours_size is given in the input,
 * otherwise we read contours until the input fails.
 * 
 * If Read_vertices_size is set then vertices_size is given in the input
 * for each contour, otherwise we read vertices until an empty line (or input failure).
 */
enum class Contours_io_flag
{
  Store_contours_size,
  Store_vertices_size
};

using Contours_io_flags = Bit_flags<Contours_io_flag>;


//
// Reading Contours format.
//

/// The result of the skip_until_nl.
enum class Skip_until_nl
{
  Eof,
  Nl,
  Other
};


/// Skip all whitespace characters except newline.
inline Skip_until_nl skip_until_nl(std::istream & input)
{
  for (char ch; input.get(ch);)
  {
    if (ch == '\n')
      return Skip_until_nl::Nl;
    if (!std::isspace(ch, input.getloc()))
    {
      input.unget();
      return Skip_until_nl::Other;
    }
  }
  
  return Skip_until_nl::Eof;
}


/// Read vertices_size and vertices.
template <class Contour>
std::istream& contours_read_contour
  (std::istream & input, Contour & contour, Contours_io_flags format)
{
  if (format[Contours_io_flag::Store_vertices_size])
    return cgal_read_contour(input, contour); // no difference here.
  
  contour.clear();
  input >> std::ws;
  for (typename Contour::FT x, y;;)
  {
    switch (skip_until_nl(input))
    {
    case Skip_until_nl::Other:
      if (input >> x >> y)
        contour.push_back({x, y});
      else
        return input;
      break;
      
    case Skip_until_nl::Nl:
    case Skip_until_nl::Eof:
      return input;
    }
  }
  
  return input;
}


/// Read a sequence of contours in Contours format.
template <class Contour, class ContoursOutIt>
ContoursOutIt contours_read
  (std::istream & input, ContoursOutIt out, Contours_io_flags format)
{
  std::size_t max_contours = -1;
  if (!format[Contours_io_flag::Store_contours_size]
    || (input >> max_contours))
  {
    Contour contour;
    while (max_contours-- && contours_read_contour(input, contour, format))
      *out++ = std::move(contour);
  }
  
  return input;
}


//
// Writing Contours format.
//

/// Write a contour using Contours format.
template <class Contour>
std::ostream& contours_write_contour
  (std::ostream & output, Contour const & contour, Contours_io_flags format)
{
  if (!format[Contours_io_flag::Store_vertices_size] 
    || output << contour.size() << "\n\n")
  {
    auto p = contour.vertices_begin();
    auto const e = contour.vertices_end();
    while (p != e && 
      output 
        << std::setw(24) << p->x() 
        << std::setw(24) << p->y() << '\n')
      ++p;
  }
  return output << '\n';
}


/// Write a sequence of contours.
template <class ContourFwdIt>
std::ostream& contours_write
  (std::ostream & output, ContourFwdIt from, ContourFwdIt to, Contours_io_flags format)
{
  if (!format[Contours_io_flag::Store_contours_size]
    || (output << std::distance(from, to) << "\n\n"))
  {
    while (from != to && contours_write_contour(output, *from, format))
      ++from;
  }
  return output;
}


/// Write a polygon with holes using Contours format.
template <class PolygonWithHoles>
std::ostream& contours_write_polygon
  (std::ostream & output, PolygonWithHoles const & polygon, Contours_io_flags format)
{
  if (contours_write_contour(output, polygon.outer_boundary(), format))
  {
    auto p = polygon.holes_begin();
    auto const e = polygon.holes_end();
    while (p != e && contours_write_contour(output, *p, format))
      ++p;
  }
  return output << '\n';
}


/// Write the data using Contours format.
template <class FwdPolyIt>
std::ostream& contours_write_polygons
  (std::ostream & output, FwdPolyIt from, FwdPolyIt to, Contours_io_flags format)
{
  if (format[Contours_io_flag::Store_contours_size])
  {
    std::size_t contours = 0;
    for (auto p = from; p != to; ++p)
      contours += 1 + p->number_of_holes();
    output << contours << "\n\n";
  }
  
  while (from != to && contours_write_polygon(output, *from, format))
    ++from;
  return output << '\n';
}


/// Write the data using Contours format.
template <class PolyContainer> inline
std::ostream& contours_write_polygons
  (std::ostream & output, PolyContainer const & polygons, Contours_io_flags format)
{
  using std::begin;
  using std::end;
  return contours_write_polygons(output, begin(polygons), end(polygons), format);
}

#endif//POLYGON_IO_HPP_INCLUDED_KZ43L9
