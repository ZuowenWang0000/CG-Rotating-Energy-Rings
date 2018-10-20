///////////////////////////////////////////
////Type your name and student ID here/////
////Name: Zuowen Wang	
////Student ID: 1155123906
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
float i_press_num = 0;
float k_press_num = 0;
float j_press_num = 0;
float l_press_num = 0;
int angle_press_num = 0;

double lookX = 0;
double lookY = 0;
double lookZ = 0;
double rot = 0;

float scale = 4.5f;

GLfloat pitch = 22.0f;
GLfloat yaw = -90.0f;

glm::vec3 cameraPosition = glm::vec3(-1.5f,-3.53489f,42.98063f);
glm::vec3 cameraGaze = glm::vec3(0.0f,0.01f,-0.99f);

GLuint VBOs[5], VAOs[5];
GLuint EBO[2];

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
		cameraPosition -= 0.5f *glm::normalize(glm::cross(cameraGaze,glm::vec3(0,1,0)));
	}
	if (key == 'd')
	{
		cameraPosition += 0.5f *glm::normalize(glm::cross(cameraGaze,glm::vec3(0,1,0)));
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
		cameraPosition -= 0.5f *glm::vec3(0.0f,0.0f,1.0f);
	}
	if (key == 'q')
	{
		cameraPosition += 0.5f *glm::vec3(0.0f,0.0f,1.0f);
	}

	// // and 2 angle operations:
	// z-> camera angle up 
	// x -> camera angle down
		if (key == '+')
	{
		scale += 0.5;
	}
	if (key == '-')
	{
		scale -= 0.5;
	}

	if(key == 'r'){
		rot = rot + 3;
	}
	if(key == 'i'){
		i_press_num = i_press_num + 0.5;
	}
	if(key == 'k'){
		k_press_num = k_press_num - 0.5;
	}
	if(key == 'j'){
		j_press_num = j_press_num - 0.5;
	}
	if(key=='l'){
		l_press_num = l_press_num + 0.5;
	}
}

void mouse(int button, int state, int x, int y){

  if (button == 3 ) {
	  pitch = pitch + 2.0f;
  }
  if (button == 4){
	  pitch = pitch - 2.0f;
  }
  if (button == GLUT_LEFT_BUTTON){
	  yaw = yaw - 5.0f;
  }
  if (button == GLUT_RIGHT_BUTTON){
	  yaw = yaw + 5.0f;
  }


  if(pitch > 89.0f){pitch = 89.0f;} // to avoid deadlock
  if(pitch < -89.0f){pitch = -89.0f;}
  	 glm::vec3 temp = glm::vec3(
		cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
		sin(glm::radians(pitch)),
		sin(glm::radians(yaw)) * cos(glm::radians(pitch)));

	cameraGaze = glm::normalize(temp);

}

void sendDataToOpenGL()
{
	//TODO:
	//create point, line, 2D object and 3D object here and bind to VAOs & VBOs
	//create VAOs VBOs 
 	glGenVertexArrays(5,VAOs);	
	glGenBuffers(5,VBOs);
	glGenBuffers(2, EBO);

	//****************0. GROUND *********************
	//using indexing
	const GLfloat ground[] = {
		-7.0f, -7.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-7.0f, +7.0f, 0.0f, 0.0f, 0.0f, 0.0f,		
		+7.0f, -7.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		+7.0f, +7.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	};

	GLushort indexforground[] = {
		0,1,2,
		1,2,3,
	};

	glBindVertexArray(VAOs[4]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground), ground, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexforground), indexforground, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));

