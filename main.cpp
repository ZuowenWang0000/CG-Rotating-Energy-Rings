///////////////////////////////////////////
////Type your name and student ID here/////
////Name:
////Student ID:
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <fstream>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;

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
	//adapter[0] = vertexShaderCode;
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	//adapter[0] = fragmentShaderCode;
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
	//TODO:
}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	
	//****************1. POINT *******STARTS******************
	const GLfloat point1[] = {
		+0.5f, +0.5f, +0.5f, //point position
		+0.0f, +1.0f, 0.0f, //point color

		+0.2f, +0.2f, +0.2f, //starting point position
		+1.0f, +0.0f, 0.0f, //point color

		+0.8f, -0.8f, +0.8f, //ending point position
		+1.0f, +0.0f, 0.0f //point color
	};

	cout << "HAHA" << endl;
	GLuint vaoID1;
	glGenVertexArrays(1,&vaoID1);
	glBindVertexArray(vaoID1);  //the VAO for point
	GLuint vboID1;
	glGenBuffers(1,&vboID1);
	glBindBuffer(GL_ARRAY_BUFFER, vboID1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point1), point1, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//****************1. POINT ******ENDS********************

	//****************2. LINE ******STARTS********************
	// const GLfloat line1[] = {
	// 	+0.2f, +0.2f, +0.2f, //starting point position
	// 	+1.0f, +0.0f, 0.0f, //point color

	// 	+0.8f, +0.8f, +0.8f, //ending point position
	// 	+1.0f, +0.0f, 0.0f //point color
	// };
	// GLuint vaoID2;
	// glGenVertexArrays(2,&vaoID2);
	// glBindVertexArray(vaoID2);  //the VAO for point
	// GLuint vboID2;
	// glGenBuffers(2,&vboID2);
	// glBindBuffer(GL_ARRAY_BUFFER, vboID2);
	// glBufferData(GL_ARRAY_BUFFER, sizeof(line1), line1, GL_STATIC_DRAW);
	// //pass the position of the point to openGL
	// glEnableVertexAttribArray(0);
	// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	// //pass the color of the point to openGL
	// glEnableVertexAttribArray(1);
	// glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));
	//****************2. LINE ******ENDS********************
}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(0.94902f,0.92941f,0.82353f,0.7f); // set the background color
	glClear(GL_COLOR_BUFFER_BIT);

	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);

	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.1f * 0, 0.0f, 0.0f));

	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
		
	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);


	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, 1); //render primitives from array data
	//NOTICE: one stride has 6 floating point data, so one point's length is 1

	glLineWidth(10);
	glDrawArrays(GL_LINES, 1, 2); //render primitives from array data
	//NOTICE: one stride has 6 floating point data, so one line's length is 2


	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	sendDataToOpenGL();
	installShaders();
}

int main(int argc, char *argv[])
{
	/*Initialization*/
	glutInit(&argc, argv);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */
	glutMainLoop();

	return 0;
}