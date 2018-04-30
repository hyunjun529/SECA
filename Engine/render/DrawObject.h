#ifndef KATA_RENDER_DRAWOBJECT_H_
#define KATA_RENDER_DRAWOBJECT_H_

#include <map>
#include <vector>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


namespace kata
{
	namespace render
	{
		class DrawObject
		{
		private:
			int m_idx = -1;

		public:
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
			std::vector<SubMesh> subMeshs;
		};
	}
}


#endif // KATA_RENDER_DRAWOBJECT_H_