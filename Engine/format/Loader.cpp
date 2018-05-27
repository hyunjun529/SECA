#include "Loader.h"


seca::render::Object seca::format::Loader::loadFBXObject(const char *_file)
{
	format::FBXLoader fbxLoader;
	return fbxLoader.loadFBX(_file);
}

seca::render::Object seca::format::Loader::loadOBJObject(const char *_file, const char *_path)
{
	format::OBJLoader objLoader;
	render::Object object = objLoader.loadOBJ(_file, _path);
	return object;
}

seca::render::Object seca::format::Loader::loadPMXObject(const char *_file, const char *_path)
{
	format::PMXLoader pmx_loader;
	render::Object object = pmx_loader.loadPMX(_file, _path);
	return object;
}