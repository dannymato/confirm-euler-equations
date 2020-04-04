#pragma once

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Modifier_base.h>
#include <CGAL/exceptions.h>

#include <string>
#include <fstream>
#include <exception>

template<class HDS>
class BuildCgalPolyheadronFromObj : public CGAL::Modifier_base<HDS> {
public:
	BuildCgalPolyheadronFromObj(const std::string& fileName) : mFilename(fileName) {}

	void operator() (HDS& hds) {

		typedef typename HDS::Vertex Vertex;
		typedef typename Vertex::Point Point;

		std::ifstream file(mFilename.c_str());
		if (!file) {
			return;
		}

		std::string line;
		int numVertices = 0;
		int numFacets = 0;

		while (file.good()) {
			std::getline(file, line);
			if (line.size() > 1) {
				if (line[0] == 'v' && line[1] == ' ') { ++numVertices; }
				if (line[0] == 'f' && line[1] == ' ') { ++numFacets; } 
			}
		}

		if (!file.good()) {
			file.clear();
		}

		file.seekg(0);

		CGAL::Polyhedron_incremental_builder_3<HDS> B(hds, true);

		B.begin_surface(numVertices, numFacets, int((numVertices + numFacets - 2) * 2.1));

		std::string token;
		while (!file.eof()) {

			token = "";
			file >> token;

			if (token == "v") {
				double x, y, z;
				file >> x >> y >> z;
				B.add_vertex(typename HDS::Vertex::Point(x, y, z));
			}

			else if (token == "f") {
				std::string line;
				std::getline(file, line);

				std::istringstream stream(line);
				std::vector<std::string> vertices;

				std::copy(std::istream_iterator <std::string>(stream),
					std::istream_iterator<std::string>(),
					std::back_inserter(vertices));

				B.begin_facet();
				for (size_t i = 0; i < vertices.size(); i++) {
					std::string::size_type pos = vertices[i].find('/', 0);
					std::string indexStr = vertices[i].substr(0, pos);
					B.add_vertex_to_facet(stoi(indexStr) - 1);
				}
				B.end_facet();
			}
		}

		file.close();

		B.end_surface();
	}

private:
	std::string mFilename;
};

template<class TPoly>
void importOBJ(const std::string& fileName, TPoly* polyhedron) {

	if (polyhedron) {
		try {
			BuildCgalPolyheadronFromObj<typename TPoly::HalfedgeDS> _buildPolyhedron(fileName);

			polyhedron->delegate(_buildPolyhedron);

			if (!polyhedron->is_valid()) {
				throw CGAL::Assertion_exception("", "", "", 0, "");
			}
		}
		catch (const CGAL::Assertion_exception&) {
			std::string msg = "Error loading " + fileName;
			std::cerr << msg << std::endl;
		}
	}
}