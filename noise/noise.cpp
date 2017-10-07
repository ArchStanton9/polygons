//
// Created by arch on 10/7/17.
//
#include <iostream>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Line_2.h>

#include <PolygonSerializer.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Point_2<K> Point;

std::vector<Point> add_noise(Point p1, Point p2){
    std::vector<Point> result;



    return result;
}


int main() {
    PolygonSerializer<K> s;

    Polygon p = s.Deserialize("input.p");

    auto e = p.edges_begin();


    std::cout << "test" << std::endl;
    std::cin.get();
}