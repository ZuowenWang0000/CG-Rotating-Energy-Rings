#version 430 //GLSL version your computer supports

out vec4 daColor;
in vec2 UV;  //passing UV from vertex shader to fragment shader


uniform sampler2D texture0plane;


void main()
{
	daColor = texture(texture0plane, UV).rgb;	

}


