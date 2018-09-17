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
#include <unistd.h>
using namespace std;
using glm::vec3;
using glm::mat4;

GLint programID;
float x_delta = 0.1f;
int x_press_num = 0;
int y_press_num = 0;
int z_press_num = 0;
int angle_press_num = 0;

double lookX = 0;
double lookY = 0;
double lookZ = 0;

float scale = 1.0f;

GLuint VBOs[5], VAOs[5];
GLuint EBO;

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
	// //Define 6 camera position operations here:
	//X axel
	// w -> forwards
	// s -> backwards
		if (key == 'a')
	{
		x_press_num -= 1;
	}
	if (key == 'd')
	{
		x_press_num += 1;
	}

	//Y axel
	// a -> leftwards
	// d -> rightwards
		if (key == 's')
	{
		y_press_num -= 1;
	}
	if (key == 'w')
	{
		y_press_num += 1;
	}
	//Z axel
	// q -> upwards
	// e -> downwards
		if (key == 'e')
	{
		z_press_num -= 1;
	}
	if (key == 'q')
	{
		z_press_num += 1;
	}

	// // and 2 angle operations:
	// z-> camera angle up 
	// x -> camera angle down
		if (key == 'x')
	{
		angle_press_num -= 1;
	}
	if (key == 'z')
	{
		angle_press_num += 1;
	}

	if (key == 'u')
	{
		lookX = lookX +0.1;
	}
	if (key == 'i')
	{
		lookY = lookY +0.1;
	}
	if (key == 'o')
	{
		lookZ = lookX +0.1;
	}

		if (key == 'j')
	{
		lookX = lookX -0.1;
	}
	if (key == 'k')
	{
		lookY = lookY -0.1;
	}
	if (key == 'l')
	{
		lookZ = lookX -0.1;
	}

}

