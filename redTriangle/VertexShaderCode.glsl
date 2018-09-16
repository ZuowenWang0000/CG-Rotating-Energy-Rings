#version 120
attribute vec3 position;
attribute vec3 vertexColor;

uniform mat4 modelTransformMatrix;


varying vec4 dadaColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 new_position = modelTransformMatrix * v;
	gl_Position = new_position;	


	dadaColor = vec4(vertexColor, 1.0);
}