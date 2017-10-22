#include <fstream>
#include <vector>
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/point_generators_2.h>

typedef CGAL::Simple_cartesian<double>			 K;
typedef CGAL::Delaunay_triangulation_2<K>		 Triangulation;
typedef K::Triangle_2							 Triangle;
typedef Triangulation::Point                     Point;
typedef CGAL::Random_points_in_triangle_2<Point> Generator;

int main() {
	Triangle triangle(Point(0.0, 0.0), Point(50.0, 0.0), Point(0.0, 50.0));
	std::vector<Point> points;
	std::copy_n(Generator(triangle), 55, std::back_inserter(points));
	
	std::stringstream st;
	for each (auto p in points) {
		st << p.x() << " " << p.y() << std::endl;
	}

	std::ofstream out("result.tr");
	std::istream_iterator<Point> begin(st);
	std::istream_iterator<Point> end;
	Triangulation t;
	t.insert(begin, end);
	
	std::cout << t.number_of_vertices() << std::endl;

	for (auto f = t.faces_begin(); f < t.faces_end(); f++) {
		auto p1 = f->vertex(0);
		auto p2 = f->vertex(1);
		auto p3 = f->vertex(2);

		out << p1->point().x() << " " << p1->point().y() << std::endl;
		out << p2->point().x() << " " << p2->point().y() << std::endl;
		out << p3->point().x() << " " << p3->point().y() << std::endl;
		out << p1->point().x() << " " << p1->point().y() << std::endl;
		out << std::endl;
	}

	return 0;
}