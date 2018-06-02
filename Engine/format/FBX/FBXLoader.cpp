#include "FBXLoader.h"


#include "stb_image.h"


/*
conflict in (format.h line 3887, mutable char buffer_[BUFFER_SIZE];), mutable char + was already overload in fbxsdk.
so i can't use Log.
*/
//#include "util/Logger.h"


seca::render::Object seca::format::FBXLoader::LoadFBX(const char *_file)
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
		TraverseFBXNodes(lRootNode);
	}
	lSdkManager->Destroy();


	return object;
}


// ref : https://github.com/suitengu/fbxDemo/blob/master/fbxDemo/fbxDemoApp.cpp
void seca::format::FBXLoader::TraverseFBXNodes(FbxNode* node)
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
			tmp_vertex.resize(num_polygon);

			// Vertex Normal
			std::vector<glm::vec3> tmp_normal;
			tmp_normal.resize(num_polygon);

			// Vertex UV
			const FbxGeometryElementUV * uvs = NULL;
			std::vector<glm::vec2> tmp_uv;
			tmp_uv.resize(num_polygon);

			for(int i = 0; i < num_polygon; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					int idx = mesh->GetPolygonVertex(i, j);
					FbxVector4 vertex_position = mesh->GetControlPointAt(idx);
					tmp_vertex[i].x = (GLfloat)vertex_position.mData[0];
					tmp_vertex[i].y = (GLfloat)vertex_position.mData[1];
					tmp_vertex[i].z = (GLfloat)vertex_position.mData[2];

					if (has_normal)
					{
						FbxVector4 vertex_normal;
						mesh->GetPolygonVertexNormal(i, j, vertex_normal);
						tmp_normal[i].x = (GLfloat)vertex_normal.mData[0];
						tmp_normal[i].y = (GLfloat)vertex_normal.mData[1];
						tmp_normal[i].z = (GLfloat)vertex_normal.mData[2];
					}

					if (has_uv)
					{
						uvs = mesh->GetElementUV(0);

						int inCtrlPointIndex = mesh->GetPolygonVertex(i, j);
						int inTextureUVIndex = mesh->GetTextureUVIndex(i, j);

						switch (uvs->GetMappingMode())
						{
						case FbxGeometryElement::eByControlPoint:
							switch (uvs->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							{
								tmp_uv[i].x = static_cast<float>(uvs->GetDirectArray().GetAt(inCtrlPointIndex).mData[0]);
								tmp_uv[i].y = static_cast<float>(uvs->GetDirectArray().GetAt(inCtrlPointIndex).mData[1]);
							}
							break;

							case FbxGeometryElement::eIndexToDirect:
							{
								int index = uvs->GetIndexArray().GetAt(inCtrlPointIndex);
								tmp_uv[i].x = static_cast<float>(uvs->GetDirectArray().GetAt(index).mData[0]);
								tmp_uv[i].y = static_cast<float>(uvs->GetDirectArray().GetAt(index).mData[1]);
							}
							break;

							default:
								throw std::exception("Invalid Reference");
							}
							break;

						case FbxGeometryElement::eByPolygonVertex:
							switch (uvs->GetReferenceMode())
							{
							case FbxGeometryElement::eDirect:
							case FbxGeometryElement::eIndexToDirect:
							{
								tmp_uv[i].x = static_cast<float>(uvs->GetDirectArray().GetAt(inTextureUVIndex).mData[0]);
								tmp_uv[i].y = static_cast<float>(uvs->GetDirectArray().GetAt(inTextureUVIndex).mData[1]);
							}
							break;

							default:
								throw std::exception("Invalid Reference");
							}
							break;
						}
					}

					object.bufferPosition.push_back(tmp_vertex[i]);
					if (has_normal) object.bufferNormal.push_back(tmp_normal[i]);
					if (has_uv)
					{
						tmp_uv[i].x = tmp_uv[i].x;
						tmp_uv[i].y = abs(1.0f - tmp_uv[i].y);

						//float for_swap = tmp_uv[i].x;
						//tmp_uv[i].x = tmp_uv[i].y;
						//tmp_uv[i].y = for_swap;

						object.bufferUV.push_back(tmp_uv[i]);
					}
				}
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
		this->TraverseFBXNodes(childNode);
	}
}
