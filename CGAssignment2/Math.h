#pragma once

#include <CGAL/Simple_cartesian.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;

double calc_angle(Point, Point);
double dot_product(Point, Point);
double magnitude(Point);
Point normalize(Point);
Point difference(Point, Point);

double calc_angle(Point vec1, Point vec2) {

	Point norm_vec1 = normalize(vec1);
	Point norm_vec2 = normalize(vec2);

	return acos(dot_product(norm_vec1, norm_vec2));

}

double dot_product(Point vec1, Point vec2) {
	return (vec1.x() * vec2.x()) + (vec1.y() * vec2.y()) + (vec1.z() * vec2.z());
}

double magnitude(Point vec) {
	double x = vec.x();
	double y = vec.y();
	double z = vec.z();

	return sqrt((x * x) + (y * y) + (z * z));

}

Point normalize(Point vec) {

	double mag = magnitude(vec);

	return Point(vec.x() / mag, vec.y() / mag, vec.z() / mag);
}

Point difference(Point source, Point destination) {

	double x = destination.x() - source.x();
	double y = destination.y() - source.y();
	double z = destination.z() - source.z();

	return Point(x, y, z);

}