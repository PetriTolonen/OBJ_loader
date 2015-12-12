#define GLM_FORCE_RADIANS
#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <shader.hpp>
#include <texture.hpp>


#include "vertexshader.h"
#include <iostream>

#include "OBJparser.h"

// Testing generated arrays in inl file.
#include "cube.inl"

namespace {
	GLuint programID;
	GLuint programID2;
	GLuint vertexbuffer;
	GLuint vertexbuffer2;
	GLuint normalbuffer;
	GLuint tangentbuffer;
	GLuint bitangentbuffer;

	GLuint VertexArrayID; 
	GLuint indexbuffer;
	GLFWwindow* window;
	float alpha = glm::radians(0.0f);
	glm::mat4 MVP(1.0);
	glm::mat3 normalMatrix(1.0);
	glm::mat4 M(1.0);
	glm::vec2 wh;
	GLuint wh_VectorID;

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> vertices2;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitangents;

	GLuint MVP_MatrixID;
	GLuint MVP_MatrixID2;
	GLuint VP_MatrixID;
	GLuint M_MatrixID;
	GLuint L_VecID;
	GLuint Normal_mapID;
	GLuint Diffuse_mapID;
	GLuint colorbuffer;
	GLuint uvbuffer;
	GLuint normal_map;
	GLuint diffuse_map;
	int numberofind = 0;

	glm::mat4 V;
	glm::mat4 P;
	glm::mat4 VP;
	glm::vec3 L;

	OBJparser res;
};

void InitObject(){
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	MVP_MatrixID = glGetUniformLocation(programID, "MVP");
	M_MatrixID = glGetUniformLocation(programID, "M");
	L_VecID = glGetUniformLocation(programID, "L");
	
	res.loadOBJ("torusUvmapped.obj", vertices, uvs, normals, tangents, bitangents, false);

	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);

	glGenBuffers(1, &tangentbuffer);
	glGenBuffers(1, &bitangentbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec3), &tangents[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(glm::vec3), &bitangents[0], GL_STATIC_DRAW);

	Normal_mapID = glGetUniformLocation(programID, "rockwall_normal");	
	normal_map = loadBMP_custom("./torusnormal.bmp");

	Diffuse_mapID = glGetUniformLocation(programID, "rockwall_diffuse");
	diffuse_map = loadBMP_custom("./uvtemplate.bmp");
}
void DrawObject(float x, float y, float z, float rotation, glm::vec3 rotationaxel){
	glEnable(GL_BLEND);

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normal_map);
	glUniform1i(Normal_mapID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuse_map);
	glUniform1i(Diffuse_mapID, 1);

	M = glm::translate(glm::vec3(x, y, z))*glm::rotate(rotation, rotationaxel);

	MVP = VP * M;
	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &M[0][0]);

	glUniform3fv(L_VecID, 1, &L[0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glVertexAttribPointer(
		VERTEX_POSITION, //layout in the shader.
		3,       // size
		GL_FLOAT,// type
		GL_FALSE,// normalized
		0,       // stride
		(void*)0 // array buffer offset
		);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glVertexAttribPointer(			
		TEXTURE_DATA,                               // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                               
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glVertexAttribPointer(
		NORMAL_DATA,                               // attribute. No particular reason for 1, but must match the layout in the shader.
		3,
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
	glVertexAttribPointer(
		TANGENT_DATA,                               // attribute. No particular reason for 1, but must match the layout in the shader.
		3,
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glEnableVertexAttribArray(4);
	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
	glVertexAttribPointer(
		BITANGENT_DATA,                               // attribute. No particular reason for 1, but must match the layout in the shader.
		3,
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);

	glDrawArrays(GL_TRIANGLES, 0, vertices.size());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

// Testing generated inl file. TODO: Remove center vertex (0 , 0).
void InitLightPoint()
{
	// Use this atleast once to generate inl object.
	//res.loadOBJ("uvpointball.obj", vertices2, uvs, normals, tangents, bitangents, true);

	glGenBuffers(1, &vertexbuffer2);

	// If using the data from vertices2
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	//glBufferData(GL_ARRAY_BUFFER, vertices2.size() * sizeof(glm::vec3), &vertices2[0], GL_STATIC_DRAW);

	glGenBuffers(1, &vertexbuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), &Vertices[0], GL_STATIC_DRAW);

	// Light cube doesnt need uvs, normals... These are for testing
	/*glBindBuffer(GL_ARRAY_BUFFER, uvbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Uvs), &Uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals), &Normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Tangents), &Tangents[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bitangents), &Bitangents[0], GL_STATIC_DRAW);*/

	programID2 = LoadShaders("Shadeless.vertexshader", "Shadeless.fragmentshader");
	MVP_MatrixID2 = glGetUniformLocation(programID2, "MVP");
}
void DrawLightPoint(glm::vec3 position, float rotation, glm::vec3 rotationaxel)
{
	glEnable(GL_BLEND);

	glUseProgram(programID2);

	M = glm::translate(position)*glm::rotate(rotation, rotationaxel);;

	MVP = VP * M;
	glUniformMatrix4fv(MVP_MatrixID2, 1, GL_FALSE, &MVP[0][0]);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer2);
	glVertexAttribPointer(
		VERTEX_POSITION, //layout in the shader.
		3,       // size
		GL_FLOAT,// type
		GL_FALSE,// normalized
		0,       // stride
		(void*)0 // array buffer offset
		);

	//glDrawArrays(GL_TRIANGLES, 0, vertices2.size());
	glDrawArrays(GL_POINTS, 0, sizeof(Vertices)/3);


	glDisableVertexAttribArray(0);
}

void Init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cameraPos = glm::vec3(1.5f, 0.0f, 4.5f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	V = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

	VP = P*V;

	InitObject();
	InitLightPoint();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	alpha += 0.005;

	L = glm::vec3(4.0f, 4.0f, (-7.0f + 14.0f * glm::cos(alpha))); //Light position
	DrawLightPoint(L, alpha, glm::vec3(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < 1000; i++)
	{
		DrawObject(((i*i) / 40.0f) * glm::sin(alpha) * 1.2f + i*0.7f, (((i*i) / 20.0f) * glm::cos(alpha) * 0.6f), (-i  * 3.0f), (i+1) * alpha, glm::vec3(0.0f, 1.0f, 1.0f));
	}	

	glfwSwapBuffers(window);
}

void Uninit(void) {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &vertexbuffer2);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &tangentbuffer);
	glDeleteBuffers(1, &bitangentbuffer);
	
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteProgram(programID);
	glDeleteProgram(programID2);
}

int main(void) {
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);

	int w = 800;
	int h = 600;
	
	window = glfwCreateWindow(w, h,
		"OpenGL demo", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	
	//framebuffer....

	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	Init();

	do{
		Render();
		glfwPollEvents();
	} while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	glfwTerminate();
	Uninit();

	return 0;
}
