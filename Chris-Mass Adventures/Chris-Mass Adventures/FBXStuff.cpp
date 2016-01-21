#include "stdafx.h"
//#include "fbxsdk.h"
#include <vector>
#include <fstream>
#include "FBXStuff.h"

HRESULT FBXStuff::LoadFBX(std::vector<VERTEX>& outVertexVector, const char * _Filename)
{


	FbxIOFileHeaderInfo* fbxheader = nullptr;
	std::string filename = std::string(_Filename);
	filename += ".fbx";

	if (fbxManager == nullptr)
	{
		fbxManager = FbxManager::Create(); // creates the fbx manager

		fbxIOSettings = FbxIOSettings::Create(fbxManager, "IOSROOT"); // creates the IOSettings
		fbxManager->SetIOSettings(fbxIOSettings); // sets the manager's IOSettings to be the new settins we just created
	}

	fbxImporter = FbxImporter::Create(fbxManager, ""); // creates the importer
	fbxScene = FbxScene::Create(fbxManager, ""); // create the scene

	fbxheader = fbxImporter->GetFileHeaderInfo();

	if (BinaryIn(outVertexVector, (*fbxheader), _Filename) == S_OK)
	{
		return S_OK;
	}

	bool OKImport = fbxImporter->Initialize(filename.c_str(), -1, fbxManager->GetIOSettings());
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
					int uvIndex = fbxMesh->GetTextureUVIndex(j, k);

					FbxVector2 vertexUVs = UVs->GetAt(uvIndex);

					vertex.uvw[0] = (float)vertexUVs.mData[0];
					vertex.uvw[1] = 1 - (float)vertexUVs.mData[1];
					vertex.uvw[2] = 0;

					FbxGeometryElementNormal* normalelement = fbxMesh->GetElementNormal();
					FbxGeometryElementBinormal* binormalelement = fbxMesh->GetElementBinormal();
					FbxGeometryElementTangent* tangentelement = fbxMesh->GetElementTangent();

					if (!normalelement)
						continue;

					//if (normalelement->GetMappingMode() != FbxGeometryElement::eByControlPoint)
					//	return E_FAIL;

					int normalsindex = 0;
					int binormalindex = 0;
					int tangentindex = 0;

					// tries to get normals
					if (normalelement->GetReferenceMode() == FbxGeometryElement::eDirect)
						normalsindex = vertexindex;
					else if (normalelement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						normalsindex = normalelement->GetIndexArray().GetAt(vertexindex);
					FbxVector4 normals = normalelement->GetDirectArray().GetAt(normalsindex);

					/*
					//tries to get binormals
					if (binormalelement->GetReferenceMode() == FbxGeometryElement::eDirect)
						binormalindex = vertexindex;
					else if (binormalelement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						binormalindex = binormalelement->GetIndexArray().GetAt(vertexindex);
					FbxVector4 binormals = binormalelement->GetDirectArray().GetAt(binormalindex);

					//tries to get tangents
					if (tangentelement->GetReferenceMode() == FbxGeometryElement::eDirect)
						tangentindex = vertexindex;
					else if (tangentelement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						tangentindex = tangentelement->GetIndexArray().GetAt(vertexindex);
					FbxVector4 tangents = tangentelement->GetDirectArray().GetAt(tangentindex);
					*/
					/////
					vertex.nrm[0] = (float)normals.mData[0];
					vertex.nrm[1] = (float)normals.mData[1];
					vertex.nrm[2] = (float)normals.mData[2];

					/*
					vertex.bin[0] = (float)binormals.mData[0];
					vertex.bin[1] = (float)binormals.mData[1];
					vertex.bin[2] = (float)binormals.mData[2];

					vertex.tan[0] = (float)tangents.mData[0];
					vertex.tan[1] = (float)tangents.mData[1];
					vertex.tan[2] = (float)tangents.mData[2];
					*/
					outVertexVector.push_back(vertex);
				}


			}

		}
	}
	return BinaryOut(outVertexVector, (*fbxheader), _Filename);
}

HRESULT FBXStuff::BinaryOut(std::vector<VERTEX>&inVertexVector, FbxIOFileHeaderInfo& fileheader, const char* _Filename)
{
	std::string filename = std::string(_Filename);
	filename += ".bin";

	std::fstream binaryStream;
	if (!binaryStream.is_open())
		binaryStream.open(filename.c_str(), std::ios_base::out | std::ios_base::binary);
	else
		return E_FAIL;

	if (binaryStream.is_open())
	{
		// declare header
		BinaryHeader header;

		// initialize header
		header.file_size = (inVertexVector.size() * sizeof(VERTEX));
		header.vector_size = inVertexVector.size();
		if (fileheader.mFileVersion > 0)
			header.fileversion = fileheader.mFileVersion;
		else
			header.fileversion = -1;

		if (fileheader.mCreationTimeStamp.mYear > 0)
			header.timestamp = fileheader.mCreationTimeStamp;
		else
			header.timestamp.mYear = -1;

		// write out header
		binaryStream.write((const char*)&header, sizeof(BinaryHeader));

		binaryStream.write((const char*)&inVertexVector[0], header.file_size);

		binaryStream.close();
	}
	else
		return E_FAIL;

	return S_OK;
}

HRESULT FBXStuff::BinaryIn(std::vector<VERTEX>&outVertexVector, FbxIOFileHeaderInfo& fileheader, const char* _Filename)
{
	std::string filename = std::string(_Filename);
	filename += ".bin";

	std::fstream binaryStream;
	if (!binaryStream.is_open())
		binaryStream.open(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	else
		return E_FAIL;

	if (binaryStream.is_open())
	{
		// declare header
		BinaryHeader header;

		// read in header
		binaryStream.read((char*)&header, sizeof(BinaryHeader));

		if (header.fileversion != -1)
		{
			if (header.fileversion < fileheader.mFileVersion)
				return E_FAIL;
		}
		if (header.timestamp.mYear != -1)
		{
			if (header.timestamp.mYear < fileheader.mCreationTimeStamp.mYear)
				return E_FAIL;
			else
				if (header.timestamp.mMonth < fileheader.mCreationTimeStamp.mMonth)
					return E_FAIL;
				else
					if (header.timestamp.mDay < fileheader.mCreationTimeStamp.mDay)
						return E_FAIL;
					else
						if (header.timestamp.mHour < fileheader.mCreationTimeStamp.mHour)
							return E_FAIL;
						else
							if (header.timestamp.mMinute < fileheader.mCreationTimeStamp.mMinute)
								return E_FAIL;
							else
								if (header.timestamp.mSecond < fileheader.mCreationTimeStamp.mSecond)
									return E_FAIL;
		}

		outVertexVector.resize(header.file_size);
		binaryStream.read((char*)&outVertexVector[0], header.file_size);

		binaryStream.close();
	}
	else
		return E_FAIL;
	return S_OK;
}