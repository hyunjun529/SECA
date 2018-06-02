#include "Loader.h"


seca::render::Object seca::format::Loader::LoadFBXObject(const char *_file)
{
	format::FBXLoader fbxLoader;
	return fbxLoader.LoadFBX(_file);
}

seca::render::Object seca::format::Loader::LoadOBJObject(const char *_file, const char *_path)
{
	format::OBJLoader objLoader;
	render::Object object = objLoader.LoadOBJ(_file, _path);
	return object;
}

seca::render::Object seca::format::Loader::LoadPMXObject(const char *_file, const char *_path)
{
	format::PMXLoader pmx_loader;
	render::Object object = pmx_loader.LoadPMX(_file, _path);
	return object;
}