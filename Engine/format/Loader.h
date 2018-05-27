#ifndef SECA_FORMAT_LOADER_H_
#define SECA_FORMAT_LOADER_H_


#include "render/Object.h"

#include "FBX/FBXLoader.h"
#include "OBJ/OBJLoader.h"
#include "PMX/PMXLoader.h"


namespace seca
{
	namespace format
	{
		class Loader
		{
		private:
			

		public:
			Loader() {}
			~Loader() {}

			render::Object loadFBXObject(const char *_file);
			render::Object loadOBJObject(const char *_file, const char *_path);
			render::Object loadPMXObject(const char *_file, const char *_path);
		};
	}
}


#endif // SECA_FORMAT_LOADER_H_