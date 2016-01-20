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

void FBXStuff::LoadUVInformation(FbxMesh* pMesh, std::vector<VERTEX>* outVertexVector)
{
	//get all UV set names
	FbxStringList lUVSetNameList;
	pMesh->GetUVSetNames(lUVSetNameList);

	//iterating over all uv sets
	for (int lUVSetIndex = 0; lUVSetIndex < lUVSetNameList.GetCount(); lUVSetIndex++)
	{
		//get lUVSetIndex-th uv set
		const char* lUVSetName = lUVSetNameList.GetStringAt(lUVSetIndex);
		const FbxGeometryElementUV* lUVElement = pMesh->GetElementUV(lUVSetName);

		if (!lUVElement)
			continue;

		// only support mapping mode eByPolygonVertex and eByControlPoint
		if (lUVElement->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			lUVElement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
			return;

		//index array, where holds the index referenced to the uv data
		const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;
		const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;

		//iterating through the data by polygon
		const int lPolyCount = pMesh->GetPolygonCount();

		if (lUVElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					FbxVector2 lUVValue;

					//get the index of the current vertex in control points array
					int lPolyVertIndex = pMesh->GetPolygonVertex(lPolyIndex, lVertIndex);

					//the UV index depends on the reference mode
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;

					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
					
					//User TODO:
					//Print out the value of UV(lUVValue) or log it to a file
				}
			}
		}
		else if (lUVElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int lPolyIndexCounter = 0;
			for (int lPolyIndex = 0; lPolyIndex < lPolyCount; ++lPolyIndex)
			{
				// build the max index array that we need to pass into MakePoly
				const int lPolySize = pMesh->GetPolygonSize(lPolyIndex);
				for (int lVertIndex = 0; lVertIndex < lPolySize; ++lVertIndex)
				{
					if (lPolyIndexCounter < lIndexCount)
					{
						FbxVector2 lUVValue;

						//the UV index depends on the reference mode
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;

						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);

						//User TODO:
						//Print out the value of UV(lUVValue) or log it to a file
						// Convert to floats
						VERTEX tempVertexforUV;
						float UVCoord[3];
						tempVertexforUV.uvw[0] = static_cast<float>(lUVValue[0]);
						tempVertexforUV.uvw[1] = static_cast<float>(lUVValue[1]);
						tempVertexforUV.uvw[2] = 0.0f;
						outVertexVector->push_back(tempVertexforUV);

						lPolyIndexCounter++;
					}
				}
			}
		}
	}
}



HRESULT FBXStuff::NormalsAndUVsToo(std::vector<VERTEX>& outVertexVector, const char * _Filename)
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

			FbxLayerElementArrayTemplate<FbxVector2> * UVs;
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
					int uvIndex = fbxMesh->GetTextureUVIndex(j,k);

					FbxVector2 vertexUVs = UVs->GetAt(uvIndex);

					vertex.uvw[0] = (float)vertexUVs.mData[0];
					vertex.uvw[1] = 1 - (float)vertexUVs.mData[1];
					vertex.uvw[2] = 0;

					// NORMALS!

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

					outVertexVector.push_back(vertex);

				}
			}

		}
	}

	return S_OK;
}
