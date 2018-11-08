/*********************************************************
FILE : main.cpp (csci3260 2018-2019 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155123906
Student Name: Zuowen Wang
*********************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#include "C:\Users\cprj2748\Downloads\Project2\Dependencies\glew\glew.h"
#include "C:\Users\cprj2748\Downloads\Project2\Dependencies\freeglut\freeglut.h"
#include "C:\Users\cprj2748\Downloads\Project2\Dependencies\glm\glm.hpp"
#include "C:\Users\cprj2748\Downloads\Project2\Dependencies\glm\gtc\matrix_transform.hpp"
#include "C:\Users\cprj2748\Downloads\Project2\Dependencies\glm\gtc\type_ptr.hpp"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using glm::vec3;
using glm::mat4;
using namespace glm;

GLint programID;
// Could define the Vao&Vbo and interaction parameter here

GLuint vbo;
GLuint uvbo;
GLuint nvbo;
int drawSize;



glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 cameraGaze = glm::vec3(0.0f, 0.01f, -0.99f);

float hor = 3.14f;
float ver = 0.0f;
float iniFov = 45.0f;



float scale1 = 4.5f;

float x_delta = 0.1f;
float i_press_num = 0;
float k_press_num = 0;
float j_press_num = 0;
float l_press_num = 0;
int angle_press_num = 0;

double lookX = 0;
double lookY = 0;
double lookZ = 0;
double rot = 0;

float scale = 4.5;

GLfloat pitch1 = 22.0f;
GLfloat yaw1 = -90.0f;







//a series utilities for setting shader parameters
void setMat4(const std::string &name, glm::mat4& value)
{
	unsigned int transformLoc = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}

void setVec4(const std::string &name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string &name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setFloat(const std::string &name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void setInt(const std::string &name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void keyboard(unsigned char key, int x, int y)
{
	// //Define 6 camera position operations here:
	//X axel
	// w -> forwards
	// s -> backwards
	if (key == 'a')
	{
		cameraPosition -= 0.5f *glm::normalize(glm::cross(cameraGaze, glm::vec3(0, 1, 0)));
	}
	if (key == 'd')
	{
		cameraPosition += 0.5f *glm::normalize(glm::cross(cameraGaze, glm::vec3(0, 1, 0)));
	}

	//Y axel
	// a -> leftwards
	// d -> rightwards
	if (key == 's')
	{
		cameraPosition -= 0.5f * cameraGaze;
	}
	if (key == 'w')
	{
		cameraPosition += 0.5f *cameraGaze;
	}
	//Z axel
	// q -> upwards
	// e -> downwards
	if (key == 'e')
	{
		cameraPosition -= 0.5f *glm::vec3(0.0f, 0.0f, 1.0f);
	}
	if (key == 'q')
	{
		cameraPosition += 0.5f *glm::vec3(0.0f, 0.0f, 1.0f);
	}

	// // and 2 angle operations:
	// z-> camera angle up 
	// x -> camera angle down
	if (key == '+')
	{
		scale1 += 0.5;
	}
	if (key == '-')
	{
		scale1 -= 0.5;
	}

	if (key == 'r') {
		rot = rot + 3;
	}
	if (key == 'i') {
		i_press_num = i_press_num + 0.5;
	}
	if (key == 'k') {
		k_press_num = k_press_num - 0.5;
	}
	if (key == 'j') {
		j_press_num = j_press_num - 0.5;
	}
	if (key == 'l') {
		l_press_num = l_press_num + 0.5;
	}
}

void mouse(int button, int state, int x, int y) {

	if (button == 3) {
		pitch1 = pitch1 + 2.0f;
	}
	if (button == 4) {
		pitch1 = pitch1 - 2.0f;
	}
	if (button == GLUT_LEFT_BUTTON) {
		yaw1 = yaw1 - 5.0f;
	}
	if (button == GLUT_RIGHT_BUTTON) {
		yaw1 = yaw1 + 5.0f;
	}


	if (pitch1 > 89.0f) { pitch1 = 89.0f; } // to avoid deadlock
	if (pitch1 < -89.0f) { pitch1 = -89.0f; }
	glm::vec3 temp = glm::vec3(
		cos(glm::radians(pitch1)) * cos(glm::radians(yaw1)),
		sin(glm::radians(pitch1)),
		sin(glm::radians(yaw1)) * cos(glm::radians(pitch1)));

	cameraGaze = glm::normalize(temp);

}


void move(int key, int x, int y)
{
	//TODO: Use arrow keys to do interactive events and animation
}

void PassiveMouse(int x, int y)
{
	//TODO: Use Mouse to do interactive events and animation

}

bool loadOBJ(
	const char * path,
	std::vector<glm::vec3> & out_vertices,
	std::vector<glm::vec2> & out_uvs,
	std::vector<glm::vec3> & out_normals
) {

	printf("Loading OBJ file %s...\n", path);

	std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<glm::vec3> temp_vertices;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;


	FILE * file = fopen(path, "r");
	if (file == NULL) {
		printf("Impossible to open the file ! Are you in the right path ? See Tutorial 6 for details\n");
		getchar();
		return false;
	}

	while (1) {

		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break; // EOF = End Of File. Quit the loop.

				   // else : parse lineHeader

		if (strcmp(lineHeader, "v") == 0) {
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			temp_vertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uv.y = -uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			std::string vertex1, vertex2, vertex3;
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
			if (matches != 9) {
				printf("File can't be read by our simple parser :-( Try exporting with other options\n");
				return false;
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
		else {
			char stupidBuffer[1000];
			fgets(stupidBuffer, 1000, file);
		}

	}

	// For each vertex of each triangle
	//this is for data handling
	for (unsigned int i = 0; i<vertexIndices.size(); i++) {

		// Get the indices of its attributes
		unsigned int vertexIndex = vertexIndices[i];
		unsigned int uvIndex = uvIndices[i];
		unsigned int normalIndex = normalIndices[i];

		// Get the attributes thanks to the index
		glm::vec3 vertex = temp_vertices[vertexIndex - 1];
		glm::vec2 uv = temp_uvs[uvIndex - 1];
		glm::vec3 normal = temp_normals[normalIndex - 1];

		// Put the attributes in buffers
		out_vertices.push_back(vertex);
		out_uvs.push_back(uv);
		out_normals.push_back(normal);

	}

	return true;
}

GLuint loadBMP_custom(const char * imagepath) {

	printf("Reading image %s\n", imagepath);

	unsigned char header[54];
	unsigned int dataPos;
	unsigned int imageSize;
	unsigned int width, height;
	unsigned char * data;

	FILE * file = fopen(imagepath, "rb");
	if (!file) { printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar(); return 0; }

	if (fread(header, 1, 54, file) != 54) {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (header[0] != 'B' || header[1] != 'M') {
		printf("Not a correct BMP file\n");
		return 0;
	}
	if (*(int*)&(header[0x1E]) != 0) { printf("Not a correct BMP file\n");    return 0; }
	if (*(int*)&(header[0x1C]) != 24) { printf("Not a correct BMP file\n");    return 0; }

	dataPos = *(int*)&(header[0x0A]);
	imageSize = *(int*)&(header[0x22]);
	width = *(int*)&(header[0x12]);
	height = *(int*)&(header[0x16]);
	if (imageSize == 0)    imageSize = width*height * 3;
	if (dataPos == 0)      dataPos = 54;

	data = new unsigned char[imageSize];
	fread(data, 1, imageSize, file);
	fclose(file);


	GLuint textureID;
	//TODO: Create one OpenGL texture and set the texture parameter
	glGenTextures(1, &textureID);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version
	delete[] data;

	//filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glGenerateMipmap(GL_TEXTURE_2D);



	return textureID;
}

void sendDataToOpenGL()
{
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	//create VAOs VBOs 

	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture



	////TODO don't know if in the templete we should define as double or int yet
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;

	
	bool obj1 = loadOBJ("C:\\Users\\cprj2748\\Downloads\\Project2\\sources\\plane.obj", vertices, uvs, normals);
	
	//create vetex array object (vao)
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);



	//send vao of obj1 to openGL

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

	glGenBuffers(1, &nvbo);
	glBindBuffer(GL_ARRAY_BUFFER, nvbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


	drawSize = (int)vertices.size();

	//load texture here
	//GLuint Texture = loadBMP_custom("N:\cprj2748.V6\assignment2\Asg2\sources");


}

void paintGL(void)
{
	//*****************TRANSFORMATION MATRIX SECTION**********************
	glm::mat4 scaleMatrix;
	scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(4.5f));  // the last is scallin coefficience

	glm::mat4 worldView;
	worldView = glm::lookAt(
		cameraPosition, // position of camera in world space
						//glm::vec3(0+x_press_num+lookX,1+y_press_num +lookY,1+z_press_num+lookZ), // gaze direction
						//glm::vec3(1,1,1),
		cameraGaze + cameraPosition,
		//glm::vec3(1,-5,1),
		glm::vec3(0, 1, 0));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(75.0f), 800.0f / 800.0f, 0.1f, 100.0f);


	cout << "camera position: " << cameraPosition.x << "," << cameraPosition.y << "," << cameraPosition.z << endl;
	cout << "gaze: " << cameraGaze.x << "," << cameraGaze.y << "," << cameraGaze.z << endl;

	glm::mat4 mvp = projection * worldView  * scaleMatrix;
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);
	//**************************************************************************


	glClearColor(0.94902f, 0.92941f, 0.82353f, 0.7f); // set the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	int width = 800;
	int height = 600;
	glViewport(0, 0, width, height);

	mat4 fullTransformMatrix;
	mat4 viewToProjectionMatrix = glm::perspective(60.0f, ((float)width) / height, 0.1f, 20.0f);
	mat4 worldToViewMatrix = glm::perspective(35.0f, 1.0f, 0.1f, 100.0f);
	mat4 worldToProjectionMatrix = viewToProjectionMatrix * worldToViewMatrix;

	// ambientLight
	GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	vec3 ambientLight(0.1f, 0.1f, 0.1f);  // RGB light of ambient light
	glUniform3fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	//eyePosition
	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	vec3 eyePosition(0.0f, 0.0f, 0.0f);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);

	//light position world   ... for now it's lightPositionWorld, slide 26
	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	vec3 lightPositionWorld(2.0f, 15.0f, -10.0f);
	glUniform3fv(lightPositionUniformLocation, 1, &lightPositionWorld[0]);



	////diffuse
	//vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	//float brightness = dot(lightVectorWorld, normalize(normalWorld));
	//vec4 diffuseLight = vec4(birghtness, brightness, brightness, 1.0);



	////send VBOs to vertexShader [one attribute one VBO]
	////1st attribute buffer: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		0, // stride
		(void*)0 // array buffer offset
	);

	glColor3f(0, 0, 1);

	glDrawArrays(GL_TRIANGLES, 0, drawSize);






	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	glewInit();
	installShaders();
	sendDataToOpenGL();
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("Assignment 2");

	glutInitWindowSize(1000, 1000);
	//TODO:
	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);

	//glutKeyboardFunc(keyboard);
	//glutSpecialFunc(move);
	//glutPassiveMotionFunc(PassiveMouse);

	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();

	return 0;
}