//××××××××××××××××××××× 1. "HK" lines××××××××××××××××××××××××××××
	const GLfloat point1[] = {
		0.0f,-1.0f,0.1f,	+1.0f, +0.0f, +0.0f,
		0.0f,+1.0f,0.1f,	+1.0f, +0.0f, +0.0f,

		1.0f,-1.0f,0.1f,	+1.0f, +0.0f, +0.0f,
		1.0f,+1.0f,0.1f,	+1.0f, +0.0f, +0.0f,

		0.0f,0.0f,0.1f,		+1.0f, +0.0f, +0.0f,
		1.0f,0.0f,0.1f,		+1.0f, +0.0f, +0.0f,

		1.5f,-1.0f,0.1f,	+1.0f, +0.0f, +0.0f,
		1.5f,+1.0f,0.1f,	+1.0f, +0.0f, +0.0f,

		1.5f, 0.0f,0.1f,	+1.0f, +0.0f, +0.0f,
		2.5f, 1.0f,0.1f,	+1.0f, +0.0f, +0.0f,

		1.5f, 0.0f,0.1f,	+1.0f, +0.0f, +0.0f,
		2.5f, -1.0f,0.1f,	+1.0f, +0.0f, +0.0f,	

	//POINTS

		-0.5f, 0.0f, 0.1f, 	+1.0f, +0.0f, +0.0f,
		2.8f, 0.0f, 0.1f, +1.0f, +0.0f, +0.0f
	};
	//bind them with different objects respectively
	glBindVertexArray(VAOs[3]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
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


//**********************2.Bank of China Tower ******************

	const GLfloat BOChk[] = {
		+0.0f, +0.0f, +0.0f, +0.3f, +0.4f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.3f, +0.4f, 1.0f,

		+0.0f, +0.0f, +0.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.3f, +0.4f, 1.0f,
		+1.0f, +0.0f, +0.0f, +0.3f, +0.5f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.3f, +0.6f, 1.0f,
		+1.0f, +1.0f, +0.0f, +0.3f, +0.5f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.3f, +0.6f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.3f, +0.5f, 1.0f,

		+1.0f, +1.0f, +0.0f, +0.3f, +0.6f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +1.0f, +0.0f, +0.3f, +0.5f, 1.0f,

		+1.0f, +1.0f, +0.0f, +0.4f, +0.5f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.3f, +0.7f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +1.0f, +0.0f, +0.3f, +0.6f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +0.0f, +0.25f, +0.5f, 1.0f,

		+0.0f, +1.0f, +0.0f, +0.3f, +0.6f, 1.0f, 
		+0.0f, +1.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.3f, +0.7f, 1.0f,

		+0.0f, +0.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.3f, +0.6f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.3f, +0.7f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.3f, +0.6f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.3f, +0.7f, 1.0f,

		// +1.0f, +0.0f, +1.0f, +0.3f, +0.6f, 1.0f,
		// +1.0f, +1.0f, +2.0f, +0.2f, +0.5f, 1.0f,
		// +1.0f, +0.0f, +2.0f, +0.2f, +0.5f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.2f, +0.5f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.2f, +0.5f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.2f, +0.55f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.55f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.7f, 1.0f,				

		+1.0f, +1.0f, +1.0f, +0.5f, +0.7f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.2f, +0.7f, 1.0f,

		+1.0f, +1.0f, +1.0f, +0.5f, +0.6f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.7f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.3f, +0.5f, 1.0f,		

		+0.0f, +1.0f, +1.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.2f, +0.7f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +1.0f, +1.0f, +0.3f, +0.7f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +0.0f, +2.0f, +0.3f, +0.7f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.4f, +0.5f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.3f, +0.7f, 1.0f,

		+0.5f, +0.5f, +1.5f, +0.4f, +0.7f, 1.0f,
		+0.5f, +0.5f, +2.5f, +0.3f, +0.5f, 0.9f,
		+1.0f, +0.0f, +2.0f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.3f, +0.5f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +1.5f, +0.3f, +0.5f, 1.0f,
		+0.5f, +0.5f, +2.5f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.3f, +0.6f, 0.9f,
		+0.0f, +0.0f, +2.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +3.0f, +0.3f, +0.6f, 0.8f,

		+0.5f, +0.5f, +2.5f, +0.3f, +0.6f, 0.8f,	
		+0.0f, +0.0f, +3.0f, +0.3f, +0.5f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.5f, +0.6f, 0.9f,

		+0.0f, +0.0f, +3.0f, +0.4f, +0.5f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.5f, +0.6f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.5f, +0.5f, 0.9f,

		+0.0f, +1.0f, +2.0f, +0.5f, +0.5f, 0.8f,
		+0.0f, +1.0f, +3.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.5f, +0.5f, 0.9f,

		+0.5f, +0.5f, +2.5f, +0.5f, +0.5f, 0.95f,
		+1.0f, +1.0f, +2.0f, +0.3f, +0.6f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.5f, +0.5f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.35f, +0.6f, 1.0f,			
		+0.5f, +0.5f, +3.5f, +0.3f, +0.5f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +3.5f, +0.4f, +0.5f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.35f, +0.5f, 1.0f,
		+1.0f, +1.0f, +4.0f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.4f, +0.5f, 1.0f,
		+1.0f, +1.0f, +4.0f, +0.3f, +0.5f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.5f, +0.5f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.5f, +0.5f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.5f, +0.5f, 1.0f,
		+0.5f, +0.5f, +5.5f, +0.3f, +0.5f, 1.0f,

		+0.5f, +0.5f, +5.5f, +0.5f, +0.5f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.5f, +0.5f, 1.0f,
		+0.0f, +1.0f, +5.0f, +0.3f, +0.6f, 1.0f,

		+0.5f, +0.5f, +5.5f, +0.5f, +0.6f, 1.0f,
		+0.0f, +1.0f, +5.0f, +0.2f, +0.6f, 0.9f,
		+0.5f, +0.5f, +4.5f, +0.4f, +0.5f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.25f, +0.6f, 0.95f,
		+0.0f, +1.0f, +4.0f, +0.2f, +0.7f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.3f, +0.8f, 0.9f,

		+0.5f, +0.5f, +3.5f, +0.4f, +0.5f, 1.0f,
		+0.0f, +0.0f, +3.0f, +0.3f, +0.5f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.3f, +0.6f, 0.9f,		

		+0.0f, +1.0f, +5.0f, +0.4f, +0.6f, 1.0f,
		+0.0f, +1.0f, +4.0f, +0.3f, +0.5f, 0.9f,
		+1.0f, +1.0f, +4.0f, +0.3f, +0.7f, 1.0f,

		+0.0f, +1.0f, +4.0f, +0.3f, +0.5f, 0.9f,
		+0.0f, +1.0f, +3.0f, +0.3f, +0.8f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +0.0f, +0.0f, +0.3f, +0.5f, 0.9f,
		+0.0f, +0.0f, +1.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +0.0f, +1.0f, +0.3f, +0.5f, 0.9f,
		+0.0f, +0.0f, +2.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.4f, +0.5f, 1.0f,

		+0.0f, +0.0f, +2.0f, +0.3f, +0.5f, 0.9f,
		+0.0f, +0.0f, +3.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.4f, +0.5f, 1.0f,	

		+0.5f, +0.5f, +3.5f, +0.3f, +0.5f, 0.9f,
		+0.0f, +0.0f, +3.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.4f, +0.5f, 1.0f,

		+0.5f, +0.5f, +3.5f, +0.3f, +0.5f, 0.9f,
		+0.0f, +1.0f, +3.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +4.0f, +0.4f, +0.5f, 1.0f,	

		+0.5f, +0.5f, +4.5f, +0.3f, +0.5f, 0.9f,
		+0.0f, +1.0f, +4.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +1.0f, +5.0f, +0.4f, +0.5f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.3f, +0.5f, 0.9f,
		+0.5f, +0.5f, +3.5f, +0.3f, +0.8f, 1.0f,
		+1.0f, +1.0f, +4.0f, +0.4f, +0.5f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.5f, 0.9f,
		+1.0f, +0.0f, +1.0f, +0.3f, +0.8f, 1.0f,
		+0.0f, +0.0f, +0.0f, +0.6f, +0.5f, 1.0f,	

		+1.0f, +0.0f, +1.0f, +0.2f, +0.5f, 0.95f,
		+1.0f, +0.0f, +2.0f, +0.3f, +0.8f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.6f, +0.5f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.3f, +0.5f, 0.9f,
		+1.0f, +1.0f, +0.0f, +0.3f, +0.8f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.4f, +0.5f, 1.0f,		

	};


	glBindVertexArray(VAOs[2]); 
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BOChk), BOChk, GL_STATIC_DRAW);
	//pass the position of the point to openGL
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	//pass the color of the point to openGL
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (char*)(3 * sizeof(float)));


	const GLfloat BOChkFrame[] = {	
		+0.0f, +0.0f, +0.0f, +0.8f, +0.8f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.8f, +0.8f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.8f, +0.8f, 1.0f,

		+0.0f, +0.0f, +0.0f, +0.8f, +0.8f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+1.0f, +0.0f, +0.0f, +0.8f, +0.8f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.8f, +0.9f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.8f, +0.8f, 1.0f,
		+1.0f, +1.0f, +0.0f, +0.8f, +0.9f, 1.0f,

		+1.0f, +0.0f, +0.0f, +0.8f, +0.8f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.8f, +0.9f, 1.0f,

		+1.0f, +1.0f, +0.0f, +0.8f, +0.8f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+0.0f, +1.0f, +0.0f, +0.8f, +0.8f, 1.0f,

		+1.0f, +1.0f, +0.0f, +0.8f, +0.9f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.8f, +0.8f, 1.0f,

		+0.0f, +1.0f, +0.0f, +0.8f, +0.9f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+0.0f, +0.0f, +0.0f, +0.85f, +0.8f, 1.0f,

		+0.0f, +1.0f, +0.0f, +0.8f, +0.8f, 1.0f, 
		+0.0f, +1.0f, +1.0f, +0.8f, +0.8f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.8f, +0.9f, 1.0f,

		+0.0f, +0.0f, +1.0f, +0.8f, +0.8f, 1.0f,
		+1.0f, +0.0f, +1.0f, +0.8f, +0.9f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.8f, +0.8f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.9f, +0.8f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.9f, +0.9f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.9f, +0.8f, 1.0f,

		// +1.0f, +0.0f, +1.0f, +0.9f, +0.8f, 1.0f,
		// +1.0f, +0.0f, +2.0f, +0.9f, +0.8f, 1.0f,
		// +0.5f, +0.5f, +1.5f, +0.9f, +0.9f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.9f, +0.8f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.9f, +0.9f, 1.0f,
		+1.0f, +1.0f, +1.0f, +0.9f, +0.8f, 1.0f,

		+1.0f, +0.0f, +1.0f, +0.9f, +0.95f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.9f, +0.8f, 1.0f,				

		+1.0f, +1.0f, +1.0f, +0.9f, +0.9f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.9f, +0.8f, 1.0f,
		+0.0f, +1.0f, +1.0f, +0.9f, +0.8f, 1.0f,

		+1.0f, +1.0f, +1.0f, +0.9f, +0.8f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.9f, +0.87f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.9f, +0.95f, 1.0f,		

		+0.0f, +1.0f, +1.0f, +0.9f, +0.85f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.9f, +0.87f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.9f, +0.85f, 1.0f,

		+0.0f, +1.0f, +1.0f, +0.9f, +0.87f, 1.0f,
		+0.0f, +1.0f, +2.0f, +0.9f, +0.85f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.9f, +0.85f, 1.0f,

		+0.0f, +0.0f, +2.0f, +0.9f, +0.97f, 1.0f,
		+0.0f, +0.0f, +1.0f, +0.9f, +0.85f, 1.0f,
		+0.5f, +0.5f, +1.5f, +0.9f, +0.87f, 1.0f,

		+0.5f, +0.5f, +1.5f, +0.9f, +0.87f, 1.0f,
		+0.5f, +0.5f, +2.5f, +0.9f, +0.85f, 0.9f,
		+1.0f, +0.0f, +2.0f, +0.9f, +0.9f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.9f, +0.85f, 1.0f,
		+1.0f, +0.0f, +2.0f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +2.0f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +1.5f, +0.87f, +0.85f, 1.0f,
		+0.5f, +0.5f, +2.5f, +0.87f, +0.85f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.87f, +0.8f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.85f, +0.86f, 0.9f,
		+0.0f, +0.0f, +2.0f, +0.85f, +0.85f, 1.0f,
		+0.0f, +0.0f, +3.0f, +0.9f, +0.86f, 0.8f,

		+0.5f, +0.5f, +2.5f, +0.9f, +0.86f, 0.8f,	
		+0.0f, +0.0f, +3.0f, +0.9f, +0.85f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.9f, +0.86f, 0.9f,

		+0.0f, +0.0f, +3.0f, +0.9f, +0.95f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.9f, +0.96f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.9f, +0.85f, 0.9f,

		+0.0f, +1.0f, +2.0f, +0.9f, +0.85f, 0.8f,
		+0.0f, +1.0f, +3.0f, +0.9f, +0.85f, 1.0f,
		+0.0f, +0.0f, +2.0f, +0.9f, +0.85f, 0.9f,

		+0.5f, +0.5f, +2.5f, +0.9f, +0.85f, 0.95f,
		+1.0f, +1.0f, +2.0f, +0.9f, +0.86f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +2.5f, +0.95f, +0.86f, 1.0f,			
		+0.5f, +0.5f, +3.5f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +3.5f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.95f, +0.85f, 1.0f,
		+1.0f, +1.0f, +4.0f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +4.0f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.9f, +0.95f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.9f, +0.85f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.9f, +0.85f, 1.0f,
		+0.5f, +0.5f, +5.5f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +5.5f, +0.9f, +0.95f, 1.0f,
		+1.0f, +1.0f, +5.0f, +0.9f, +0.85f, 1.0f,
		+0.0f, +1.0f, +5.0f, +0.9f, +0.86f, 1.0f,

		+0.5f, +0.5f, +5.5f, +0.9f, +0.86f, 1.0f,
		+0.0f, +1.0f, +5.0f, +0.9f, +0.86f, 0.9f,
		+0.5f, +0.5f, +4.5f, +0.9f, +0.85f, 1.0f,

		+0.5f, +0.5f, +4.5f, +0.95f, +0.86f, 0.95f,
		+0.0f, +1.0f, +4.0f, +0.9f, +0.78f, 1.0f,
		+0.5f, +0.5f, +3.5f, +0.9f, +0.88f, 0.9f,

		+0.5f, +0.5f, +3.5f, +0.9f, +0.85f, 1.0f,
		+0.0f, +0.0f, +3.0f, +0.9f, +0.85f, 1.0f,
		+0.0f, +1.0f, +3.0f, +0.9f, +0.96f, 0.9f,		

		+0.0f, +1.0f, +5.0f, +0.9f, +0.86f, 1.0f,
		+0.0f, +1.0f, +4.0f, +0.9f, +0.95f, 0.9f,
		+1.0f, +1.0f, +4.0f, +0.9f, +0.87f, 1.0f,

		+0.0f, +1.0f, +4.0f, +0.9f, +0.75f, 0.9f,
		+0.0f, +1.0f, +3.0f, +0.9f, +0.78f, 1.0f,
		+1.0f, +1.0f, +3.0f, +0.9f, +0.85f, 1.0f,

		+0.4, +0.58, +5.45, +0.9f, +0.85f, 0.9f,
		+0.4, +0.58, +6.05, +0.8f, +0.95f, 0.9f,

		+0.6, +0.58, +5.45, +0.9f, +0.85f, 0.9f,
		+0.6, +0.58, +6.05, +0.8f, +0.95f, 0.9f,

		+0.4, +0.58, +5.53, +0.8f, +0.95f, 0.9f,
		+0.6, +0.58, +5.53, +0.9f, +0.85f, 0.9f,

		+0.4, +0.58, +5.53, +0.8f, +0.95f, 0.9f,
		+0.5, +0.5, +5.5, +0.8f, +0.95f, 0.9f,

		+0.6, +0.58, +5.53, +0.9f, +0.85f, 0.9f,
		+0.5, +0.5, +5.5, +0.8f, +0.95f, 0.9f,

	};

