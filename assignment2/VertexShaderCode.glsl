#version 430  // GLSL version your computer supports

in layout(location = 0) vec3 position;
in layout(location = 1) vec2 vertexUV;
in layout(location = 2) vec3 vertexNormal;

uniform vec3 ambientLight;

uniform mat4 MVP;

out vec2 UV;

void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = MVP*v;
	UV = vertexUV;

}
