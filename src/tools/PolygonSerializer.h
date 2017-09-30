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

template <class Kernel>
class PolygonSerializer {

public:
    // Allow to save polygon in file
    void Serialize(std::string file_path, CGAL::Polygon_2<Kernel> polygon);
    void Serialize(std::string file_path, CGAL::Polygon_with_holes_2<Kernel> pwh);

    // Allow to read polygon from file
    CGAL::Polygon_2<Kernel> Deserialize(std::string file_path);
};

using namespace std;

template <class Kernel>
void PolygonSerializer<Kernel>::Serialize(std::string file_path, CGAL::Polygon_2<Kernel> polygon) {
    ofstream out(file_path);
    for (auto i = polygon.vertices_begin(); i != polygon.vertices_end(); i++) {
        out << i->x() << " " << i->y() << std::endl;
    }

    out.close();
    cout << "Polygon saved in file: " << file_path << std::endl;
}


template <class Kernel>
void PolygonSerializer<Kernel>::Serialize(std::string file_path, CGAL::Polygon_with_holes_2<Kernel> pwh) {
    ofstream out(file_path);

    CGAL::Polygon_2<Kernel> poly = pwh.outer_boundary();
    for (auto i = poly.vertices_begin(); i != poly.vertices_end(); i++) {
        out << i->x() << " " << i->y() << std::endl;
    }

    out.close();
    cout << "Polygon with holes saved in file: " << file_path << std::endl;
}





template <class Kernel>
CGAL::Polygon_2<Kernel> PolygonSerializer<Kernel>::Deserialize(std::string file_path) {
    fstream stream(file_path);
    stream.flags(ios_base::skipws);

    CGAL::Polygon_2<Kernel> p;
    string line;

    while (getline(stream, line)) {
        istringstream iss(line);
        vector<string> tokens;

        // split line by space character
        copy(istream_iterator<string>(iss),
             istream_iterator<string>(),
             back_inserter(tokens));

        if (tokens.size() != 2) {
            throw invalid_argument("Incorrect file format");
        }

        // parse and add point to polygon vertices
        int x = stoi(tokens[0]);
        int y = stoi(tokens[1]);

        p.push_back(CGAL::Point_2<Kernel> (x, y));
    }

    cout << "Load polygon from file: " << file_path << endl;

    return p;
}

#endif //POLYGONS_POLYGONSERIALIZER_H
