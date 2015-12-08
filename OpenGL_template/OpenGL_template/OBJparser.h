#pragma once
#include <vector>
#include <glm\glm.hpp>

class OBJparser
{
public:
	OBJparser();
	~OBJparser();

	bool loadOBJ(const char * path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals, std::vector<glm::vec3> &out_tangents, std::vector<glm::vec3> &out_bitangents);
	void OBJparser::computeTangentBasis(
		// inputs
		std::vector<unsigned int>&   indices,
		std::vector<glm::vec3> & vertices,
		std::vector<glm::vec2> & uvs,
		std::vector<glm::vec3> & normals,
		// outputs
		std::vector<glm::vec3> & tangents,
		std::vector<glm::vec3> & bitangents
		);
private:
	std::vector< unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::string path;
};

