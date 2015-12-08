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

namespace {
	GLuint programID;
	GLuint textureProgramID;
	GLuint vertexbuffer;
	GLuint normalbuffer;
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
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	//
	GLuint texturevertexbuffer;
	GLuint textureVertexArrayID;
	GLuint textureindexbuffer;
	//

	GLuint MVP_MatrixID;
	GLuint VP_MatrixID;
	GLuint M_MatrixID;
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

	OBJparser res;
};

void InitBox(){
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	MVP_MatrixID = glGetUniformLocation(programID, "MVP");
	VP_MatrixID = glGetUniformLocation(programID, "VP");
	M_MatrixID = glGetUniformLocation(programID, "M");
	
	res.loadOBJ("rockwall.obj", vertices, uvs, normals);


	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &normalbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

	Normal_mapID = glGetUniformLocation(programID, "rockwall_normal");	
	normal_map = loadBMP_custom("./rockwall_normal_map.bmp");

	Diffuse_mapID = glGetUniformLocation(programID, "rockwall_diffuse");
	diffuse_map = loadBMP_custom("./rockwall_diffuse_map.bmp");
}
void DrawBox(){
	glEnable(GL_BLEND);

	glUseProgram(programID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, normal_map);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Normal_mapID, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuse_map);
	// Set our "myTextureSampler" sampler to user Texture Unit 0
	glUniform1i(Diffuse_mapID, 1);

	M = glm::translate(glm::vec3(cos(alpha),0.0f,0.0f))*glm::rotate(alpha, glm::vec3(1.0f,0.0f,0.0f));
	alpha += 0.008f;

	//M = glm::translate(glm::vec3(0.0f, 0.0f, 0.0f))*glm::rotate(1.5707963267f, glm::vec3(1.0f, 0.0f, 0.0f));

	MVP = VP * M;
	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
	M = M;
	glUniformMatrix4fv(M_MatrixID, 1, GL_FALSE, &M[0][0]);

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
		NORMAL_BUFFER,                               // attribute. No particular reason for 1, but must match the layout in the shader.
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
}

//void Initlamp()
//{
//	GLuint lightVAO;
//	glGenVertexArrays(1, &lightVAO);
//	glBindVertexArray(lightVAO);
//	// We only need to bind to the VBO, the container's VBO's data already contains the correct data.
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	// Set the vertex attributes (only position data for our lamp)
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
//	glEnableVertexAttribArray(0);
//	glBindVertexArray(0);
//}

void Init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	V = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

	VP = P*V;

	InitBox();
	//Initlamp();

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Render(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//alpha += 0.01;

	DrawBox();
	//DrawOctagon();

	glfwSwapBuffers(window);
}

void Uninit(void) {
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &colorbuffer);
	glDeleteBuffers(1, &texturevertexbuffer);
	glDeleteBuffers(1, &textureindexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteVertexArrays(1, &VertexArrayID);
	glDeleteVertexArrays(1, &textureVertexArrayID);
	glDeleteProgram(programID);
	glDeleteProgram(textureProgramID);
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
		"Tutorial UV", NULL, NULL);
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

	return 0;
}
