#version 430  // GLSL version your computer supports

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexUV;
in layout(location = 2) vec3 vertexNormal;


out vec2 UV;
out vec3 normal;
out vec3 vertexPositionWorld;

uniform mat4 MVP;
uniform vec3 ambientLight;


void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = MVP*v;

	normal = (MVP*vec4(vertexNormal, 0)).xyz;
	vertexPositionWorld = (MVP*v).xyz;

	theColor = ambientLight;
	UV = vertexUV;
}
