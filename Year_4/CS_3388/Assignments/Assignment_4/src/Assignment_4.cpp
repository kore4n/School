﻿// Assignment 4
// Created by: Jason Kwan

#define GLM_ENABLE_EXPERIMENTAL
#define _CRT_SECURE_NO_WARNINGS

// Include standard headers
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
using namespace glm;

#include <vector>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#define CAMERA_MOVE_SPEED 1.0f/100.0f
#define CAMERA_ROTATE_SPEED_DEGREES 1.0f 

#define HEADER_FILE_SIZE 14

double dy = -15.0f;
double dx = -100.0f;
//Nope. You'll have to do this yourself for assignment 5
//#include "CamControls.hpp"
void cameraControlsGlobe(glm::vec3& eye, glm::vec3& center, glm::vec3& up, glm::mat4& V, glm::mat4& M) {
	if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_PRESS && glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_PRESS)
		return;

	glm::vec3 lookAt;
	glm::vec3 facing = center - eye;
	facing = glm::normalize(facing) * CAMERA_MOVE_SPEED;

	glm::vec3 facingRight = glm::cross(facing, up);
	facingRight = glm::normalize(facingRight) * CAMERA_MOVE_SPEED;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		eye += facing;
		center += facing;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		eye -= facing;
		center += facing;
	}

	glm::vec3 testMove = { 0.0f, 0.1f, 0.0f };
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		// Rotate counter-clockwise in place
		dx += 1.0f;
		lookAt.x = cos(glm::radians(dx)) * cos(glm::radians(dy));
		lookAt.y = sin(glm::radians(dy));
		lookAt.z = sin(glm::radians(dx)) * cos(glm::radians(dy));

		lookAt = glm::normalize(lookAt);

		center = eye + lookAt;

		//std::cout << dx << std::endl;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		// Rotate clockwise in place
		dx -= 1.0f;

		lookAt.x = cos(glm::radians(dx)) * cos(glm::radians(dy));
		lookAt.y = sin(glm::radians(dy));
		lookAt.z = sin(glm::radians(dx)) * cos(glm::radians(dy));

		lookAt = glm::normalize(lookAt);

		//std::cout << dx << std::endl;
		center = eye + lookAt;
	}

	V = glm::lookAt(eye, center, up);
}

/**
 * Given a file path imagepath, read the data in that bitmapped image
 * and return the raw bytes of color in the data pointer.
 * The width and height of the image are returned in the weight and height pointers,
 * respectively.
 *
 * usage:
 *
 * unsigned char* imageData;
 * unsigned int width, height;
 * loadARGB_BMP("mytexture.bmp", &imageData, &width, &height);
 *
 * Modified from https://github.com/opengl-tutorials/ogl.
 */
void loadARGB_BMP(const char* imagepath, unsigned char** data, unsigned int* width, unsigned int* height) {

	printf("Reading image %s\n", imagepath);

	// Data read from the header of the BMP file
	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	// Actual RGBA data

	// Open the file
	FILE* file = fopen(imagepath, "rb");
	if (!file) {
		printf("%s could not be opened. Are you in the right directory?\n", imagepath);
		getchar();
		return;
	}

	// Read the header, i.e. the 54 first bytes

	// If less than 54 bytes are read, problem
	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file1\n");
		fclose(file);
		return;
	}

	// Read the information about the image
	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	*width = *(int*)&(header[0x12]);
	*height = *(int*)&(header[0x16]);
	// A BMP files always begins with "BM"
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file2\n");
		fclose(file);
		return;
	}
	// Make sure this is a 32bpp file
	if (*(int*)&(header[0x1E]) != 3) {
		printf("Not a correct BMP file3\n");
		fclose(file);
		return;
	}
	// fprintf(stderr, "header[0x1c]: %d\n", *(int*)&(header[0x1c]));
	// if ( *(int*)&(header[0x1C])!=32 ) {
	//     printf("Not a correct BMP file4\n");
	//     fclose(file);
	//     return;
	// }

	// Some BMP files are misformatted, guess missing information
	if (imageSize == 0)    imageSize = (*width) * (*height) * 4; // 4 : one byte for each Red, Green, Blue, Alpha component
	if (dataPos == 0)      dataPos = 54; // The BMP header is done that way

	// Create a buffer
	*data = new unsigned char[imageSize];

	if (dataPos != 54) {
		fread(header, 1, dataPos - 54, file);
	}

	// Read the actual data from the file into the buffer
	fread(*data, 1, imageSize, file);

	// Everything is in memory now, the file can be closed.
	fclose(file);
}

