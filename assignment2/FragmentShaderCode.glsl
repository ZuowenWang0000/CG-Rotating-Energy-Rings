#version 430 //GLSL version your computer supports

out vec4 daColor;

in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform vec3 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;

uniform sampler2D texture0;
uniform vec3 diffuseStrength;
uniform vec3 specularStrength;

void main()
{

	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = max(dot(lightVectorWorld, normalize(normalWorld)), 0.0);
	vec3 diffuseLight = vec3(brightness, brightness, brightness);
	diffuseLight = clamp(diffuseStrength * diffuseLight, 0, 1);

	//specular light
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s = clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	vec3 specularLight = vec3(s, s, s);
	specularLight = specularStrength*specularLight;

	daColor = vec4((texture(texture0, UV).rgb)*(ambientLight + diffuseLight + specularLight), 1.0);
	//daColor = vec4(texture(texture0, UV).rgb * ambientLight, 1.0);
	//daColor = vec4((texture(texture0, UV).rgb)*(diffuseLight*diffAdjust), 1.0);
	//daColor = vec4((texture(texture0, UV).rgb)*(specularLight + vec3(0.5,0.5,0.5)), 1.0);

}


