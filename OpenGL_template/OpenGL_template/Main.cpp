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
	GLuint VertexArrayID;
	GLuint indexbuffer;
	GLFWwindow* window;
	float alpha = glm::radians(0.0f);
	glm::mat4 MVP(1.0);
	glm::mat4 M(1.0);
	glm::vec2 wh;
	GLuint wh_VectorID;

	//
	GLuint texturevertexbuffer;
	GLuint textureVertexArrayID;
	GLuint textureindexbuffer;
	//

	GLuint MVP_MatrixID;
	GLuint TextureID;
	GLuint colorbuffer;
	GLuint uvbuffer;
	GLuint Texture;
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
	wh_VectorID = glGetUniformLocation(programID, "wh");
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	
	res.loadOBJ("cube.obj", vertices, uvs, normals);

	//static const GLfloat g_vertex_buffer_data[] = {
	//	0.0f, 0.0f, 0.0f,
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	1.0f, 1.0f, 0.0f,
	//};

	//glGenBuffers(1, &vertexbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//static const GLfloat g_color_buffer_data[] = {
	//	1.0f, 0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f, 1.0f,
	//	0.0f, 0.0f, 1.0f, 1.0f,
	//	0.1f, 1.0f, 1.0f, 1.0f,
	//};

	//glGenBuffers(1, &colorbuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

	//static const GLubyte g_indices[] = {
	//	0, 1, 2, 1, 3, 2,
	//};
	//glGenBuffers(1, &indexbuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);


	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &uvbuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

}
void DrawBox(){
	glEnable(GL_BLEND);

	glUseProgram(programID);

	MVP = VP*M;
	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
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
		VERTEX_COLOR,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		2,                               
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
		);
	//glDrawElements(GL_TRIANGLES, numberofind, GL_UNSIGNED_BYTE, (GLvoid*)0);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

//void InitOctagon(){
//	glGenVertexArrays(1, &textureVertexArrayID);
//	glBindVertexArray(textureVertexArrayID);
//
//	textureProgramID = LoadShaders("Texture.VertexShader", "Texture.FragmentShader");
//	
//	MVP_MatrixID = glGetUniformLocation(textureProgramID, "MVP");
//
//	static const GLfloat g_vertex_buffer_data[] = {
//		0.0f, 0.0f, 0.0f,
//		512*1.0f, 0.0f, 0.0f,
//		0.0f, 512*1.0f, 0.0f,
//		512*1.0f, 512*1.0f, 0.0f,
//	};
//
//	glGenBuffers(1, &texturevertexbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, texturevertexbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
//
//	static const GLubyte g_indices[] = {
//		0, 1, 2, 1, 3, 2,
//	};
//	glGenBuffers(1, &textureindexbuffer);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(g_indices), g_indices, GL_STATIC_DRAW);
//
//	static const GLfloat g_uv_buffer_data[] = {
//		0.0f, 0.0f,
//		1.0f, 0.0f,
//		0.0f, 1.0f,
//		1.0f, 1.0f,
//	};
//
//	glGenBuffers(1, &uvbuffer);
//	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
//	TextureID = glGetUniformLocation(programID, "myTextureSampler");
//
//	numberofind = sizeof(g_indices) / sizeof(g_indices[0]);
//
//	Texture = loadBMP_custom("./uvtemplate2.bmp");	
//}
//
//void DrawOctagon(){
//	glEnable(GL_BLEND);
//
//	glUseProgram(textureProgramID);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, Texture);
//	// Set our "myTextureSampler" sampler to user Texture Unit 0
//	glUniform1i(TextureID, 0);
//
//	MVP = VP*M;
//	glUniformMatrix4fv(MVP_MatrixID, 1, GL_FALSE, &MVP[0][0]);
//	glEnableVertexAttribArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
//	glVertexAttribPointer(
//		VERTEX_POSITION, //layout in the shader.
//		3,       // size
//		GL_FLOAT,// type
//		GL_FALSE,// normalized
//		0,       // stride
//		(void*)0 // array buffer offset
//		);
//
//	glEnableVertexAttribArray(1);
//	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
//	glVertexAttribPointer(
//		TEXTURE_DATA,                                // attribute. No particular reason for 1, but must match the layout in the shader.
//		2,                                // size : U+V => 2
//		GL_FLOAT,                         // type
//		GL_FALSE,                         // normalized?
//		0,                                // stride
//		(void*)0                          // array buffer offset
//		);
//	glDrawElements(GL_TRIANGLES, numberofind, GL_UNSIGNED_BYTE, (GLvoid*)0);
//	
//
//	//std::cout << glm::degrees(alpha2) << " - " << roundround2 << std::endl;
//	glDisableVertexAttribArray(0);
//	glDisableVertexAttribArray(1);
//}

void Init(void) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glEnable(GL_DEPTH_TEST);

	glm::vec3 cameraPos = glm::vec3(3.0f, 3.0f, 10.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	P = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f);
	//P = glm::ortho(-1, 1, -1, 1);
	V = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);

	VP = P*V;

	InitBox();
	//InitOctagon();

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
