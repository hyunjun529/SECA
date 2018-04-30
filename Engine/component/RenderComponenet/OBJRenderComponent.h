#ifndef KATA_COMPONENT_SCENEOBJCOMPONENT_H_
#define KATA_COMPONENT_SCENEOBJCOMPONENT_H_

#include "../../util/Log.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>

#include "RenderComponent.h"

#include "../InputComponent/ImguiViewerInputComponent.h"

#include "../../render/World.h"
#include "../../render/DrawObject.h"
#include "../../render/OBJLoader.h"

namespace kata
{
	namespace component
	{
		class OBJRenderComponent : public RenderComponent
		{
		private:
			// http://www.opengl-tutorial.org/kr/beginners-tutorials/tutorial-7-model-loading/

			component::ImguiInputComponent *m_inputImgui = nullptr;

			GLuint
				TextureId,
				MvpId;

			GLuint
				VertexShaderId,
				FragmentShaderId,
				ProgramId;

			GLuint
				cntTirangle = 0;

			std::vector<render::DrawObject> m_drawObjects;


			void CreateVBO(void)
			{
				glfwMakeContextCurrent(m_world->m_window);
				GLenum ErrorCheckValue = glGetError();

				for (int k = 0; k < m_drawObjects.size(); k++)
				{
					render::DrawObject *o = &m_drawObjects[k];

					if (o->vaoId != 0) continue;

					// texture
					for (int i = 0; i < o->subMeshs.size(); i++)
					{
						render::DrawObject::SubMesh *sm = &o->subMeshs[i];

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
					glGenBuffers(1, &o->vboUvId);
					glBindBuffer(GL_ARRAY_BUFFER, o->vboUvId);
					glBufferData(GL_ARRAY_BUFFER, o->bufferUV.size() * sizeof(glm::vec2), &o->bufferUV[0], GL_STATIC_DRAW);
					glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
					glEnableVertexAttribArray(1);

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

			void DestroyVBO(void)
			{
				GLenum ErrorCheckValue = glGetError();

				glDisableVertexAttribArray(1);
				glDisableVertexAttribArray(0);

				glBindBuffer(GL_ARRAY_BUFFER, 0);
				for (int k = 0; k < m_drawObjects.size(); k++)
				{
					render::DrawObject *o = &m_drawObjects[k];
					glDeleteBuffers(1, &o->vboVId);
					glDeleteBuffers(1, &o->vboUvId);
				}

				glBindVertexArray(0);

				ErrorCheckValue = glGetError();
				if (ErrorCheckValue != GL_NO_ERROR)
				{
					assert("errer");
				}

				m_drawObjects.clear();
			}

			void CreateShaders(void)
			{
				glfwMakeContextCurrent(m_world->m_window);

				GLenum ErrorCheckValue = glGetError();

				const GLchar *VertexShader = m_world->loadShader("UVonly.vert");
				const GLchar *FragmentShader = m_world->loadShader("UVonly.frag");

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

			void DestroyShaders(void)
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


		public:
			OBJRenderComponent() {}

			~OBJRenderComponent()
			{
				DestroyShaders();
				DestroyVBO();
			}

			void setImguiInputComponent(component::ImguiInputComponent *_inputImgui)
			{
				m_inputImgui = _inputImgui;
			}

			void setup()
			{
				if (!m_world)
				{
					assert("check init order");
				}

				if (!isSingleWindow) glfwHideWindow(m_world->m_window);

				glfwMakeContextCurrent(m_world->m_window);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
				
				glEnable(GL_DEPTH_TEST);

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				CreateShaders();
			}

			void clear()
			{
				DestroyVBO();

				// load Grid

				CreateVBO();
			}

			void load(const char *_file, const char *_path)
			{
				render::OBJLoader objLoader;
				m_drawObjects.push_back(objLoader.loadOBJ(_file, _path));

				CreateVBO();
			}

			void render()
			{
				render(glm::mat4x4(1.0));
			}

			void render(const glm::mat4 &MVP)
			{
				if (!m_inputImgui)
				{
					assert("check init order");
				}
				glfwMakeContextCurrent(m_world->m_window);

				glClearColor(
					m_inputImgui->getVec4Clear()->x,
					m_inputImgui->getVec4Clear()->y,
					m_inputImgui->getVec4Clear()->z,
					m_inputImgui->getVec4Clear()->w
				);

				if (!isSingleWindow) glClear(GL_COLOR_BUFFER_BIT);

				// MVP
				glUniformMatrix4fv(MvpId, 1, GL_FALSE, &MVP[0][0]);

				// draw
				for (int k = 0; k < m_drawObjects.size(); k++)
				{
					render::DrawObject *o = &m_drawObjects[k];

					glBindVertexArray(o->vaoId);

					for (int i = 0; i < o->subMeshs.size(); i++)
					{
						render::DrawObject::SubMesh *sm = &o->subMeshs[i];

						glActiveTexture(GL_TEXTURE0);
						glBindTexture(GL_TEXTURE_2D, sm->textureId);
						glUniform1i(TextureId, 0);

						glDrawArrays(GL_TRIANGLES, sm->idxBegin, sm->cntVertex);

						glBindTexture(GL_TEXTURE_2D, 0);
					}
				}

				if (m_inputImgui->funcReset)
				{
					clear();
					m_inputImgui->funcReset = false;
				}
				if (m_inputImgui->funcLoad)
				{
					switch (m_inputImgui->param_load_obj)
					{
					// 빌드 설정에 따라서 상대 경로가 달라질 수 있습니다.
					case 1:
						load("cube.obj", "..//resource//cube//");
						break;
					case 2:
						load("bunny.obj", "..//resource//bunny//");
						break;
					case 3:
						load("teapot.obj", "..//resource//teapot//");
						break;
					case 4:
						load("capsule.obj", "..//resource//capsule//");
						break;
					case 5:
						load("white_oak.obj", "..//resource//white_oak//");
						break;
					case 6:
						load("kizunaai.obj", "..//resource//kizunaai//");
						break;
					}
					m_inputImgui->funcLoad = false;
				}

				if (!isSingleWindow) glfwSwapBuffers(m_world->m_window);
			}
		};
	}
}

#endif // KATA_COMPONENT_SCENEOBJCOMPONENT_H_
