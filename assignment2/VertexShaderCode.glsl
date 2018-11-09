#version 430  // GLSL version your computer supports

layout(location = 0) in vec3 Position_modelspaceVertex;
layout(location = 1) in vec2 UVvertex;
layout(location = 2) in vec3 normalDirec;


out vec2 UV;

uniorm mat4 MVP;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = MVP*v;


	UV = UVvertex;
}
