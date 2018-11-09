#version 430 //GLSL version your computer supports

out vec3 daColor;

in vec2 UV;

uniform vec4 ambientLight;
uniform vec3 diffuseLight;

uniform sampler2D texture0plane;
uniform sampler2D texture1jeep;
uniform sampler2D texture2block;


void main()
{

	daColor = (texture(texture0plane, UV).rgb)  ;

}


