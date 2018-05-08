#ifndef SECA_FORMAT_OBJ_OBJLOADER_H_
#define SECA_FORMAT_OBJ_OBJLOADER_H_


#include "stb_image.h"
#include "tiny_obj_loader.h"


#include "render/Object.h"

#include "util/Log.h"


namespace seca
{
	namespace format
	{
		// ref https://github.com/syoyo/tinyobjloader/blob/master/examples/viewer/viewer.cc
		class OBJLoader
		{
		private:
			std::string err;

			tinyobj::attrib_t attrib;
			std::vector<tinyobj::shape_t> shapes;
			std::vector<tinyobj::material_t> materials;
			

		public:
			OBJLoader() {}
			~OBJLoader() {}
			render::Object loadOBJ(const char *_file, const char *_path);
			void CalcNormal(glm::vec3 N, glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
		};
	}
}


#endif // SECA_FORMAT_OBJ_OBJLOADER_H_