void mouse(int button, int state, int x, int y){

  if (button == 3 ) {
	  scale = scale + 0.05f;
  }
  if (button == 4){
	  scale = scale - 0.05f;
  }

}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	//create VAOs VBOs 
 	glGenVertexArrays(5,VAOs);	
	glGenBuffers(5,VBOs);

	//****************0. GROUND *********************
	//using indexing
	const GLfloat ground[] = {
		-5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-5.0f, +5.0f, 0.0f, 0.0f, 0.0f, 0.0f,		
		+5.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		+5.0f, +5.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	GLushort indexforground[] = {
		0,1,2,
		1,2,3,
	};


	glBindVertexArray(VAOs[4]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexforground), indexforground, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


	//****************1. POINT *******STARTS******************
	const GLfloat point1[] = {
		-0.0f, -0.0f, -0.0f, //point position
		+0.5f, +0.5f, 0.5f, //point color

		-0.3f, -0.3f, -0.3f, //point position
		+0.0f, +0.0f, 0.0f, //point color

		+0.3f, +0.3f, +0.3f, //point position
		+0.0f, +0.0f, 0.0f, //point color

//×××××××××××××××××××××2.  COORDINATOR××××××××××××××××××××××××××××
		+0.0f, +0.2f, +0.3f, //starting point position
		+0.0f, +0.3f, 0.8f, //point color

		+0.8f, -0.8f, +0.3f, //ending point position
		+0.0f, +0.2f, 0.9f, //point color

		//X axel
		+0.0f, +0.0f, +0.0f, //point position
		+1.0f, +0.0f, 0.0f, //point color

		+1.0f, +0.0f, +0.0f, //point position
		+1.0f, +0.0f, 0.0f, //point color

		//Y axel
		+0.0f, +0.0f, +0.0f, //starting point position
		+0.0f, +1.0f, 0.0f, //point color

		+0.0f, +1.0f, +0.0f, //ending point position
		+0.0f, +1.0f, 0.0f, //point color

		//Z axel
		+0.0f, +0.0f, +0.0f, //starting point position
		+0.0f, +0.0f, +1.0f, //point color

		+0.0f, +0.0f, +1.0f, //ending point position
		+0.0f, +0.0f, 1.0f //point color

	};

	

	cout << "HAHA" << endl;


	//bind them with different objects respectively
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(point1), point1, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

	//××××××××××××××××3.Cubic *********************************
	//every cubic contains 6 squares * 2 triangles/square * 3 points/ triangle = 36 points
	const GLfloat cubic1[] = {

		-0.5f, -0.5f, +0.5f,  +1.0f, +0.0f, +0.0f,
		+0.5f, -0.5f, +0.5f,  +0.0f, +1.0f, +0.0f,
		+0.5f, +0.5f, +0.5f,  +1.0f, +1.0f, +0.0f,
		+0.5f, +0.5f, +0.5f,  +1.0f, +0.0f, +1.0f,
		-0.5f, +0.5f, +0.5f,  +1.0f, +0.0f, +1.0f,
		-0.5f, -0.5f, +0.5f,  +1.0f, +0.5f, +0.0f,

		-0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +0.0f,
		+0.5f, -0.5f, -0.5f,  +1.0f, +1.0f, +0.0f,
		+0.5f, +0.5f, -0.5f,  +0.0f, +1.0f, +1.0f,
		+0.5f, +0.5f, -0.5f,  +0.0f, +0.0f, +1.0f,
		-0.5f, +0.5f, -0.5f,  +0.0f, +1.0f, +1.0f,
		-0.5f, -0.5f, -0.5f,  +0.2f, +0.3f, +0.4f,
 
		-0.5f, +0.5f, +0.5f,  +1.0f, +1.0f, +0.0f,
		-0.5f, +0.5f, -0.5f,  +0.6f, +1.0f, +0.0f,
		-0.5f, -0.5f, -0.5f,  +1.0f, +1.0f, +0.0f,
		-0.5f, -0.5f, -0.5f,  +0.0f, +0.6f, +0.0f,
		-0.5f, -0.5f,  0.5f,  +0.0f, +0.8f, +0.6f,
		-0.5f,  0.5f,  0.5f,  +1.0f, +1.0f, +0.0f,
 
		+0.5f, +0.5f, +0.5f,  +1.0f, +0.4f, +1.0f,
		+0.5f, +0.5f, -0.5f,  +1.0f, +0.0f, +1.0f,
		+0.5f, -0.5f, -0.5f,  +0.0f, +0.4f, +1.0f,
		+0.5f, -0.5f, -0.5f,  +0.0f, +0.0f, +0.6f,
		+0.5f, -0.5f, +0.5f,  +0.0f, +0.0f, +0.8f,
		+0.5f,  0.5f, +0.5f,  +0.0f, +1.0f, +1.0f,
 
		-0.5f, -0.5f, -0.5f,  +1.0f, +0.9f, +0.4f,
		+0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +1.0f,
		+0.5f, -0.5f, +0.5f,  +0.0f, +1.0f, +0.3f,
		+0.5f, -0.5f, +0.5f,  +1.0f, +1.0f, +0.8f,
		-0.5f, -0.5f, +0.5f,  +1.0f, +0.2f, +1.0f,
		-0.5f, -0.5f, -0.5f,  +0.0f, +1.0f, +1.0f,
 
		-0.5f, +0.5f, -0.5f,  +1.0f, +0.4f, +0.0f,
		+0.5f, +0.5f, -0.5f,  +1.0f, +0.3f, +0.0f,
		+0.5f, +0.5f, +0.5f,  +1.0f, +0.8f, +0.0f,
		+0.5f, +0.5f, +0.5f,  +0.0f, +0.2f, +0.0f,
		-0.5f, +0.5f, +0.5f,  +0.0f, +1.0f, +0.0f,
		-0.5f, +0.5f, -0.5f,  +0.3f, +1.0f, +0.0f
	};




	glBindVertexArray(VAOs[1]);  //the VAO for point
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubic1), cubic1, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));



