#include "stdafx.h"
//#include "fbxsdk.h"
#include <vector>

#include "FBXStuff.h"


HRESULT FBXStuff::LoadFBX(std::vector<MyVertex>* outVertexVector)
{
	//if (fbxManager == nullptr)
	//{
	//	fbxManager = FbxManager::Create(); // creates the fbx manager
	//
	//	fbxIOSettings = FbxIOSettings::Create(fbxManager, "IOSROOT"); // creates the IOSettings
	//	fbxManager->SetIOSettings(fbxIOSettings); // sets the manager's IOSettings to be the new settins we just created
	//}
	//fbxImporter = FbxImporter::Create(fbxManager, ""); // creates the importer
	//fbxScene = FbxScene::Create(fbxManager, ""); // create the scene
	//
	//bool OKImport = fbxImporter->Initialize("F:\Program Files (x86)\RTA\FBX\Box_Jump.fbx", -1, fbxManager->GetIOSettings());
	//if (!OKImport)
	//	return E_FAIL;
	//
	//OKImport = fbxImporter->Import(fbxScene);
	//if (!OKImport)
	//	return E_FAIL;
	//
	//fbxImporter->Destroy();
	//
	//FbxNode* fbxRootNode = fbxScene->GetRootNode();
	//
	//
	//if (fbxRootNode)
	//{
	//	for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
	//	{
	//		FbxNode* fbxChildNode = fbxRootNode->GetChild(i);
	//
	//
	//		if (fbxChildNode->GetNodeAttribute() == NULL)
	//			continue;
	//
	//		FbxNodeAttribute::EType fbxNodeAttributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
	//		if (fbxNodeAttributeType != FbxNodeAttribute::eMesh)
	//			continue;
	//
	//		fbxMesh = (FbxMesh*)fbxChildNode->GetNodeAttribute();
	//		FbxVector4* vertices = fbxMesh->GetControlPoints();
	//
	//		for (int j = 0; j < fbxMesh->GetPolygonCount(); j++)
	//		{
	//			int numverts = fbxMesh->GetPolygonSize(j);
	//
	//			for (int k = 0; k < numverts; k++)
	//			{
	//				int vertexindex = fbxMesh->GetPolygonVertex(j, k);
	//
	//				MyVertex vertex;
	//				vertex.pos[0] = (float)vertices[vertexindex].mData[0];
	//				vertex.pos[1] = (float)vertices[vertexindex].mData[0];
	//				vertex.pos[2] = (float)vertices[vertexindex].mData[0];
	//				outVertexVector->push_back(vertex);
	//			}
	//		}
	//
	//	}
	//}
	return S_OK;
}

HRESULT FBXStuff::UVsToo(std::vector<MyVertex>* outVertexVector)
{
	//if (fbxManager == nullptr)
	//{
	//	fbxManager = FbxManager::Create(); // creates the fbx manager
	//
	//	fbxIOSettings = FbxIOSettings::Create(fbxManager, "IOSROOT"); // creates the IOSettings
	//	fbxManager->SetIOSettings(fbxIOSettings); // sets the manager's IOSettings to be the new settins we just created
	//}
	//fbxImporter = FbxImporter::Create(fbxManager, ""); // creates the importer
	//fbxScene = FbxScene::Create(fbxManager, ""); // create the scene
	//
	//bool OKImport = fbxImporter->Initialize("F:\Program Files (x86)\RTA\FBX\Box_Jump.fbx", -1, fbxManager->GetIOSettings());
	//if (!OKImport)
	//	return E_FAIL;
	//
	//OKImport = fbxImporter->Import(fbxScene);
	//if (!OKImport)
	//	return E_FAIL;
	//
	//fbxImporter->Destroy();
	//
	//FbxNode* fbxRootNode = fbxScene->GetRootNode();
	//
	//if (fbxRootNode)
	//{
	//	for (int i = 0; i < fbxRootNode->GetChildCount(); i++)
	//	{
	//		FbxNode* fbxChildNode = fbxRootNode->GetChild(i);
	//
	//
	//		if (fbxChildNode->GetNodeAttribute() == NULL)
	//			continue;
	//
	//		FbxNodeAttribute::EType fbxNodeAttributeType = fbxChildNode->GetNodeAttribute()->GetAttributeType();
	//		if (fbxNodeAttributeType != FbxNodeAttribute::eMesh)
	//			continue;
	//
	//		fbxMesh = (FbxMesh*)fbxChildNode->GetNodeAttribute();
	//
	//		// getting the UV's from the mesh!
	//
	//		FbxStringList fbxStrings;
	//		fbxMesh->GetUVSetNames(fbxStrings);
	//		for (int UVI = 0; UVI < fbxStrings.GetCount(); UVI++)
	//		{
	//			char* uvsetname = fbxStrings.GetStringAt(UVI);
	//			FbxGeometryElementUV* fbxUVElement = fbxMesh->GetElementUV(uvsetname);
	//			if (!fbxUVElement)
	//				continue;
	//
	//			if (fbxUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex)
	//				return E_FAIL;
	//
	//			//index array, where holds the index referenced to the uv data
	//			bool okuvindex = fbxUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;;
	//
	//			FbxVector4* vertices = fbxMesh->GetControlPoints();
	//
	//			for (int j = 0; j < fbxMesh->GetPolygonCount(); j++)
	//			{
	//				int numverts = fbxMesh->GetPolygonSize(j);
	//
	//				for (int k = 0; k < numverts; k++)
	//				{
	//					int vertexindex = fbxMesh->GetPolygonVertex(j, k);
	//
	//					//gets the uv index and fills the fbxvctor 2 of the array of uvs at an index
	//					/////
	//					FbxVector4 UVs;
	//					int uvindex;
	//					if (okuvindex)
	//						uvindex = fbxUVElement->GetIndexArray().GetAt(vertexindex);
	//					else
	//						uvindex = vertexindex;
	//					UVs = fbxUVElement->GetDirectArray().GetAt(uvindex);
	//					/////
	//					MyVertex vertex;
	//					vertex.pos[0] = (float)vertices[vertexindex].mData[0];
	//					vertex.pos[1] = (float)vertices[vertexindex].mData[0];
	//					vertex.pos[2] = (float)vertices[vertexindex].mData[0];
	//					vertex.uv[0] = (float)UVs.mData[0];
	//					vertex.uv[1] = (float)UVs.mData[1];
	//					vertex.uv[2] = (float)UVs.mData[2];
	//
	//
	//
	//					outVertexVector->push_back(vertex);
	//				}
	//			}
	//		}
	//
	//	}
	//}
	return S_OK;
}

