#include "PMXLoader.h"


#include "util/Logger.h"


seca::render::Object seca::format::PMXLoader::loadPMX(const char *_file, const char *_path)
{
	seca::render::Object object;

	// if PMD then ?
	std::shared_ptr<saba::PMXModel> pmxModel = std::make_shared<saba::PMXModel>();

	std::string filename_with_path = std::string(_path) + std::string(_file);

	// load PMX
	if (!pmxModel->Load(filename_with_path.c_str(), _path))
	{
		SECA_CONSOLE_ERROR("PMX load fail");
	}

	// convert PMXModel to GL Object
	// ref : https://github.com/benikabocha/saba/blob/master/viewer/Saba/GL/Model/MMD/GLMMDModel.h
	size_t num_vertex = pmxModel->GetVertexCount();
	auto positions = pmxModel->GetPositions();
	auto normals = pmxModel->GetNormals();
	auto uvs = pmxModel->GetUVs();

	const void* iboBuf = pmxModel->GetIndices();
	size_t num_index = pmxModel->GetIndexCount();
	size_t indexElemSize = pmxModel->GetIndexElementSize();
	std::vector<int> indicies;
	indicies.resize(num_index);
	switch (indexElemSize)
	{
	case 1:
		for (int i = 0; i < num_index; i++) indicies[i] = ((uint8_t*)iboBuf)[i];
		break;
	case 2:
		for (int i = 0; i < num_index; i++) indicies[i] = ((uint16_t*)iboBuf)[i];
		break;
	case 4:
		for (int i = 0; i < num_index; i++) indicies[i] = ((uint32_t*)iboBuf)[i];
		break;
	}
	
	size_t subMeshCount = pmxModel->GetSubMeshCount();
	auto subMeshes = pmxModel->GetSubMeshes();

	std::vector<glm::vec3> tmp_vertex;
	tmp_vertex.resize(num_vertex);
	for (int i = 0; i < num_vertex; i++)
	{
		tmp_vertex[i].x = positions[i].x;
		tmp_vertex[i].y = positions[i].y;
		tmp_vertex[i].z = positions[i].z;
	}
	
	for (int i = 0; i < num_index; i++)
	{
		object.bufferPosition.push_back(tmp_vertex[indicies[i]]);
	}


	return object;
}