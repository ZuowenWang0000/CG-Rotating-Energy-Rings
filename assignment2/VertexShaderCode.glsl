#version 430  // GLSL version your computer supports

in layout(location = 0) vec3 position;  //vbo
in layout(location = 1) vec2 vertexUV;  //uvbo
in layout(location = 2) vec3 vertexNormal; //nvbo

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;

uniform mat4 MVP;

out vec2 UV;
out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 newPosition = modelTransformMatrix * v;
	vec4 projectedPosition = projectionMatrix * newPosition;

	//calculate world coordinate of vertex and normal
	vec4 normal_temp = modelTransformMatrix * vec4(vertexNormal, 0);
	normalWorld = normal_temp.xyz;
	vertexPositionWorld = newPosition.xyz;

	gl_Position = projectionMatrix*modelTransformMatrix*v;
	UV = vertexUV;

}
