#version 330 core
in vec2 texCoord;
in vec3 normal;
in vec3 fragPos;
in float AO;

out vec4 fragColor;

uniform float uTime;
uniform sampler2D ourTexture;
uniform vec3 uLightPos;
uniform vec3 uLightColor;
uniform vec3 uAmbient;
uniform float uAOIntensity;

void main()
{
	vec3 norm = normalize(normal);
	//vec3 lightDir = normalize(uLightPos - fragPos);
	vec3 lightDir = normalize(uLightPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * uLightColor;
	vec3 result = texture(ourTexture, texCoord).rgb * uAmbient - diffuse - vec3(AO*uAOIntensity);
	fragColor = vec4(result,1.0);
}