class VertexData {
	public: 
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
		glm::vec2 texCoord;
};

class TriData {
	public:
		unsigned int indexV1;
		unsigned int indexV2;
		unsigned int indexV3;
};

static void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces);


class TexturedMesh {
	private: 
		GLuint vertexPositionsBufferID;
		GLuint texCoordID;
		GLuint indexBufferID;
		GLuint textureID;
		GLuint ProgramID;	// For the shader
		GLuint vaoID;
		

		std::vector<VertexData> vertices;
		std::vector<TriData> faces;

		std::vector<GLfloat> vertexInfo;
		std::vector<unsigned int> vertexIndices;
		std::vector<GLfloat> uvInfo;


		GLuint MatrixID;

	public: 
		TexturedMesh(std::string PLYFilePath, std::string bitmapImageFilePath) {
			readPLYFile(PLYFilePath, vertices, faces);

			//std::cout << "Vertex Indices: " << std::endl;

			for (int i = 0; i < faces.size(); i++) {
				unsigned int indexV1 = faces[i].indexV1;
				unsigned int indexV2 = faces[i].indexV2;
				unsigned int indexV3 = faces[i].indexV3;

				vertexIndices.push_back(indexV1);
				vertexIndices.push_back(indexV2);
				vertexIndices.push_back(indexV3);

				//std::cout << "Index 1: " << indexV1 << "Index 2: " << indexV2 << "Index 3: " << indexV3 << std::endl;
			}

			for (int i = 0; i < vertices.size(); i++) {
				vertexInfo.push_back(vertices[i].position.x);
				vertexInfo.push_back(vertices[i].position.y);
				vertexInfo.push_back(vertices[i].position.z);
				/*vertexInfo.push_back(vertices[i].texCoord.x);
				vertexInfo.push_back(vertices[i].texCoord.y);*/
				//vertexInfo.push_back(vertices[i].color.r);

				//std::cout << "X: " << vertices[i].texCoord.x << " Y: " << vertices[i].texCoord.y << std::endl;
				uvInfo.push_back(vertices[i].texCoord.x);
				uvInfo.push_back(vertices[i].texCoord.y);
			}

			
			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
			std::string VertexShaderCode = "\
    			#version 330 core\n\
				// Input vertex data, different for all executions of this shader.\n\
				layout(location = 0) in vec3 vertexPosition;\n\
				layout(location = 1) in vec2 uv;\n\
				// Output data ; will be interpolated for each fragment.\n\
				out vec2 uv_out;\n\
				// Values that stay constant for the whole mesh.\n\
				uniform mat4 MVP;\n\
				void main(){ \n\
					// Output position of the vertex, in clip space : MVP * position\n\
					gl_Position =  MVP * vec4(vertexPosition,1);\n\
					// The color will be interpolated to produce the color of each fragment\n\
					uv_out = uv;\n\
				}\n";

			// Read the Fragment Shader code from the file
			std::string FragmentShaderCode = "\
				#version 330 core\n\
				in vec2 uv_out; \n\
				uniform sampler2D tex;\n\
				void main() {\n\
					gl_FragColor = texture(tex, uv_out);\n\
				}\n";
			char const* VertexSourcePointer = VertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);

