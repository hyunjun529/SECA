#include "FBXLoader.h"


#include "stb_image.h"


/*
conflict in (format.h line 3887, mutable char buffer_[BUFFER_SIZE];), mutable char + was already overload in fbxsdk.
so i can't use Log.
*/
//#include "util/Logger.h"


seca::render::Object seca::format::FBXLoader::loadFBX(const char *_file)
{
	lSdkManager = FbxManager::Create();

	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT);
	lSdkManager->SetIOSettings(ios);
	
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager, "");
	if (!lImporter->Initialize(_file, -1, lSdkManager->GetIOSettings())) {
		printf("Call to FbxImporter::Initialize() failed.\n");
		printf("Error returned: %s\n\n", lImporter->GetStatus().GetErrorString());
		exit(-1);
	}

	FbxScene* lScene = FbxScene::Create(lSdkManager, "myScene");

	lImporter->Import(lScene);

	// https://help.autodesk.com/cloudhelp/2018/ENU/FBX-Developer-Help/cpp_ref/class_fbx_axis_system.html
	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eZAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
	OurAxisSystem.ConvertScene(lScene);

	// split mesh per material for submesh
	// https://forums.autodesk.com/t5/fbx-forum/how-can-i-determine-mesh-and-sub-mesh/td-p/4227676
	FbxGeometryConverter converter(lSdkManager);
	converter.Triangulate(lScene, true);
	converter.SplitMeshesPerMaterial(lScene, true);


	lImporter->Destroy();


	// Materials
	int num_material = lScene->GetMaterialCount();
	int lTextureCount = lScene->GetTextureCount();
	FbxProperty prop;
	object.subMeshes.resize(num_material);
	for (int i = 0; i < num_material; i++)
	{
		render::Object::Texture texture;
		std::string texture_name = "";

		FbxTexture *lTexture = lScene->GetTexture(i);
		FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(lTexture);

		const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(_file);
		const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

		// find relative texture path
		const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
		texture.image = stbi_load(lResolvedFileName, &texture.w, &texture.h, &texture.comp, STBI_default);

		// find absolute texture path

		// not found

		texture_name = lFileTexture->GetRelativeFileName();

		object.textures.insert(std::make_pair(texture_name, texture));
		object.subMeshes[i].texname = texture_name;
	}
	

	//// Texture
	//for (int lTextureIndex = 0; lTextureIndex < lTextureCount; ++lTextureIndex)
	//{
	//	render::Object::Texture texture;
	//	std::string texture_name = "";

	//	FbxTexture *lTexture = lScene->GetTexture(lTextureIndex);
	//	FbxFileTexture *lFileTexture = FbxCast<FbxFileTexture>(lTexture);

	//	const FbxString lAbsFbxFileName = FbxPathUtils::Resolve(_file);
	//	const FbxString lAbsFolderName = FbxPathUtils::GetFolderName(lAbsFbxFileName);

	//	// find relative texture path
	//	const FbxString lResolvedFileName = FbxPathUtils::Bind(lAbsFolderName, lFileTexture->GetRelativeFileName());
	//	texture.image = stbi_load(lResolvedFileName, &texture.w, &texture.h, &texture.comp, STBI_default);

	//	// find absolute texture path

	//	// not found

	//	texture_name = lFileTexture->GetRelativeFileName();

	//	object.textures.insert(std::make_pair(texture_name, texture));
	//}


	// make object great again
	FbxNode* lRootNode = lScene->GetRootNode();
	if (lRootNode)
	{
		traverseFBXNodes(lRootNode);
	}
	lSdkManager->Destroy();


	return object;
}