//**********************OBJECT3 Bank of China Tower ******************
	// GLfloat r = (float) ((double)rand() / (double)RAND_MAX);
	// GLfloat g = (float) ((double)rand() / (double)RAND_MAX);
	// GLfloat b = (float) ((double)rand() / (double)RAND_MAX);
	const GLfloat BOChk[] = {
		+0.0f, +0.0f, +0.0f, +1.0f, +0.0f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.0f, +1.0f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.0f, +0.0f, 1.0f,

		+0.0f, +0.0f, +0.0f, +1.0f, +1.0f, 0.0f,
		+0.0f, +0.0f, +1.0f, +0.0f, +0.0f, 0.6f,
		+1.0f, +0.0f, +0.0f, +0.2f, +0.0f, 0.5f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.0f, 0.0f,
		+1.0f, +0.0f, +1.0f, +0.0f, +0.8f, 1.0f,
		+1.0f, +1.0f, +0.0f, +0.9f, +0.0f, 0.0f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.0f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.0f, +0.2f, 1.0f,
		+1.0f, +1.0f, +1.0f, +1.0f, +0.3f, 0.0f,

		+1.0f, +1.0f, +0.0f, +0.0f, +0.6f, 0.0f,
		+1.0f, +1.0f, +1.0f, +1.0f, +0.0f, 1.0f,
		+0.0f, +1.0f, +0.0f, +0.2f, +0.0f, 1.0f, 

		+1.0f, +1.0f, +0.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +1.0f, +1.0f, +0.3f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.1f, +0.0f, 0.0f, 

		+0.0f, +1.0f, +0.0f, +0.4f, +0.0f, 0.0f, 
		+0.0f, +1.0f, +1.0f, +1.0f, +0.7f, 0.0f, 
		+0.0f, +0.0f, +0.0f, +0.9f, +0.0f, 0.4f, 

		+0.0f, +1.0f, +0.0f, +1.0f, +0.0f, 0.6f, 
		+0.0f, +1.0f, +1.0f, +0.0f, +0.7f, 0.0f, 
		+0.0f, +0.0f, +1.0f, +1.0f, +0.0f, 1.0f,

		+0.0f, +0.0f, +1.0f, +0.4f, +0.3, +0.9f,
		+1.0f, +0.0f, +1.0f, +0.2f, +0.0, +0.4f,
		+0.5f, +0.5f, +1.5f, +0.2f, +0.9, +0.0f,

		+1.0f, +0.0f, +1.0f, +0.2f, +0.0, +0.4f,
		+0.5f, +0.5f, +1.5f, +0.2f, +0.9, +0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,

		+1.0f, +0.0f, +1.0f, +0.2f, +0.0, +0.4f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,

		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,

		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,					

		+1.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,

		+1.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,		

		+0.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +0.0f, +1.0f, +0.4f, +0.0f, 0.0f,

		+0.0f, +1.0f, +1.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,

		+0.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +0.0f, +1.0f, +0.4f, +0.0f, 0.0f,
		+0.5f, +0.5f, +1.5f, +0.2f, +0.9, +0.0f,

		+0.5f, +0.5f, +1.5f, +0.2f, +0.9, +0.0f,
		+0.5f, +0.5f, +2.5f, +0.2f, +0.9, +0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +2.5f, +0.2f, +0.9, +0.0f,
		+1.0f, +0.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +1.5f, +0.2f, +0.9, +0.0f,
		+0.5f, +0.5f, +2.5f, +0.2f, +0.9, +0.0f,
		+0.0f, +0.0f, +2.0f, +0.2f, +0.9, +0.0f,

		+0.5f, +0.5f, +2.5f, +0.2f, +0.9f, +0.0f,
		+0.0f, +0.0f, +2.0f, +0.2f, +0.9f, +0.0f,
		+0.0f, +0.0f, +3.0f, +0.4f, +0.0f, +0.0f,

		+0.5f, +0.5f, +2.5f, +0.2f, +0.9f, +0.0f,	
		+0.0f, +0.0f, +3.0f, +0.4f, +0.0f, +0.0f,
		+0.5f, +0.5f, +3.5f, +0.2f, +0.9f, +1.0f,

		+0.0f, +0.0f, +3.0f, +0.4f, +0.0f, +0.0f,
		+0.5f, +0.5f, +3.5f, +0.2f, +0.9f, +1.0f,
		+0.0f, +1.0f, +3.0f, +0.1f, +0.0f, +0.0f,	

		+0.0f, +1.0f, +2.0f, +0.1f, +0.0f, +0.0f,
		+0.0f, +1.0f, +3.0f, +0.1f, +0.0f, +0.0f,
		+0.0f, +0.0f, +2.0f, +0.4f, +0.0f, +0.0f,

		+0.5f, +0.5f, +2.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +2.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +3.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +2.5f, +0.4f, +0.0f, +0.0f,				
		+0.5f, +0.5f, +3.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +3.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +3.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +3.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +4.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +4.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +4.0f, +0.4f, +0.0f, 0.0f,
		+1.0f, +1.0f, +5.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +4.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +5.0f, +0.4f, +0.0f, 0.0f,
		+0.5f, +0.5f, +5.5f, +0.4f, +0.0f, +0.0f,

		+0.5f, +0.5f, +5.5f, +0.4f, +0.0f, +0.0f,
		+1.0f, +1.0f, +5.0f, +0.4f, +0.0f, 0.0f,
		+0.0f, +1.0f, +5.0f, +0.4f, +0.0f, 0.0f,

		+0.5f, +0.5f, +5.5f, +0.4f, +1.0f, +0.0f,
		+0.0f, +1.0f, +5.0f, +0.4f, +0.0f, +0.0f,
		+0.5f, +0.5f, +4.5f, +0.4f, +1.0f, +0.0f,

		+0.5f, +0.5f, +4.5f, +0.1f, +1.0f, +0.0f,
		+0.0f, +1.0f, +4.0f, +0.0f, +0.0f, +0.0f,
		+0.5f, +0.5f, +3.5f, +0.4f, +0.0f, +1.0f,

		+0.5f, +0.5f, +3.5f, +0.3f, +0.0f, +1.0f,
		+0.0f, +0.0f, +3.0f, +0.2f, +0.0f, +1.0f,
		+0.0f, +1.0f, +3.0f, +0.9f, +0.0f, +0.0f,		

		+0.0f, +1.0f, +5.0f, +0.3f, +0.0f, +1.0f,
		+0.0f, +1.0f, +4.0f, +0.2f, +0.0f, +1.0f,
		+1.0f, +1.0f, +4.0f, +0.9f, +0.0f, +0.0f,

		+0.0f, +1.0f, +4.0f, +0.3f, +0.0f, +1.0f,
		+0.0f, +1.0f, +3.0f, +0.2f, +0.0f, +1.0f,
		+1.0f, +1.0f, +3.0f, +0.9f, +0.0f, +0.0f,		

	};



	glBindVertexArray(VAOs[2]);  //the VAO for point
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BOChk), BOChk, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

















}

