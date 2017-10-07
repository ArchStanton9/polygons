//
// Created by arch on 10/7/17.
//

#include <iostream>
#include <vector>
#include <tuple>
#include <random>
#include <math.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/Line_2.h>
#include <CGAL/squared_distance_2.h>

#include <PolygonSerializer.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Point_2<K> Point;

class LineFunctor
{
public:
	LineFunctor(double a, double b) {
		this->a = a;
		this->b = b;
		this->dist = [](double x) { return 0.0; };
	};

	double a, b;
	double (*dist) (double);

	double operator()(double x) {
		return a * x + b + dist(x);
	}
};

double sin_dist(double x) {
	return std::sin(x * 100 ) * 0.5;
}

LineFunctor get_line(Point p1, Point p2) {
	double a = (p2.y() - p1.y()) / (p1.x() + p2.x());
	double b = p1.y() + p1.x() * a;

	return LineFunctor(a, b);
}

std::list<double> get_random_numbers(size_t size, double min, double max) {
	std::random_device rd;
	std::mt19937 eng(rd());
	std::uniform_real_distribution<> distr(min, max);

	std::list<double> result;

	for (size_t i = 0; i < size; i++)
	{
		result.push_back(distr(eng));
	}

	return result;
}

std::vector<Point> add_noise(Point p1, Point p2){
    std::vector<Point> result;
	result.push_back(p1);

	double min = p1.x() < p2.x() ? p1.x() : p2.x();
	double max = p1.x() > p2.x() ? p1.x() : p2.x();

	LineFunctor f = get_line(p1, p2);
	f.dist = sin_dist;

	std::list<double> numbers = get_random_numbers(3, min, max);
	numbers.sort();

	for each (auto x in numbers)
	{
		result.push_back(Point(x, f(x)));
	}

	result.push_back(p2);

    return result;
}

int main() {
	PolygonSerializer<K> sz;
	
	Point p1(0.0, 4.0);
	Point p2(2.0, 0.0);

	Polygon inP;
	inP.push_back(p1);
	inP.push_back(p2);
	sz.Serialize("input.p", inP);

	auto v = add_noise(p1, p2);

	Polygon outP;
	for each (Point point in v)
	{
		outP.push_back(point);
	}

	sz.Serialize("output.p", outP);
}

