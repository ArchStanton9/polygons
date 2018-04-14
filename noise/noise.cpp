#include <iostream>
#include <vector>
#include <random>
#include <math.h>

#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polygon_2.h>

#include <PolygonSerializer.hpp>

typedef CGAL::Simple_cartesian<double> K;
typedef CGAL::Polygon_2<K> Polygon;
typedef CGAL::Point_2<K> Point;

class LineFunctor
{
public:
	LineFunctor(double a, double b) {
		this->a = a;
		this->b = b;

		// no disturbance by default
		this->disturbance = [](double x) { return 0.0; };
	};

	double a, b;
	double(*disturbance) (double);

	double operator()(double x) {
		return a * x + b + disturbance(x);
	}
};

double sin_dist(double x) {
	return std::sin(x) * 0.5;
}

// restore line laying on two given points 
LineFunctor get_line(Point p1, Point p2) {
	double a = (p2.y() - p1.y()) / (p2.x() - p1.x());
	double b = p1.y() - p1.x() * a;

	return LineFunctor(a, b);
}

std::random_device rd;
std::mt19937 eng(rd());

std::list<double> get_random_numbers(size_t size, double min, double max) {
	std::uniform_real_distribution<> distr(min, max);
	std::list<double> result;

	for (size_t i = 0; i < size; i++)
	{
		result.push_back(distr(eng));
	}

	return result;
}

// tranform straight line to angled line
std::vector<Point> add_noise(Point p1, Point p2) {
	double min = std::min(p1.x(), p2.x());
	double max = std::max(p1.x(), p2.x());

	LineFunctor f = get_line(p1, p2);
	f.disturbance = sin_dist;

	// todo calculate count based on length of interval
	std::list<double> numbers = get_random_numbers(25, min, max);

	p2.x() < p1.x()
		? numbers.sort(std::greater<double>())
		: numbers.sort(std::less<double>());

	std::vector<Point> result;
	result.push_back(p1);
	for(auto x : numbers)
	{
		result.push_back(Point(x, f(x)));
	}
	result.push_back(p2);

	return result;
}

Polygon add_noise(Polygon polygon) {
	Polygon result;

	for (auto i = polygon.vertices_begin(); i < polygon.vertices_end() - 1;)
	{
		auto p1 = *i++;
		auto p2 = *i;

		auto noize = add_noise(p1, p2);
		for (auto p = noize.begin(); p < noize.end(); p++)
		{
			result.push_back(*p);
		}
	}

	// make closure
	auto last = polygon.vertices_end() - 1;
	auto first = polygon.vertices_begin();

	auto noize = add_noise(*last, *first);
	for (auto p = noize.begin(); p < noize.end(); p++)
	{
		result.push_back(*p);
	}

	return result;
}

int main() {
	PolygonSerializer<K> sz;

	Polygon inP = sz.Deserialize("input.p");
	Polygon outP = add_noise(inP);

	sz.Serialize("output.p", outP);
}
