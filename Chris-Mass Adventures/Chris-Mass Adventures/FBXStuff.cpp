#include "stdafx.h"
//#include "fbxsdk.h"
#include <vector>

#include "FBXStuff.h"

HRESULT FBXStuff::NormalsAndUVsToo(std::vector<VERTEX>* outVertexVector, const char * _Filename)
{
	if (fbxManager == nullptr)
	{
		fbxManager = FbxManager::Create(); // creates the fbx manager

		fbxIOSettings = FbxIOSettings::Create(fbxManager, "IOSROOT"); // creates the IOSettings
		fbxManager->SetIOSettings(fbxIOSettings); // sets the manager's IOSettings to be the new settins we just created
	}
	fbxImporter = FbxImporter::Create(fbxManager, ""); // creates the importer
	fbxScene = FbxScene::Create(fbxManager, ""); // create the scene

	bool OKImport = fbxImporter->Initialize(_Filename, -1, fbxManager->GetIOSettings());
	if (!OKImport)
		return E_FAIL;

	OKImport = fbxImporter->Import(fbxScene);
	if (!OKImport)
		return E_FAIL;

	fbxImporter->Destroy();

	FbxNode* fbxRootNode = fbxScene->GetRootNode();


	if (fbxRootNode)
	{
		for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
		{
			FbxNode* fbxChildNode = fbxRootNode->GetChild(i);


			if (fbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType fbxNodeAttributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
			if (fbxNodeAttributeType != FbxNodeAttribute::eMesh)
				continue;

			fbxMesh = (FbxMesh*)fbxChildNode->GetNodeAttribute();
			FbxLayerElementArrayTemplate<FbxVector2>* UVs;
			fbxMesh->GetTextureUV(&UVs);


			FbxVector4* vertices = fbxMesh->GetControlPoints();

			VERTEX vertex;

			// get the vertices
			int vertexindex;
			int polycount = fbxMesh->GetPolygonCount();

			int polygoncounter = 0;

			for (int j = 0; j < polycount; j++)
			{
				int numverts = fbxMesh->GetPolygonSize(j);

				for (int k = 0; k < numverts; k++)
				{
					vertexindex = fbxMesh->GetPolygonVertex(j, k);

					vertex.pos[0] = (float)vertices[vertexindex].mData[0];
					//vertex.pos[0] = -vertex.pos[0];
					vertex.pos[1] = (float)vertices[vertexindex].mData[1];
					//vertex.pos[1] = -vertex.pos[1];
					vertex.pos[2] = (float)vertices[vertexindex].mData[2];

					// getting the UV's from the mesh!
					int uvindex = fbxMesh->GetTextureUVIndex(j, k);
					FbxVector2 vertexuvs = UVs->GetAt(uvindex);

					vertex.uvw[0] = vertexuvs.mData[0];
					vertex.uvw[1] = 1-vertexuvs.mData[1];
					vertex.uvw[2] = 0;

					FbxGeometryElementNormal* normalelement = fbxMesh->GetElementNormal();

					if (!normalelement)
						continue;

					//if (normalelement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
					//	return E_FAIL;

					int normalsindex = 0;

					if (normalelement->GetReferenceMode() == FbxGeometryElement::eDirect)
						normalsindex = vertexindex;
					if (normalelement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						normalsindex = normalelement->GetIndexArray().GetAt(vertexindex);
					FbxVector4 normals = normalelement->GetDirectArray().GetAt(normalsindex);

					/////
					vertex.nrm[0] = (float)normals.mData[0];
					vertex.nrm[1] = (float)normals.mData[1];
					vertex.nrm[2] = (float)normals.mData[2];

					outVertexVector->push_back(vertex);

				}
			}

		}
	}
	return S_OK;
}

HRESULT FBXStuff::BinaryOut(std::vector<VERTEX>inVertexVector, const char* _Filename)
{
	std::string filename = std::string(_Filename);
	

	return S_OK
}
