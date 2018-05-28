#include "PMXLoader.h"


#include "stb_image.h"


#include "util/Logger.h"


seca::render::Object seca::format::PMXLoader::loadPMX(const char *_file, const char *_path)
{
	seca::render::Object object;

	// if PMD then ?
	std::shared_ptr<saba::PMXModel> pmxModel = std::make_shared<saba::PMXModel>();

	std::string filename_with_path = std::string(_path) + std::string(_file);

	// Load PMX
	if (!pmxModel->Load(filename_with_path.c_str(), _path))
	{
		SECA_CONSOLE_ERROR("PMX load fail");
	}


	// convert PMXModel to GL Object
	// ref : https://github.com/benikabocha/saba/blob/master/viewer/Saba/GL/Model/MMD/GLMMDModel.h
	
	// Indicies
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

	// Vertex
	size_t num_vertex = pmxModel->GetVertexCount();
	auto positions = pmxModel->GetPositions();
	auto normals = pmxModel->GetNormals();
	auto uvs = pmxModel->GetUVs();
	std::vector<glm::vec3> tmp_vertex;
	std::vector<glm::vec3> tmp_normal;
	std::vector<glm::vec2> tmp_uv;
	tmp_vertex.resize(num_vertex);
	tmp_normal.resize(num_vertex);
	tmp_uv.resize(num_vertex);
	for (int i = 0; i < num_vertex; i++)
	{
		tmp_vertex[i].x = positions[i].x;
		tmp_vertex[i].y = positions[i].y;
		tmp_vertex[i].z = positions[i].z;
		tmp_normal[i].x = normals[i].x;
		tmp_normal[i].y = normals[i].y;
		tmp_normal[i].z = normals[i].z;
		tmp_uv[i].x = uvs[i].x;
		tmp_uv[i].y = abs(1.0f - uvs[i].y); // in PMX reversed
	}
	
	for (int i = 0; i < num_index; i++)
	{
		object.bufferPosition.push_back(tmp_vertex[indicies[i]]);
		object.bufferNormal.push_back(tmp_normal[indicies[i]]);
		object.bufferUV.push_back(tmp_uv[indicies[i]]);
	}
	
	// Material
	size_t matCount = pmxModel->GetMaterialCount();
	auto materials = pmxModel->GetMaterials();
	for (size_t matIdx = 0; matIdx < matCount; matIdx++)
	{
		render::Object::Texture texture;
		std::string texture_name = "";
		
		const auto& src = materials[matIdx];
		if (!src.m_texture.empty())
		{
			texture_name = src.m_texture;
		}
		else continue;

		if (!src.m_texture.empty())
		{
			texture.image = stbi_load(src.m_texture.c_str(), &texture.w, &texture.h, &texture.comp, STBI_default);
		}
		else continue;

		object.textures.insert(std::make_pair(texture_name, texture));
	}

	// SubMesh
	size_t subMeshCount = pmxModel->GetSubMeshCount();
	auto subMeshes = pmxModel->GetSubMeshes();
	object.subMeshes.resize(subMeshCount);
	for (size_t subMeshIdx = 0; subMeshIdx < subMeshCount; subMeshIdx++)
	{
		seca::render::Object::SubMesh sm;
		sm.idxBegin = 0;
		sm.cntVertex = 0;
		sm.textureId = 0;
		sm.texname = "";

		const auto& src = subMeshes[subMeshIdx];

		sm.idxBegin = src.m_beginIndex;
		sm.cntVertex = src.m_vertexCount;
		sm.texname = materials[src.m_materialID].m_texture.c_str();

		object.subMeshes.push_back(sm);
	}

	return object;
}