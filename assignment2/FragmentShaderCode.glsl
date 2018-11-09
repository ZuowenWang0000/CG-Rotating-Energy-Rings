#version 430 //GLSL version your computer supports

out vec3 daColor;

in vec2 UV;  //passing UV from vertex shader to fragment shader
in vec3 normal;  //normal world
in vec3 vertexPositionWorld;

uniform vec4 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;
uniform vec3 diffuseLight;

uniform sampler2D texture0plane;


void main()
{
	float bright = dot(normalize(lightPositionWorld - vertexPositionWorld), normalize(normal));
	vec4 diff = vec4(bright, bright, bright, 1.0);




	daColor = ambientLight*(0.5*texture(texture0plane, UV).rgb) + //this is ambient light
			  diffuseLight*(0.5*texture(texture0plane, UV).rgb)*clamp(diff, 0, 1); // diffuse light

}