HRESULT FBXStuff::NormalsAndUVsToo(std::vector<MyVertex>& outVertexVector)
{
	if (fbxManager == nullptr)
	{
		fbxManager = FbxManager::Create(); // creates the fbx manager

		fbxIOSettings = FbxIOSettings::Create(fbxManager, "IOSROOT"); // creates the IOSettings
		fbxManager->SetIOSettings(fbxIOSettings); // sets the manager's IOSettings to be the new settins we just created
	}
	fbxImporter = FbxImporter::Create(fbxManager, ""); // creates the importer
	fbxScene = FbxScene::Create(fbxManager, ""); // create the scene

	bool OKImport = fbxImporter->Initialize("F:\\Program Files (x86)\\RTA\\FBX\\Box_Jump.fbx", -1, fbxManager->GetIOSettings());
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
			
					FbxVector4* vertices = fbxMesh->GetControlPoints();
			
					MyVertex vertex;
			
					// get the vertices
					int vertexindex;
			
					for (int j = 0; j < fbxMesh->GetPolygonCount(); j++)
					{
						int numverts = fbxMesh->GetPolygonSize(j);
			
						for (int k = 0; k < numverts; k++)
						{
							vertexindex = fbxMesh->GetPolygonVertex(j, k);
			
							vertex.pos.x = (float)vertices[vertexindex].mData[0];
							vertex.pos.y = (float)vertices[vertexindex].mData[1];
							vertex.pos.z = (float)vertices[vertexindex].mData[2];
			
						}
					}
			
			
					// getting the UV's from the mesh!
			
					FbxStringList fbxStrings;
					fbxMesh->GetUVSetNames(fbxStrings);
					for (int UVI = 0; UVI < fbxStrings.GetCount(); UVI++)
					{
						char* uvsetname = fbxStrings.GetStringAt(UVI);
						FbxGeometryElementUV* fbxUVElement = fbxMesh->GetElementUV(uvsetname);
						if (!fbxUVElement)
							continue;
			
						if (fbxUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
							return E_FAIL;
			
						//index array, where holds the index referenced to the uv data
						bool okuvindex = fbxUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;;
			
						for (int j = 0; j < fbxMesh->GetPolygonCount(); j++)
						{
							int numverts = fbxMesh->GetPolygonSize(j);
			
							for (int k = 0; k < numverts; k++)
							{
								int vertexindex = fbxMesh->GetPolygonVertex(j, k);
			
								//gets the uv index and fills the fbxvctor 2 of the array of uvs at an index
								/////
								FbxVector4 UVs;
								int uvindex;
								if (okuvindex)
									uvindex = fbxUVElement->GetIndexArray().GetAt(vertexindex);
								else
									uvindex = vertexindex;
								UVs = fbxUVElement->GetDirectArray().GetAt(uvindex);
								/////
			
								vertex.uvm.x = (float)UVs.mData[0];
								vertex.uvm.y = (float)UVs.mData[1];
								vertex.uvm.z = (float)UVs.mData[2];
			
							}
						}
			
					}
			
					// NORMALS!
			
					FbxGeometryElementNormal* normalelement = fbxMesh->GetElementNormal();
			
					if (!normalelement)
						continue;
			
					if (normalelement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
						return E_FAIL;
			
			
			
					for (int j = 0; j < fbxMesh->GetPolygonCount(); j++)
					{
						int numverts = fbxMesh->GetPolygonSize(j);
			
						for (int k = 0; k < numverts; k++)
						{
							int vertexindex = fbxMesh->GetPolygonVertex(j, k);
			
							int normalsindex = 0;
			
							if (normalelement->GetReferenceMode() == FbxGeometryElement::eDirect)
								normalsindex = vertexindex;
							if (normalelement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
								normalsindex = normalelement->GetIndexArray().GetAt(vertexindex);
							FbxVector4 normals = normalelement->GetDirectArray().GetAt(normalsindex);
			
							/////
							vertex.nrm.x= (float)normals.mData[0];
							vertex.nrm.y= (float)normals.mData[1];
							vertex.nrm.z= (float)normals.mData[2];
			
						}
					}
			//
			//
					outVertexVector.push_back(vertex);
				}
			}
			return S_OK;
		}
