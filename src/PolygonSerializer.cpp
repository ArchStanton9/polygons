//
// Created by arch on 9/9/17.
//

#include "PolygonSerializer.h"

using namespace std;

void PolygonSerializer::Serialize(std::string file_path, Polygon polygon) {
    ofstream out(file_path);
    for (auto i = polygon.vertices_begin(); i != polygon.vertices_end(); i++) {
        out << i->x() << " " << i->y() << std::endl;
    }

    out.close();
    cout << "Polygon saved in file: " << file_path << std::endl;
}

Polygon PolygonSerializer::Deserialize(std::string file_path) {
    fstream stream(file_path);
    stream.flags(ios_base::skipws);

    Polygon p;
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

        p.push_back(Point(x, y));
    }

    cout << "Load polygon from file: " << file_path << endl;

    return p;
}