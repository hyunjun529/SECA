#include "Loader.h"


seca::render::Object seca::format::Loader::loadFBXObject(const char *_file)
{
	format::FBXLoader fbxLoader;
	render::Object object = fbxLoader.loadFBX(_file);
	return object;
}

seca::render::Object seca::format::Loader::loadOBJObject(const char *_file, const char *_path)
{
	format::OBJLoader objLoader;
	render::Object object = objLoader.loadOBJ(_file, _path);
	return object;
}