// ref : https://github.com/suitengu/fbxDemo/blob/master/fbxDemo/fbxDemoApp.cpp
void seca::format::FBXLoader::traverseFBXNodes(FbxNode* node)
{
	const char* nodeName = node->GetName();
	printf("node name: %s\n", nodeName);

	// Get da transforms
	FbxDouble3 translation = node->LclTranslation.Get();
	FbxDouble3 rotation = node->LclRotation.Get();
	FbxDouble3 scaling = node->LclScaling.Get();

	FbxLayerElementArrayTemplate<int>* lMaterialIndice = NULL;
	FbxGeometryElement::EMappingMode lMaterialMappingMode = FbxGeometryElement::eNone;

	// Determine # of children the node has
	int numChildren = node->GetChildCount();
	FbxNode* childNode = 0;
	for (int child = 0; child < numChildren; child++)
	{
		childNode = node->GetChild(child);
		FbxMesh *mesh = childNode->GetMesh();

		if (mesh != NULL)
		{
			int num_polygon = mesh->GetPolygonCount();
			int num_vertices = mesh->GetControlPointsCount();
			bool has_normal = mesh->GetElementNormalCount() > 0;
			bool has_uv = mesh->GetElementUVCount() > 0;


			// Vertex position
			std::vector<glm::vec3> tmp_vertex;
			tmp_vertex.resize(num_vertices);
			for (int j = 0; j < num_vertices; j++)
			{
				FbxVector4 coord = mesh->GetControlPointAt(j); // 4,w = 1.0f
				tmp_vertex[j].x = (GLfloat)coord.mData[0];
				tmp_vertex[j].y = (GLfloat)coord.mData[1];
				tmp_vertex[j].z = (GLfloat)coord.mData[2];
			}

			// Normal
			const FbxGeometryElementNormal * normals = NULL;
			std::vector<glm::vec3> tmp_normal;
			tmp_normal.resize(num_vertices);
			if (has_normal)
			{
				normals = mesh->GetElementNormal(0);
				for (int j = 0; j < num_vertices; j++)
				{
					FbxVector4 coord = normals->GetDirectArray().GetAt(j); // 4,w = 1.0f
					tmp_normal[j].x = (GLfloat)coord.mData[0];
					tmp_normal[j].y = (GLfloat)coord.mData[1];
					tmp_normal[j].z = (GLfloat)coord.mData[2];
				}
			}

			// UV
			const FbxGeometryElementUV * uvs = NULL;
			std::vector<glm::vec2> tmp_uv;
			tmp_uv.resize(num_vertices);
			if (has_uv)
			{
				uvs = mesh->GetElementUV(0);
				for (int j = 0; j < num_vertices; j++)
				{
					FbxVector4 coord = uvs->GetDirectArray().GetAt(j); // 4,w = 1.0f
					tmp_uv[j].x = (GLfloat)coord.mData[0];
					tmp_uv[j].y = (GLfloat)coord.mData[1];
				}
			}


			// Get indices from the mesh
			int num_indices = mesh->GetPolygonVertexCount();
			int *indices = mesh->GetPolygonVertices();
			for (int j = 0; j < num_indices; j++)
			{
				object.bufferPosition.push_back(tmp_vertex[indices[j]]);
				if (has_normal) object.bufferNormal.push_back(tmp_normal[indices[j]]);
				if (has_uv) object.bufferUV.push_back(tmp_uv[indices[j]]);
			}


			// Create SubMesh
			// We have Texture, but not each material
			// we need grouped vertex each materials
			FbxLayerElementArrayTemplate<int>* materialIndices;
			FbxGeometryElement::EMappingMode materialMappingMode = FbxGeometryElement::eNone;
			if (mesh->GetElementMaterial())
			{
				materialIndices = &(mesh->GetElementMaterial()->GetIndexArray());
				materialMappingMode = mesh->GetElementMaterial()->GetMappingMode();
				
				if (materialIndices)
				{
					switch (materialMappingMode)
					{
					case FbxGeometryElement::eByPolygon:
					{
						if (materialIndices->GetCount() == num_polygon)
						{
							int current_material_index = 0;
							render::Object::SubMesh sm;
							sm.idxBegin = 0;
							sm.cntVertex = 0;
							sm.textureId = 0;
							sm.texname = "";

							for (unsigned int i = 0; i < num_polygon; ++i)
							{
								unsigned int materialIndex = materialIndices->GetAt(i);
								if (current_material_index == materialIndex)
								{
									sm.cntVertex += 3;
								}
								else
								{
									object.subMeshes.push_back(sm);

									//if (materials.size() > 0)
									//{
									//	sm.texname = materials[material_id].diffuse_texname;
									//}
									sm.idxBegin += sm.cntVertex;
									sm.cntVertex = 3;

									current_material_index = materialIndex;
								}
							}
							object.subMeshes.push_back(sm);
						}
					}
					break;

					case FbxGeometryElement::eAllSame:
					{
						unsigned int materialIndex = materialIndices->GetAt(0);

						render::Object::SubMesh sm;
						sm.idxBegin = 0;
						sm.cntVertex = 0;
						sm.textureId = 0;
						sm.texname = "";

						object.subMeshes[0].cntVertex = num_polygon * 3;

						//sm.cntVertex = num_polygon * 3;

						//object.subMeshes.push_back(sm);
					}
					break;

					default:
						throw std::exception("Invalid mapping mode for material\n");
					}
				}
			}
		}
		this->traverseFBXNodes(childNode);
	}
}