//in total 36 triangles
	GLushort indexforground2[] = {
		0,1,  1,2,  0,2,
		3,4,  4,5,  3,5,
		6,7,  7,8,  6,8,
		9,10,  10,11,  9,11,
		12,13,  13,14,  12,14,
		15,16,  16,17,  15,17,
		18,19,  19,20,  18,20,
		21,22,  22,23,  21,23,
		24,25,  25,26,  24,26,
		27,28,  28,29,  27,29,
		30,31,  31,32,  30,32,
		33,34,  34,35,  33,35,
		36,37,  37,38,  36,38,
		39,40,  40,41,  39,41,
		42,43,  43,44,  42,44,
		45,46,  46,47,  45,47,
		48,49,  49,50,  48,50,
		51,52,  52,53,  51,53,
		54,55,  55,56,  54,56,
		57,58,  58,59,  57,59,
		60,61,  61,62,  60,62,
		63,64,  64,65,  63,65,
		66,67,  67,68,  66,68,
		69,70,  70,71,  69,71,
		72,73,  73,74,  72,74,
		75,76,  76,77,  75,77,
		78,79,  79,80,  78,80,
		81,82,  82,83,  81,83,
		84,85,  85,86,  84,86,
		87,88,  88,89,  87,89,
		90,91,  91,92,  90,92,
		93,94,  94,95,  93,95,
		96,97,  97,98,  96,98,
		99,100,  100,101,  99,101,
		102,103,  103,104,  102,104,

		105,106,107,108,109,110,111,112,112,113
	};


