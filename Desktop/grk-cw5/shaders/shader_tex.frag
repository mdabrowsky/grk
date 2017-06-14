#version 430 core

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D sampler;

in vec3 interpNormal;
in vec2 texCoord;

void main()
{
	vec4 textureColor = texture2D(sampler, texCoord);
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	gl_FragColor = vec4(textureColor.xyz * diffuse, 1.0);
}
