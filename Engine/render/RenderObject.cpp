#include "RenderObject.h"


seca::render::RenderObject::RenderObject() {}

seca::render::RenderObject::~RenderObject()
{
	DestroyShaders();
	DestroyVBO();
}

void seca::render::RenderObject::setup(GLFWwindow *_window)
{
	m_window = _window;

	if (!m_window)
	{
		assert("check init order");
	}

	glfwMakeContextCurrent(m_window);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CreateShaders();
}

void seca::render::RenderObject::clear()
{
	DestroyVBO();
	CreateVBO();
}

void seca::render::RenderObject::loadOBJObject(const char *_file, const char *_path)
{
	format::OBJLoader objLoader;
	m_objects.push_back(objLoader.loadOBJ(_file, _path));

	CreateVBO();
}

void seca::render::RenderObject::loadObject(render::Object object)
{
	m_objects.push_back(object);

	CreateVBO();
}

void seca::render::RenderObject::render()
{
	render(glm::mat4x4(1.0));
}

void seca::render::RenderObject::render(const glm::mat4 &MVP)
{
	glfwMakeContextCurrent(m_window);

	// MVP
	glUniformMatrix4fv(MvpId, 1, GL_FALSE, &MVP[0][0]);

	// draw
	for (int k = 0; k < m_objects.size(); k++)
	{
		render::Object *o = &m_objects[k];

		glBindVertexArray(o->vaoId);

		if (!o->subMeshes.empty())
		{
			for (int i = 0; i < o->subMeshes.size(); i++)
			{
				render::Object::SubMesh *sm = &o->subMeshes[i];


				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, sm->textureId);
				glUniform1i(TextureId, 0);

				glDrawArrays(o->drawHow, sm->idxBegin, sm->cntVertex);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}
		else
		{
			glDrawArrays(o->drawHow, 0, o->bufferPosition.size());
		}
	}
}

void seca::render::RenderObject::CreateVBO(void)
{
	glfwMakeContextCurrent(m_window);
	GLenum ErrorCheckValue = glGetError();

	for (int k = 0; k < m_objects.size(); k++)
	{
		render::Object *o = &m_objects[k];

		if (o->vaoId != 0) continue;

		// texture
		for (int i = 0; i < o->subMeshes.size(); i++)
		{
			render::Object::SubMesh *sm = &o->subMeshes[i];

			if (sm->textureId == 0 && sm->texname != "")
			{
				glActiveTexture(GL_TEXTURE0);
				glGenTextures(1, &sm->textureId);
				glBindTexture(GL_TEXTURE_2D, sm->textureId);
				if (o->textures[sm->texname].comp == 3) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, o->textures[sm->texname].w, o->textures[sm->texname].h, 0, GL_RGB,
						GL_UNSIGNED_BYTE, o->textures[sm->texname].image);
				}
				else if (o->textures[sm->texname].comp == 4) {
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, o->textures[sm->texname].w, o->textures[sm->texname].h, 0, GL_RGBA,
						GL_UNSIGNED_BYTE, o->textures[sm->texname].image);
				}
				else {
					assert(0);  // TODO
				}
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
		}

		// VAO
		glGenVertexArrays(1, &o->vaoId);
		glBindVertexArray(o->vaoId);

		// Position
		glGenBuffers(1, &o->vboVId);
		glBindBuffer(GL_ARRAY_BUFFER, o->vboVId);
		glBufferData(GL_ARRAY_BUFFER, o->bufferPosition.size() * sizeof(glm::vec3), &o->bufferPosition[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);

		// UV
		if(!o->bufferUV.empty())
		{ 
			glGenBuffers(1, &o->vboUvId);
			glBindBuffer(GL_ARRAY_BUFFER, o->vboUvId);
			glBufferData(GL_ARRAY_BUFFER, o->bufferUV.size() * sizeof(glm::vec2), &o->bufferUV[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(1);
		}

		// Normal

		// count Tirangle
		cntTirangle += o->numTriangles;
	}

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		assert("errer");
	}
}

void seca::render::RenderObject::DestroyVBO(void)
{
	GLenum ErrorCheckValue = glGetError();

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	for (int k = 0; k < m_objects.size(); k++)
	{
		render::Object *o = &m_objects[k];
		glDeleteBuffers(1, &o->vboVId);
		glDeleteBuffers(1, &o->vboUvId);
	}

	glBindVertexArray(0);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		assert("errer");
	}

	m_objects.clear();
}

void seca::render::RenderObject::CreateShaders(void)
{
	glfwMakeContextCurrent(m_window);

	GLenum ErrorCheckValue = glGetError();

	ShaderUtil shaderUtil;
	const GLchar *VertexShader = shaderUtil.vertUVonly;
	const GLchar *FragmentShader = shaderUtil.fragUVonly;

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	glCompileShader(VertexShaderId);

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);

	MvpId = glGetUniformLocation(ProgramId, "MVP");
	TextureId = glGetUniformLocation(ProgramId, "myTextureSampler");

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		assert("errer");
	}
}

void seca::render::RenderObject::DestroyShaders(void)
{
	GLenum ErrorCheckValue = glGetError();

	glUseProgram(0);

	glDetachShader(ProgramId, VertexShaderId);
	glDetachShader(ProgramId, FragmentShaderId);

	glDeleteShader(FragmentShaderId);
	glDeleteShader(VertexShaderId);

	glDeleteProgram(ProgramId);

	ErrorCheckValue = glGetError();
	if (ErrorCheckValue != GL_NO_ERROR)
	{
		assert("errer");
	}
}

