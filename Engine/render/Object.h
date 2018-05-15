#ifndef SECA_RENDER_OBJECT_H_
#define SECA_RENDER_OBJECT_H_


#include <map>
#include <vector>


#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


namespace seca
{
	namespace render
	{
		class Object
		{
		private:
			int m_idx = -1;

		public:
			Object() {}
			~Object() {}

			GLuint vaoId;

			GLuint vboVId;
			GLuint vboUvId;
			GLuint vboNorId;

			int numTriangles = 0;
			std::vector<glm::vec3> bufferPosition;
			std::vector<glm::vec2> bufferUV;
			std::vector<glm::vec3> bufferNormal;

			typedef struct {
				int w;
				int h;
				int comp;
				unsigned char* image = nullptr;
			} Texture;
			std::map<std::string, Texture> textures;

			typedef struct {
				std::string texname;
				GLuint textureId;
				GLuint idxBegin;
				GLuint cntVertex;
			} SubMesh;
			std::vector<SubMesh> subMeshes;

			GLenum drawHow = GL_TRIANGLES;
		};
	}
}


#endif // SECA_RENDER_DRAWOBJECT_H_