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
	gl_FragColor = vec4(textureColor.xyz, 1.0);
}
