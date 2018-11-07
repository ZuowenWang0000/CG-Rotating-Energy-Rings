#version 430  // GLSL version your computer supports

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;
in layout(location = 2) vec3 normal;


uniform vec3 ambientLight;



uniform mat4 fullTransformMatrix;
uniform mat4 modelTranformMatrix;
unifrom mat4 projectionMatrix;


out vec3 theColor;
out vec3 normalWorld;
out vec3 vertexPositionWorld;




void main()
{
	vec4 v = vec4(position, 1.0);
	gl_Position = fullTransformMatrix * v;
	theColor = vertexColor * ambientLight;	

	vec4 normal_temp = modelTransformMatrix * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	vertexPositionWorld = newPosition.xyz;
}