//***************** FRAME OF BOC Tower**********************
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(BOChkFrame), BOChkFrame, GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexforground2), indexforground2, GL_STATIC_DRAW);
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
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



//************************ FIRST OBJECT, some points and coordinate system ********************************
	glm::mat4 scaleMatrix;
	scaleMatrix = glm::scale(glm::mat4(1.0f),glm::vec3(scale));

	glm::mat4 worldView;
	worldView = glm::lookAt(
		cameraPosition, // position of camera in world space
		//glm::vec3(0+x_press_num+lookX,1+y_press_num +lookY,1+z_press_num+lookZ), // gaze direction
		//glm::vec3(1,1,1),
		cameraGaze + cameraPosition,
		//glm::vec3(1,-5,1),
		glm::vec3(0,1,0) );
	
	cout << "camera position: " << cameraPosition.x << "," <<cameraPosition.y << "," << cameraPosition.z  << endl;
	cout << "gaze: " << cameraGaze.x << "," <<cameraGaze.y << "," << cameraGaze.z  << endl;

	
	// glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	// modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3( 0.0f, 0.0f, 0.0f)); //slightly shifted

	cout << "pitch: "<< pitch << endl;
	cout << "yaw: " << yaw << endl;

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(75.0f), 800.0f / 800.0f, 0.1f, 100.0f);

	glm::mat4 mvp = projection * worldView  * scaleMatrix;
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp[0][0]);


