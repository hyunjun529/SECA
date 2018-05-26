#ifndef SECA_RENDER_RENDEROBJECT_H_
#define SECA_RENDER_RENDEROBJECT_H_


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


#include "util/Log.h"

#include "render/Object.h"
#include "render/ShaderUtil.h"

#include "format/FBX/FBXLoader.h"
#include "format/OBJ/OBJLoader.h"


namespace seca
{
	namespace render
	{
		class RenderObject
		{
		private:
			GLFWwindow *m_window; // if multi window then need check each buffer.

			std::vector<render::Object> m_objects; // hmm...

			GLuint
				TextureId,
				MvpId;

			GLuint
				VertexShaderId,
				FragmentShaderId,
				ProgramId;

			GLuint
				cntTirangle = 0;

			void CreateVBO(void);
			void DestroyVBO(void);
			void CreateShaders(void);
			void DestroyShaders(void);


		public:
			RenderObject();
			~RenderObject();

			void setup(GLFWwindow *_window);
			void clear();
			void render();
			void render(const glm::mat4 &MVP);

			void loadFBXObject(const char *_file);
			void loadOBJObject(const char *_file, const char *_path);
			void loadObject(render::Object object);
		};
	}
}


#endif // SECA_RENDER_RENDEROBJECT_H_