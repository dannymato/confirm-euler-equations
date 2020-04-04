// CGAssignment2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "ImportObj.h"
#include "Math.h"
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_items_3.h>
#include <CGAL/HalfedgeDS_list.h>
#include <CGAL/Polyhedron_3.h>

#include <iostream>
#include <vector>
#include <unordered_set>
#include <Windows.h>

typedef CGAL::Simple_cartesian<double> Kernel;
typedef Kernel::Point_3 Point;
typedef CGAL::Polyhedron_3<Kernel,
	CGAL::Polyhedron_items_3,
	CGAL::HalfedgeDS_list> Polyhedron;
typedef Polyhedron::Vertex_iterator Vertex_iterator;
typedef Polyhedron::Halfedge_around_vertex_circulator Vertex_circulator;
typedef Polyhedron::Halfedge_handle Halfedge_handle;
typedef std::vector<double> vertex;
typedef std::vector<vertex> Vector;



int main()
{
	Polyhedron poly;
	importOBJ("assets\\dino.obj", &poly);

	const int numVertices = poly.size_of_vertices();
	int numFaces = poly.size_of_facets();
	int numEdges = poly.size_of_halfedges() / 2;
	
	const int genus = 0;

	std::cout << "Vertices: " << numVertices << std::endl;
	std::cout << "Faces: " << numFaces << std::endl;
	std::cout << "Edges: " << numEdges << std::endl;
	std::cout << "Genus: " << genus << std::endl;

	std::cout << "V + F - E = " << numVertices + numFaces - numEdges << std::endl;
	std::cout << "2 - 2g = " << 2 - 2 * genus << std::endl;

	Vertex_iterator iter = poly.vertices_begin();

	Vector vertices;

	for (; iter != poly.vertices_end(); iter++) {
		Vertex_circulator curr = iter->vertex_begin();
		Vertex_circulator next = iter->vertex_begin();
		next++;

		vertex new_vert;

		do {
			Halfedge_handle hand_curr = curr->opposite();
			Halfedge_handle hand_next = next->opposite();

			Point vec1 = difference(iter->point(), hand_curr->vertex()->point());
			Point vec2 = difference(iter->point(), hand_next->vertex()->point());
			
			double angle = calc_angle(vec1, vec2);

			new_vert.push_back(angle);
			++next;

		} while (++curr != iter->vertex_begin());

		vertices.push_back(new_vert);
	}

	double curvature = 0.0;

	for (Vector::iterator it = vertices.begin(); it != vertices.end(); ++it) {

		double angle_sum = 0.0f;
		for (std::vector<double>::iterator i = it->begin(); i != it->end(); ++i) {

			angle_sum += *i;

		}

		curvature += (2 * CGAL_PI) - angle_sum;

	}

	SetConsoleOutputCP(CP_UTF8);

	setvbuf(stdout, nullptr, _IOFBF, 1000);

	std::cout << u8"Σ Kv = " << curvature << std::endl;

	double x = (double) (numVertices + numFaces - numEdges);

	std::cout << u8"2\u03C0\u03C7 = " << 2 * CGAL_PI * x << std::endl;

	std::cout << "Press enter to exit" << std::endl;

	std::cin.get();

	return 0;

}

