#include "OBJparser.h"
#include <iostream>
#include <fstream>

OBJparser::OBJparser()
{
}


OBJparser::~OBJparser()
{
}

bool OBJparser::loadOBJ(const char * path, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals, std::vector<glm::vec3> &out_tangents, std::vector<glm::vec3> &out_bitangents, bool saveToTxt)
{
	this->saveToTxt = saveToTxt;
	out_vertices.clear();
	out_uvs.clear();
	out_normals.clear();
	out_tangents.clear();
	out_bitangents.clear();

	temp_vertices.clear();
	temp_uvs.clear();
	temp_normals.clear();

	vertexIndices.clear();
	vertexIndices.clear();
	vertexIndices.clear();
	uvIndices.clear();
	uvIndices.clear();
	uvIndices.clear();
	normalIndices.clear();
	normalIndices.clear();
	normalIndices.clear();

	FILE * file = fopen(path, "r");
	if (file == NULL)
	{
		std::cout << "OBJ not loaded!!!" << std::endl;
		return false;
	}

	while (1)
	{
		char lineHeader[128]; //SIZE of the line!!!

		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break;
		}
		if (strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9)
			{
				std::cout << "Object cannot be read with this parser. Try exporting with other options\n";
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	

	for (unsigned int i = 0; i < vertexIndices.size(); i++)
	{
		unsigned int vertexIndex = vertexIndices[i];
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		out_vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < uvIndices.size(); i++)
	{
		unsigned int uvIndex = uvIndices[i];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		out_uvs.push_back(uv);
	}

	for (unsigned int i = 0; i < normalIndices.size(); i++)
	{
		unsigned int normalIndex = normalIndices[i];
		glm::vec3 normal = temp_normals[normalIndex - 1];
		out_normals.push_back(normal);
	}
	computeTangentBasis(out_vertices, out_uvs, out_normals, out_tangents, out_bitangents);
	
	// Saving data... TODO: remove last , from arrays?
	if (saveToTxt == true)
	{
		std::ofstream outPutData;
		outPutData.open("cube.inl");

		outPutData << "float Vertices[" << out_vertices.size() * 3<<"] = { \n";
		for (int i = 0; i < out_vertices.size(); i++)
		{
			outPutData << out_vertices[i].x << " , " << out_vertices[i].y << " , " << out_vertices[i].z << " , " << std::endl;
		}
		outPutData << "};\n";

		outPutData << "float Uvs[" << out_uvs.size() * 3 << "] = {\n";
		for (int i = 0; i < out_uvs.size(); i++)
		{
			outPutData << out_uvs[i].x << " , " << out_uvs[i].y << " , "  << std::endl;
		}
		outPutData << "};\n";
	
		outPutData << "float Normals[" << out_normals.size() * 3 << "] = {\n";
		for (int i = 0; i < out_normals.size(); i++)
		{
			outPutData << out_normals[i].x << " , " << out_normals[i].y << " , " << out_normals[i].z << " , " << std::endl;
		}
		outPutData << "};\n";

		outPutData << "float Tangents[" << out_tangents.size() * 3 << "] = {\n";
		for (int i = 0; i < out_tangents.size(); i++)
		{
			outPutData << out_tangents[i].x << " , " << out_tangents[i].y << " , " << out_tangents[i].z << " , " << std::endl;
		}
		outPutData << "};\n";

		outPutData << "float Bitangents[" << out_bitangents.size() * 3 << "] = {\n";
		for (int i = 0; i < out_bitangents.size(); i++)
		{
			outPutData << out_bitangents[i].x << " , " << out_bitangents[i].y << " , " << out_bitangents[i].z << " , " << std::endl;
		}
		outPutData << "};\n";

		outPutData << "uint16_t Indices[" << vertexIndices.size() << "] = {\n";
		for (int i = 0; i < vertexIndices.size(); i++)
		{
			outPutData << vertexIndices[i] << " , " << std::endl;
		}
		outPutData << "};\n";

		outPutData.close();
	}
}

void OBJparser::computeTangentBasis(
	// inputs
	std::vector<glm::vec3> & vertices,
	std::vector<glm::vec2> & uvs,
	std::vector<glm::vec3> & normals,
	// outputs
	std::vector<glm::vec3> & tangents,
	std::vector<glm::vec3> & bitangents
	){
	for (unsigned int i = 0; i<vertices.size(); i += 3)
	{
		glm::vec3 & v0 = vertices[i + 0];
		glm::vec3 & v1 = vertices[i + 1];
		glm::vec3 & v2 = vertices[i + 2];

		// Shortcuts for UVs
		glm::vec2 & uv0 = uvs[i + 0];
		glm::vec2 & uv1 = uvs[i + 1];
		glm::vec2 & uv2 = uvs[i + 2];

		// Edges of the triangle : postion delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		// UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		// Set the same tangent for all three vertices of the triangle.
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		// Same thing for binormals
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
	}

	for (unsigned int i = 0; i<vertices.size(); i += 1)
	{
		glm::vec3 & n = normals[i];
		glm::vec3 & t = tangents[i];
		glm::vec3 & b = bitangents[i];

		// Gram-Schmidt orthogonalize
		t = glm::normalize(t - n * glm::dot(n, t));

		// Calculate handedness
		if (glm::dot(glm::cross(n, t), b) < 0.0f){
			t = t * -1.0f;
		}
	}
}