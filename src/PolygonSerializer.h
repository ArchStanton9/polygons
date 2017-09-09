//
// Created by arch on 9/9/17.
//

#ifndef POLYGONS_POLYGONSERIALIZER_H
#define POLYGONS_POLYGONSERIALIZER_H


#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Point_2<K> Point;

class PolygonSerializer {

public:
    // Allow to save polygon in file
    void Serialize(std::string file_path, Polygon polygon);

    // Allow to read polygon from file
    Polygon Deserialize(std::string file_path);
};


#endif //POLYGONS_POLYGONSERIALIZER_H
