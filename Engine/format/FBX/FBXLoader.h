#ifndef SECA_FORMAT_FBX_FBXLOADER_H_
#define SECA_FORMAT_FBX_FBXLOADER_H_


#include <memory>
#include <vector>
#include <fbxsdk.h>


#include "render/Object.h"


namespace seca
{
	namespace format
	{
		// ref http://help.autodesk.com/view/FBX/2018/ENU/?guid=FBX_Developer_Help_getting_started_your_first_fbx_sdk_program_html
		class FBXLoader
		{
		private:
			seca::render::Object object;

			FbxManager *lSdkManager;

		public:
			FBXLoader() {}
			~FBXLoader() {}

			render::Object LoadFBX(const char *_file);
			void TraverseFBXNodes(FbxNode* node);
		};
	}
}


#endif // SECA_FORMAT_FBX_FBXLOADER_H_