			// Compile Fragment Shader
			char const* FragmentSourcePointer = FragmentShaderCode.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);

			// Link the program
			ProgramID = glCreateProgram();
			glAttachShader(ProgramID, VertexShaderID);
			glAttachShader(ProgramID, FragmentShaderID);
			glLinkProgram(ProgramID);

			glDetachShader(ProgramID, VertexShaderID);
			glDetachShader(ProgramID, FragmentShaderID);

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);

			

			glGenVertexArrays(1, &vaoID);
			glBindVertexArray(vaoID);

			//std::cout << bitmapImageFilePath.c_str() << std::endl;

			unsigned char* data;
			GLuint width, height;
			loadARGB_BMP(bitmapImageFilePath.c_str(), &data, &width, &height);
			glGenTextures(1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
			
			MatrixID = glGetUniformLocation(ProgramID, "MVP");

			glGenBuffers(1, &vertexPositionsBufferID);
			glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBufferID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * vertexInfo.size(), vertexInfo.data(), GL_STATIC_DRAW);

			// 1st attribute : vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(
				0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0			// buffer offset
			);

			glGenBuffers(1, &texCoordID);
			glBindBuffer(GL_ARRAY_BUFFER, texCoordID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * uvInfo.size(), uvInfo.data(), GL_STATIC_DRAW);

			// 2nd attribute : UV
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                  // stride
				(void*)0			// buffer offset
			);

			glGenBuffers(1, &indexBufferID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * vertexIndices.size(), vertexIndices.data(), GL_STATIC_DRAW);

			//glBindVertexArray(0);
		}

	void draw(glm::mat4 MVP) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glUseProgram(ProgramID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glBindVertexArray(vaoID);

		glDrawElements(GL_TRIANGLES, vertexIndices.size(), GL_UNSIGNED_INT, (void*)0);	// Asn 4

		glBindVertexArray(0);
		glUseProgram(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

bool startsWith(std::string myString, std::string prefix) {
	return myString.rfind(prefix, 0) == 0;
}

static void splitWords(std::string str, std::vector<std::string> &wordsList)
{
	std::string word = "";

	for (auto x : str)
	{
		if (x == ' ')
		{
			wordsList.push_back(word);
			word = "";
		}
		else {
			word = word + x;
		}
	}

	if (word != "") {
		wordsList.push_back(word);
	}
}

// 1. must handle the vertex properties x, y, z, nx, ny, nz, red, green,
// blue, u, v and any number of vertices.Assume there are no other vertex properties.You can
// assume x, y, z will be specified but any other property is optional
// 2. must handle faces with a property list vertex_indices. You can
// assume this list is always of size 3
static void readPLYFile(std::string fname, std::vector<VertexData>& vertices, std::vector<TriData>& faces) {
	try {
		std::ifstream myfile;
		myfile.open(fname);

		std::string currentLine;

		int i = 0;
		int numVertices = 0;
		int numFaces = 0;

		int verticesLeftToRead = 0;
		int facesLeftToRead = 0;
		bool headerEnded = false;

		std::vector<std::string> words;

		//std::cout << "Trying to open file." << fname << std::endl;


		if (myfile.is_open()) { // always check whether the file is open
			//std::cout << "Opened file";
			while (myfile.good()) {
				while (std::getline(myfile, currentLine)) {
					std::vector<std::string> words = {};
					splitWords(currentLine, words);

					if (startsWith(currentLine, "element vertex ")) {
						//std::cout << words.back() << std::endl;
						numVertices = std::stoi(words.back());
						verticesLeftToRead = numVertices;
					}

					if (startsWith(currentLine, "element face ")) {
						//std::cout << words.back() << std::endl;
						numFaces = std::stoi(words.back());
						facesLeftToRead = numFaces;
					}

					// End of header
					if (headerEnded == false) {
						if (startsWith(currentLine, "end_header")) {
							headerEnded = true;
						}
						continue; 
					}

					if (verticesLeftToRead > 0) {
						float vx = std::stof(words[0]);
						float vy = std::stof(words[1]);
						float vz = std::stof(words[2]);

						float nx = std::stof(words[3]);
						float ny = std::stof(words[4]);
						float nz = std::stof(words[5]);

						float u = std::stof(words[6]);
						float v = std::stof(words[7]);

						//std::cout << currentLine << std::endl;


						VertexData newVertex;

						newVertex.position = glm::vec3{ vx, vy, vz };
						newVertex.normal = glm::vec3{ nx, ny, nz };
						newVertex.texCoord.x = u;
						newVertex.texCoord.y = v;

						//std::cout << "U: " << u << " V: " << v << std::endl;
						//std::cout << "X: " << newVertex->texCoord.x  << " Y: " << newVertex->texCoord.y << std::endl;

						vertices.push_back(newVertex);

						verticesLeftToRead--;
					}
					else if (facesLeftToRead > 0) {


						unsigned int indexOfVertex1 = std::stoi(words[1]);
						unsigned int indexOfVertex2 = std::stoi(words[2]);
						unsigned int indexOfVertex3 = std::stoi(words[3]);

						facesLeftToRead--;
						//std::cout << currentLine << std::endl;

						TriData newFace;
						newFace.indexV1 = indexOfVertex1;
						newFace.indexV2 = indexOfVertex2;
						newFace.indexV3 = indexOfVertex3;

						faces.push_back(newFace);
						//std::cout << "Word index 1: " << words[1] << " Word index 2: " << words[2] << " Word index 3: " << words[3] << std::endl;

						//std::cout << "Index 1: " << indexOfVertex1 << " Index 2: " << indexOfVertex2 << " Index 3: " << indexOfVertex3 << std::endl;

					}
					//std::cout << currentLine << std::endl;
				}
			}
		}

		myfile.close();

		/*std::cout << "From READPLYFILE function: Vertices length " << vertices.size() << std::endl;
		std::cout << "From READPLYFILE function: Faces length " << faces.size() << std::endl;*/
	}
	catch (int exception) {
		std::cout << "An error occurred reading the file." << std::endl;
	}
}



//////////////////////////////////////////////////////////////////////////////
// Helper Objects
//////////////////////////////////////////////////////////////////////////////


class Plane {

public:
	enum PLANE_WHICH {
		x,
		y,
		z
	};

private:
	PLANE_WHICH plane = PLANE_WHICH::x;

	glm::vec4 color = glm::vec4(0.9f, 0.9f, 0.9f, 0.1f);

	GLfloat size;

public:



	Plane(GLfloat sz) : size(sz) {}

	void draw() {

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (plane == PLANE_WHICH::x) {
			glBegin(GL_QUADS);

			glColor4f(color.x, color.y, color.z, color.w);
			glVertex3f(-size, 0.0f, -size);
			glVertex3f(size, 0.0f, -size);
			glVertex3f(size, 0.0f, size);
			glVertex3f(-size, 0.0f, size);

			glEnd();

			glBegin(GL_LINES);
			glColor4f(color.x, color.y, color.z, color.w + 0.2f);

			for (int i = -size; i < size; ++i) {
				glVertex3f(1.0f * i, 0.0f, -size);
				glVertex3f(1.0f * i, 0.0f, size);
				glVertex3f(size, 0.0f, 1.0f * i);
				glVertex3f(-size, 0.0f, 1.0f * i);
			}
			glEnd();

		}


		glPopMatrix();
		glDisable(GL_BLEND);
	}

};

class Axes {

	glm::vec3 origin;
	glm::vec3 extents;

	glm::vec3 xcol = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 ycol = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 zcol = glm::vec3(0.0f, 0.0f, 1.0f);

public:

	Axes(glm::vec3 orig, glm::vec3 ex) : origin(orig), extents(ex) {}

	void draw() {

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();


		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3f(xcol.x, xcol.y, xcol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);

		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z + 0.1);
		glVertex3f(origin.x + extents.x, origin.y, origin.z);
		glVertex3f(origin.x + extents.x, origin.y, origin.z - 0.1);

		glColor3f(ycol.x, ycol.y, ycol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);

		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z + 0.1);
		glVertex3f(origin.x, origin.y + extents.y, origin.z);
		glVertex3f(origin.x, origin.y + extents.y, origin.z - 0.1);

		glColor3f(zcol.x, zcol.y, zcol.z);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(origin.x, origin.y, origin.z + extents.z);

		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x + 0.1, origin.y, origin.z + extents.z);

		glVertex3f(origin.x, origin.y, origin.z + extents.z);
		glVertex3f(origin.x - 0.1, origin.y, origin.z + extents.z);
		glEnd();


		glPopMatrix();
	}

};




