#version 400 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 WVP;

void main()
{
	gl_Position =  WVP * vertexPosition;
	UV = vertexUV;
}