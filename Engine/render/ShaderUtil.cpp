#include "ShaderUtil.h"


#include <string>
#include <iostream>
#include <fstream>
#include <vector>


#include "util/Log.h"


GLuint seca::render::ShaderUtil::getShaderFromFile(const char * file_path, GLenum shader_type)
{
	// Read the Shader code from the file
	SECA_CONSOLE_INFO("read shader : {}", file_path);
	std::string shader_code;
	std::ifstream shader_stream(file_path, std::ios::in);
	if (shader_stream.is_open()) {
		std::string Line = "";
		while (getline(shader_stream, Line))
			shader_code += "\n" + Line;
		shader_stream.close();
	}
	else {
		SECA_CONSOLE_ERROR("Impossible to open {}. Are you in the right directory ?", file_path);
		return 0;
	}

	m_shader_code = new char[shader_code.length() + 1];
	memcpy_s(m_shader_code, shader_code.length(), shader_code.c_str(), shader_code.length());
	m_shader_code[shader_code.length()] = '\0';
	
	return compileShader(m_shader_code, shader_type);
}

GLuint seca::render::ShaderUtil::getShaderUVonlyVert()
{
	return compileShader(vertUVonly, GL_VERTEX_SHADER);
}

GLuint seca::render::ShaderUtil::getShaderUVonlyFrag()
{
	return compileShader(fragUVonly, GL_FRAGMENT_SHADER);
}

GLuint seca::render::ShaderUtil::compileShader(const char *shader_code, GLenum shader_type)
{
	// Create the shaders
	GLuint shaderID = glCreateShader(shader_type);

	// Compile Shader
	glShaderSource(shaderID, 1, &shader_code, NULL);
	glCompileShader(shaderID);

	// Check Shader
	GLint Result = GL_FALSE; // GL_COMPILE_STATUS
	int InfoLogLength;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (!Result) {
		std::vector<char> ShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
		SECA_CONSOLE_ERROR("Compile log : {}", ShaderErrorMessage[0]);
	}

	return shaderID;
}