//////////////////////////////////////////////////////////////////////////////
// Main
//////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	float screenW = 1400;
	float screenH = 900;
	window = glfwCreateWindow(screenW, screenH, "Assignment_4", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}


	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.2f, 0.2f, 0.3f, 0.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glm::mat4 MVP;

	Axes ax(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(4.0f, 4.0f, 4.0f));
	Plane plane(5.0f);

	glm::vec3 eye = { 0.5f, 0.4f, 0.5f };
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 center = { 0.0f, 0.0f, -1.0f };
	
	glm::vec3 initialLookAt = center - eye;
	float initialRadians = glm::angle(eye, center);

	// Begin with the camera being positioned at (0.5,0.4,0.5) in world space and looking in the
	// direction of (0, 0, −1).
	glm::mat4 V = glm::lookAt(eye, center, up);
	glm::mat4 M = glm::mat4(1.0f);


	TexturedMesh* bottlesMesh = new TexturedMesh("Bottles.ply", "bottles.bmp");
	TexturedMesh* floorMesh = new TexturedMesh("Floor.ply", "floor.bmp");
	TexturedMesh* woodObjects = new TexturedMesh("WoodObjects.ply", "woodobjects.bmp");
	TexturedMesh* walls = new TexturedMesh("Walls.ply", "walls.bmp");
	TexturedMesh* table = new TexturedMesh("Table.ply", "table.bmp");
	TexturedMesh* patio = new TexturedMesh("Patio.ply", "patio.bmp");
	TexturedMesh* windows = new TexturedMesh("WindowBG.ply", "windowbg.bmp");

	TexturedMesh* door = new TexturedMesh("DoorBG.ply", "doorbg.bmp");
	TexturedMesh* metalObjects = new TexturedMesh("MetalObjects.ply", "metalObjects.bmp");
	TexturedMesh* curtainsMesh = new TexturedMesh("Curtains.ply", "curtains.bmp");

	do {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glm::mat4 Projection = glm::perspective(glm::radians(45.0f), screenW / screenH, 0.001f, 1000.0f);
		// Projection = glm::mat4(1.0f);
		glLoadMatrixf(glm::value_ptr(Projection));

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		
		
		// Move camera
		cameraControlsGlobe(eye, center, up, V, M);


		glm::mat4 MV = V * M;
		glLoadMatrixf(glm::value_ptr(V));

		MVP = Projection * V * M;

		/*glUseProgram(ProgramID);
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);*/


		bottlesMesh->draw(MVP);
		floorMesh->draw(MVP);
		woodObjects->draw(MVP);
		walls->draw(MVP);
		table->draw(MVP);
		patio->draw(MVP);
		windows->draw(MVP);

		door->draw(MVP);
		metalObjects->draw(MVP);
		curtainsMesh->draw(MVP);

		glUseProgram(0);

		/*ax.draw();
		plane.draw();*/

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();


	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup shader
	//glDeleteProgram(ProgramID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}
