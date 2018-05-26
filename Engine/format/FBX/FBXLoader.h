#ifndef SECA_FORMAT_FBX_FBXLOADER_H_
#define SECA_FORMAT_FBX_FBXLOADER_H_


//#include <fbxsdk.h>


#include "render/Object.h"

#include "util/Log.h"


namespace seca
{
	namespace format
	{
		// ref http://help.autodesk.com/view/FBX/2018/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html
		class FBXLoader
		{
		private:
			

		public:
			FBXLoader() {}
			~FBXLoader() {}
			render::Object loadFBX(const char *_file, const char *_path);
		};
	}
}


#endif // SECA_FORMAT_FBX_FBXLOADER_H_