//******************GROUND**************************************************
	glm::mat4 modelTransformMatrix0 = glm::mat4(1.0f);
	modelTransformMatrix0 = glm::translate(glm::mat4(), glm::vec3( 0.0f, 0.0f, -0.1f)); 

	glm::mat4 mvp0 = projection * worldView * scaleMatrix * modelTransformMatrix0 ;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp0[0][0]);

	glBindVertexArray(VAOs[4]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[0]);  //using indexing
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

//***********************LINES***********************************
	glLineWidth(10);
	glm::mat4 modelTransformMatrix1 = glm::mat4(1.0f);
	modelTransformMatrix1 = glm::translate(glm::mat4(), glm::vec3( -4.0f + j_press_num + l_press_num, 4.0f + i_press_num + k_press_num, 0.2f));
	glm::mat4 mvp1 = projection * worldView * scaleMatrix * modelTransformMatrix1;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp1[0][0]);

	//modelTransformMatrix1 = glm::rotate(modelTransformMatrix1,45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	glBindVertexArray(VAOs[3]);  //the VAO for lines 
	glDrawArrays(GL_LINES, 0, 12); //render primitives from array data
	glPointSize(10);

	glDrawArrays(GL_POINTS, 12,4);
	//NOTICE: one stride has 6 floating point data, so one line's length is 2

