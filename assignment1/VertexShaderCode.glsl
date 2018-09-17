#version 130  // GLSL version your computer supports
//#extension ARB_explicit_attrib_location : require

in vec3 position;
in vec3 vertexColor;

uniform mat4 MVP;

out vec3 theColor;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 new_position = MVP * v;
	gl_Position = new_position;	
	theColor = vertexColor;
}