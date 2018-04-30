#version 400 core

in vec2 UV;

out vec4 color;

uniform sampler2D myTextureSampler;

void main()
{
	color = vec4(texture( myTextureSampler, UV ).rgb, 1.0f);
}