void paintGL(void)
{
	//TODO:
	//render your objects and control the transformation here
	glClearColor(0.94902f,0.92941f,0.82353f,0.7f); // set the background color
	glClear(GL_COLOR_BUFFER_BIT);



//************************ FIRST OBJECT, some points and coordinate system ********************************
	glm::mat4 scaleMatrix;
	scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(scale));


	glm::mat4 worldView;
	worldView = glm::lookAt(
		glm::vec3(1 + x_press_num, -5 + y_press_num, 3 + z_press_num), // position of camera in world space
		glm::vec3(1+lookX,1+lookY,1+lookZ), // gaze direction
		glm::vec3(0,1,0) ); // No idea yet....
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3( 0.0f, 0.0f, 0.0f)); //slightly shifted

	cout << "Camera Position" <<endl;
	cout << "X: " << 1 + x_press_num << "   Y: " << -5 + y_press_num << "     Z: " << 3 + z_press_num << "  Scale: " << scale << endl;

	cout << (double)rand() / (double)RAND_MAX << endl;
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.1f, 100.0f);

	glm::mat4 mvp = projection * worldView * modelTransformMatrix * scaleMatrix;
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//******************GROUND**************************************************
	glm::mat4 modelTransformMatrix0 = glm::mat4(1.0f);
	modelTransformMatrix0 = glm::translate(glm::mat4(), glm::vec3( 0.0f, 0.0f, 0.0f)); 

	glm::mat4 mvp0 = projection * worldView * modelTransformMatrix0 * scaleMatrix;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp0[0][0]);

	glBindVertexArray(VAOs[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

//***********************************************************************



	glBindVertexArray(VAOs[0]);  //the VAO for point
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, 3); //render primitives from array data
	//NOTICE: one stride has 6 floating point data, so one point's length is 1

	//glLineWidth(5);
	glDrawArrays(GL_LINES, 3, 8); //render primitives from array data
	//NOTICE: one stride has 6 floating point data, so one line's length is 2

//************************ SECOND OBJECT, cubic ********************************
	glm::mat4 modelTransformMatrix2 = glm::mat4(1.0f);
	modelTransformMatrix2 = glm::translate(glm::mat4(), glm::vec3( 2.0f, 2.0f, 2.0f)); //slightly shifted

	glm::mat4 mvp2 = projection * worldView * modelTransformMatrix2 * scaleMatrix;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);

	glBindVertexArray(VAOs[1]);
	//glDrawArrays(GL_TRIANGLES, 0, 36);


	//************************ THIRD OBJECT, BOCHK ********************************
	glm::mat4 modelTransformMatrix3 = glm::mat4(1.0f);
	modelTransformMatrix3 = glm::translate(glm::mat4(), glm::vec3( 1.0f, 1.0f, 0.0f)); //slightly shifted

	glm::mat4 mvp3 = projection * worldView * modelTransformMatrix3 * scaleMatrix;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp3[0][0]);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(5);
	glBindVertexArray(VAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 108);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glLineWidth(1);

	sleep(0.8);
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
	glutInitWindowSize(800, 800);
	glutCreateWindow("Assignment 1");
	glewInit();

	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */
	initializedGL();
	glutDisplayFunc(paintGL);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);

	/*Enter the GLUT event processing loop which never returns.
	it will call different registered CALLBACK according
	to different events. */


	glutMainLoop();

	return 0;
}