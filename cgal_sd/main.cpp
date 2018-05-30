// cgal_testing: Minkowski sum and geometric difference of generic polygons.
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <iterator>
#include <list>

#define ASSERTIONS_HPP_DEBUG_DEFAULT ASSERTIONS_HPP_LOG
#include "polygon_io.hpp"
#include "generic_polygon.hpp"
#include "minkowski.hpp"
#include "simplify.hpp"

// args parsing
#include <boost/program_options.hpp>

// CGAL constructions:
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

using Kernel = CGAL::Exact_predicates_exact_constructions_kernel;
using Point_2 = Kernel::Point_2;
using Polygon_2 = CGAL::Polygon_2<Kernel>;
using Polygon_with_holes_2 = CGAL::Polygon_with_holes_2<Kernel>;
using Polygon_set_2 = CGAL::Polygon_set_2<Kernel>;

// My constructions:
using Polygons_with_holes_2 = std::list<Polygon_with_holes_2>;
using Generic_polygon_2 = Generic_polygon<Polygons_with_holes_2, Polygon_set_2>;

namespace po = boost::program_options;
using namespace std;

string p_path;
string q_path;
string r_path;
string output_dir;
int precision;
int steps;
bool write_sum;
double threshold_ratio; 


Contours_io_flags const format{
	Contours_io_flag::Store_contours_size,
	Contours_io_flag::Store_vertices_size };

bool try_write_step_to_file(Generic_polygon_2 polygon, int step, string_view file_prefix) {
	std::stringstream ss;
	ss << output_dir << file_prefix << step << ".txt";
	string filepath = ss.str();
	clog << "write step result " << step << " to file " << filepath << "\n";

	ofstream file(filepath);
	if (!file.is_open()) {
		clog << "File " << filepath << " can't be open to write step result.\n";
		return false;
	}

	file.precision(precision);
    cgal_write_polygons(file, polygon.polygons_with_holes_sync());
	file.close();

    return true;
}

int run_algorithm(Generic_polygon_2 & P, Generic_polygon_2 & Q, Generic_polygon_2 & R) {
	int i = 0;
	Generic_polygon_2 S;

	try {
		for (; i < steps; i++) {
			minkowski_sum(R, P, S);
			if (write_sum && !try_write_step_to_file(S, i, "S_")) {
				return -11;
			}

			geometric_difference(S, Q, R);
            if (threshold_ratio > 1 / 10000000){
                simplify_gp(R, threshold_ratio);
            }

			if (!try_write_step_to_file(R, i, "W_")) {
				return -12;
			}
		}
	}
	catch (const std::exception &e) {
		std::cerr
			<< "Algorithm stoped at step " << i << " cause of exception:\n"
			<< e.what() << '\n';

		return -6;
	}

	return 0;
}


int main(int argc, char *argv[]) {
	try {
		
		po::options_description desc("Allowed options");
		desc.add_options()
			("help,h", "print usage message")
			("p", po::value<string>()->required(), "first polygon")
			("q", po::value<string>()->required(), "second polygon")
			("r", po::value<string>()->required(), "target set")
			("out,o", po::value<string>()->required(), "output folder")
			("steps,s", po::value<int>()->default_value(10), "Count of steps.")
			("pr", po::value<int>()->default_value(14), "Precision of contours points io")
            ("tr", po::value<double>()->default_value(0.0), "Contour simplification threshold ratio")
			("ws", po::bool_switch()->default_value(false), "Save obtained minkowski sum at every step.");

		po::variables_map vm;
		po::parsed_options parsed = po::command_line_parser(argc, argv)
			.options(desc)
			.allow_unregistered()
			.run();

		po::store(parsed, vm);

		if (vm.count("help")) {
			cout << desc << "\n";
			return 0;
		}

		p_path = vm["p"].as<string>();
		q_path = vm["q"].as<string>();
		r_path = vm["r"].as<string>();
		output_dir = vm["out"].as<string>();
		steps = vm["steps"].as<int>();
		precision = vm["pr"].as<int>();
	    write_sum = vm["ws"].as<bool>();
        threshold_ratio = vm["tr"].as<double>();

		clog 
			<< "P polygon: " << p_path << "\n"
			<< "Q polygon: " << q_path << "\n"
			<< "R target: " << q_path << "\n"
			<< "Output dir: " << output_dir << "\n"
			<< "Steps: " << steps << "\n"
			<< "Precision: " << precision << "\n"
			<< "Write sum: " << write_sum << "\n"
            << "Threshold ratio: " << threshold_ratio << "\n";
	}
	catch (exception& e) {
		cerr << e.what() << "\n";
		return -1;
	}

	// Open input files.

	ifstream P_file(p_path);
	if (!P_file.is_open()) {
		clog << "File " << p_path << " can't be open.\n";
		return -2;
	}

	ifstream Q_file(q_path);
	if (!Q_file.is_open()) {
		clog << "File " << q_path << " can't be open.\n";
		return -3;
	}

	ifstream R_file(r_path);
	if (!R_file.is_open()) {
		clog << "File " << r_path << " can't be open.\n";
		return -4;
	}

	// Read the input.
	// Polygon format:
	//   https://doc.cgal.org/latest/Polygon/classCGAL_1_1Polygon__with__holes__2.html#a4b6117c3559e09dcdfa908746984c9bc
	//   The format consists of the number of points of the outer boundary
	//   followed by the points themselves in counterclockwise order,
	//   followed by the number of holes, and for each hole,
	//   the number of points of the outer boundary is followed by the points themselves in clockwise order.

	Generic_polygon_2 P, Q, R;
	cgal_read_polygons<Polygon_with_holes_2>(
		P_file,
		std::back_inserter(P.polygons_with_holes_modify()));
	P_file.close();

	cgal_read_polygons<Polygon_with_holes_2>(
		Q_file,
		std::back_inserter(Q.polygons_with_holes_modify()));
	Q_file.close();

	cgal_read_polygons<Polygon_with_holes_2>(
		R_file,
		std::back_inserter(R.polygons_with_holes_modify()));
	R_file.close();

    if (threshold_ratio > 1 / 10000000){
        simplify_gp(R, threshold_ratio);
    }

	return run_algorithm(P, Q, R);
}
