
#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
//#include <glad/glad.h>
//what is glad?
//one question here about -lglfw   -lglfw3 in compiling


#include <GL/glx.h>    
#include <GL/gl.h>
#include <GL/glut.h>

#include <iostream>
#include <stdlib.h>

using namespace std;

void myDisplay(void){
    glClear(GL_COLOR_BUFFER_BIT);
    glColor3f(0.0f,1.0f,1.0f);
    glRectf(-0.5f,-0.5f,0.5f,0.5f);
    glFlush();
}

void myIdle(void){
    glutPostRedisplay();
}

int main(int argc, char *argv[]){


    glutInit(&argc, argv);
    glutInitDisplayMode( GLUT_SINGLE);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(640,480);
    glutCreateWindow("InitialTry!");
    glutDisplayFunc(myDisplay);
    glutIdleFunc(myIdle);
    glutMainLoop();



}