//************************ SECOND OBJECT, cubic ********************************
	glm::mat4 modelTransformMatrix2 = glm::mat4(1.0f);
	modelTransformMatrix2 = glm::translate(glm::mat4(), glm::vec3( -2.0f, -2.0f, 2.0f));
	modelTransformMatrix2 = glm::rotate(modelTransformMatrix2, (float)rot , glm::vec3(0.0f, 1.0f, 0.0f)); //slightly shifted and rotate 
	glLineWidth(7);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glm::mat4 mvp2 = projection * worldView * scaleMatrix * modelTransformMatrix2;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);

	glBindVertexArray(VAOs[1]);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//************************ THIRD OBJECT, BOCHK ********************************
	glm::mat4 modelTransformMatrix3 = glm::mat4(1.0f);
	modelTransformMatrix3 = glm::translate(glm::mat4(), glm::vec3( 1.0f, 1.0f, 0.01f)); //slightly shifted

	glm::mat4 mvp3 = projection * worldView * scaleMatrix * modelTransformMatrix3;
	//GLuint MatrixID2 = glGetUniformLocation(programID, "MVP2");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp3[0][0]);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(8);
	glBindVertexArray(VAOs[2]);
	glDrawArrays(GL_TRIANGLES, 0, 160);
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[1]);  //using indexing
	glDrawElements(GL_LINES, 220, GL_UNSIGNED_SHORT, 0);


	// glDrawElements(GL_LINE_SMOOTH,2,GL_UNSIGNED_SHORT, 0);	

	glLineWidth(1);

	// sleep();
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
	glutInitWindowSize(1000, 1000);
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