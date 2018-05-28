#ifndef SECA_FORMAT_PMX_PMXLOADER_H_
#define SECA_FORMAT_PMX_PMXLOADER_H_


#include <string>


#include "render/Object.h"

#include "Saba/PMXFile.h"
#include "Saba/PMXModel.h"


namespace seca
{
	namespace format
	{
		class PMXLoader
		{
		private:
			seca::render::Object object;


		public:
			PMXLoader() {}
			~PMXLoader() {}

			render::Object loadPMX(const char *_file, const char *_path);
		};
	}
}


#endif // SECA_FORMAT_PMX_PMXLOADER_H_