#version 330 core
in vec3 aPosition;
in vec2 aTexCoord;
in vec3 aNormal;

out vec2 texCoord;
out vec3 normal;
out vec3 fragPos;

uniform mat4 uProjection;
uniform mat4 uView;
uniform mat4 uTransform;

void main()
{
	gl_Position = uProjection * uView * uTransform * vec4(aPosition,1.0);
	texCoord = aTexCoord;
	fragPos = vec3(uTransform * vec4(aPosition, 1.0));
	normal = aNormal;
}