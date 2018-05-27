#include "FBXLoader.h"


/*
conflict in (format.h line 3887, mutable char buffer_[BUFFER_SIZE];), mutable char + was already overload in fbxsdk.
so i can't use Log.
*/
//#include "util/Log.h"


seca::render::Object seca::format::FBXLoader::loadFBX(const char *_file)
{
	FbxManager* lSdkManager = FbxManager::Create();

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

	lImporter->Destroy();


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

	// Determine # of children the node has
	int numChildren = node->GetChildCount();
	FbxNode* childNode = 0;
	for (int i = 0; i < numChildren; i++)
	{
		childNode = node->GetChild(i);
		FbxMesh* mesh = childNode->GetMesh();
		if (mesh != NULL)
		{
			// Get vertices from the mesh
			// https://help.autodesk.com/view/FBX/2018/ENU/?guid=FBX_Developer_Help_meshes_materials_and_textures_meshes_html
			int num_vertices = mesh->GetControlPointsCount();
			std::vector<glm::vec3> tmp_vertex;
			tmp_vertex.resize(num_vertices);
			for (int j = 0; j < num_vertices; j++)
			{
				FbxVector4 coord = mesh->GetControlPointAt(j); // 4,w = 1.0f
				tmp_vertex[j].x = (GLfloat)coord.mData[0] * 0.01f;
				tmp_vertex[j].y = (GLfloat)coord.mData[1] * 0.01f;
				tmp_vertex[j].z = (GLfloat)coord.mData[2] * 0.01f;
			}

			// Get indices from the mesh
			int num_indices = mesh->GetPolygonVertexCount();
			int *indices = mesh->GetPolygonVertices();
			for (int j = 0; j < num_indices; j++)
			{
				object.bufferPosition.push_back(tmp_vertex[indices[j]]);
			}

		}
		this->traverseFBXNodes(childNode);
	}
}
