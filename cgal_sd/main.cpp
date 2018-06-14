// cgal_testing: Minkowski sum and geometric difference of generic polygons.
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cmath>
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
using Simplify = Polygon_simplify<Kernel>;

namespace po = boost::program_options;
using namespace std;

string p_path;
string q_path;
string m_path;
string output_dir;
int precision;
int steps;
double dt;
bool write_sum;
bool keep_shape;
double threshold_ratio; 


Contours_io_flags const format{
	Contours_io_flag::Store_contours_size,
	Contours_io_flag::Store_vertices_size };

bool try_write_step_to_file(Generic_polygon_2 polygon, int step, string_view file_prefix) {
	std::stringstream ss;
	ss << output_dir << file_prefix << std::setw(5) << std::setfill('0') << step << ".txt";
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

int run_algorithm(Generic_polygon_2 & P, Generic_polygon_2 & Q, Generic_polygon_2 & M) {
	int i = 0;
	Generic_polygon_2 S;
    
    if (std::abs(dt - 1.0) > std::pow(0.1, 10))
    {
        clog << "scale P and Q with factor " << dt << "\n";
        P.scale(dt);
        Q.scale(dt);
    }

    if (threshold_ratio > 1 / 10000000) {
        clog << "simplify input polygons with threshold ratio " << threshold_ratio << "\n";

        simplify_gp(P, threshold_ratio);
        simplify_gp(Q, threshold_ratio);

        if (keep_shape) {
            Simplify simplify;
            Generic_polygon_2 plain_M;

            simplify(M, plain_M,
            CGAL::Polyline_simplification_2::Scaled_squared_distance_cost{},
            CGAL::Polyline_simplification_2::Stop_above_cost_threshold{threshold_ratio});
    
            M = plain_M;   
        } 
        else {
            simplify_gp(M, threshold_ratio);
        }
    }

    try_write_step_to_file(M, 0, "W_");

	try {
        P.reflect();

		for (; i < steps; i++) {
			minkowski_sum(M, P, S);
			if (write_sum && !try_write_step_to_file(S, i + 1, "S_")) {
				return -11;
			}

			geometric_difference(S, Q, M);

            // if (threshold_ratio > 1 / 10000000){
            //     simplify_gp(M, threshold_ratio);
            // }

			if (!try_write_step_to_file(M, i + 1, "W_")) {
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
			("m", po::value<string>()->required(), "target set")
			("out,o", po::value<string>()->required(), "output folder")
			("steps,s", po::value<int>()->default_value(10), "Count of steps.")
            ("dt", po::value<double>()->default_value(1.0), "Time delta")
			("pr", po::value<int>()->default_value(14), "Precision of contours points io")
            ("tr", po::value<double>()->default_value(0.0), "Contour simplification threshold ratio")
            ("ks", po::bool_switch()->default_value(false), "Keep shape of polyline set while simplification.")
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
		m_path = vm["m"].as<string>();
		output_dir = vm["out"].as<string>();
		steps = vm["steps"].as<int>();
        dt = vm["dt"].as<double>();
		precision = vm["pr"].as<int>();
	    write_sum = vm["ws"].as<bool>();
        keep_shape = vm["ks"].as<bool>();
        threshold_ratio = vm["tr"].as<double>();

		clog 
			<< "P polygon: " << p_path << "\n"
			<< "Q polygon: " << q_path << "\n"
			<< "M target: " << m_path << "\n"
			<< "Output dir: " << output_dir << "\n"
			<< "Steps: " << steps << "\n"
            << "Delta t: " << dt << "\n"
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

	ifstream M_file(m_path);
	if (!M_file.is_open()) {
		clog << "File " << m_path << " can't be open.\n";
		return -4;
	}

	// Read the input.
	// Polygon format:
	//   https://doc.cgal.org/latest/Polygon/classCGAL_1_1Polygon__with__holes__2.html#a4b6117c3559e09dcdfa908746984c9bc
	//   The format consists of the number of points of the outer boundary
	//   followed by the points themselves in counterclockwise order,
	//   followed by the number of holes, and for each hole,
	//   the number of points of the outer boundary is followed by the points themselves in clockwise order.

	Generic_polygon_2 P, Q, M;
	cgal_read_polygons<Polygon_with_holes_2>(
		P_file,
		std::back_inserter(P.polygons_with_holes_modify()));
	P_file.close();

	cgal_read_polygons<Polygon_with_holes_2>(
		Q_file,
		std::back_inserter(Q.polygons_with_holes_modify()));
	Q_file.close();

	cgal_read_polygons<Polygon_with_holes_2>(
		M_file,
		std::back_inserter(M.polygons_with_holes_modify()));
	M_file.close();

	return run_algorithm(P, Q, M);
}
