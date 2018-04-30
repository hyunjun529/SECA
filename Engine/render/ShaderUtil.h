#ifndef KATA_RENDER_SHADERUTIL_H_
#define KATA_RENDER_SHADERUTIL_H_

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>


namespace kata
{
	namespace render
	{
		class ShaderUtil
		{
		public:
			const GLchar *vertUVonly =
			{
				"#version 400 core\n"\

				"layout(location = 0) in vec3 vertexPosition;\n"\
				"layout(location = 1) in vec2 vertexUV;\n"\

				"out vec2 UV;\n"\

				"uniform mat4 MVP;\n"\

				"void main(){\n"\

				"	gl_Position =  MVP * vec4(vertexPosition, 1.0f);\n"\

				"	UV = vertexUV;\n"\

				"}\n"
			};

			const GLchar *fragUVonly =
			{
				"#version 400 core\n"\

				"in vec2 UV;\n"\

				"out vec4 color;\n"\

				"uniform sampler2D myTextureSampler;\n"\

				"void main(){\n"\

				"	color = texture( myTextureSampler, UV ).rgba;\n"\

				"}\n"
			};
		};
	}
}


#endif // KATA_RENDER_SHADERUTIL_H_