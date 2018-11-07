#version 430 //GLSL version your computer supports

out vec4 daColor;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec4 ambientLight;
unifrom vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;


void main()
{
	//Specular
	//calculate reflect light direction
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	//calculate direction from eye to object
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	//calculate light brightness according to the angle between eye and reflect light
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0 , 1);
	//control lobe of specularLight
	s = pow(s, 50);
	vec4 specularLight = vec4(s, 0 , 0 , 1);
	
	daColor = ambientLight + clamp(diffuseLight, 0, 1) + specularLight;
}
