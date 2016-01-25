#include "stdafx.h"
//#include "fbxsdk.h"
#include <vector>
#include <fstream>
#include "FBXStuff.h"



using namespace DirectX;

HRESULT FBXStuff::LoadFBX(std::vector<VERTEX>& outVertexVector, const char * _Filename, ID3D11Buffer ** outbuffer)
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

	// bitangent and binormal calculation
	int faceCount;
	int index = 0;
	VERTEX vertex1, vertex2, vertex3;
	XMFLOAT3 tangent, binormal, normal;

	faceCount = outVertexVector.size() / 3;
	for (int i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.pos[0] = outVertexVector[index].pos[0];
		vertex1.pos[1] = outVertexVector[index].pos[1];
		vertex1.pos[2] = outVertexVector[index].pos[2];

		vertex1.nrm[0] = outVertexVector[index].nrm[0];
		vertex1.nrm[1] = outVertexVector[index].nrm[1];
		vertex1.nrm[2] = outVertexVector[index].nrm[2];

		vertex1.uvw[0] = outVertexVector[index].uvw[0];
		vertex1.uvw[1] = outVertexVector[index].uvw[1];
		vertex1.uvw[2] = outVertexVector[index].uvw[2];

		index++;

		vertex2.pos[0] = outVertexVector[index].pos[0];
		vertex2.pos[1] = outVertexVector[index].pos[1];
		vertex2.pos[2] = outVertexVector[index].pos[2];

		vertex2.nrm[0] = outVertexVector[index].nrm[0];
		vertex2.nrm[1] = outVertexVector[index].nrm[1];
		vertex2.nrm[2] = outVertexVector[index].nrm[2];

		vertex2.uvw[0] = outVertexVector[index].uvw[0];
		vertex2.uvw[1] = outVertexVector[index].uvw[1];
		vertex2.uvw[2] = outVertexVector[index].uvw[2];

		index++;

		vertex3.pos[0] = outVertexVector[index].pos[0];
		vertex3.pos[1] = outVertexVector[index].pos[1];
		vertex3.pos[2] = outVertexVector[index].pos[2];

		vertex3.nrm[0] = outVertexVector[index].nrm[0];
		vertex3.nrm[1] = outVertexVector[index].nrm[1];
		vertex3.nrm[2] = outVertexVector[index].nrm[2];

		vertex3.uvw[0] = outVertexVector[index].uvw[0];
		vertex3.uvw[1] = outVertexVector[index].uvw[1];
		vertex3.uvw[2] = outVertexVector[index].uvw[2];

		index++;

		// Calculate the tangent and binormal of that face
		CalculateTangentBinormal(vertex1, vertex2, vertex3, &tangent, &binormal);

		CalculateNormal(tangent, binormal, normal);

		///float length;

		//normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
		//normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
		//normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

		// work out the new length
		//length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));


		//normal.x = normal.x / length;
		//normal.y = normal.y / length;
		//normal.z = normal.z / length;

		//	model_data[index - 1].nrm = normal;
		outVertexVector[index - 1].tan[0] = tangent.x;
		outVertexVector[index - 1].tan[1] = tangent.y;
		outVertexVector[index - 1].tan[2] = tangent.z;

		outVertexVector[index - 1].bin[0] = binormal.x;
		outVertexVector[index - 1].bin[1] = binormal.y;
		outVertexVector[index - 1].bin[2] = binormal.z;
		//vertexvector[index - 1].bin = binormal;

		//	model_data[index - 2].nrm = normal;
		outVertexVector[index - 2].tan[0] = tangent.x;
		outVertexVector[index - 2].tan[1] = tangent.y;
		outVertexVector[index - 2].tan[2] = tangent.z;

		outVertexVector[index - 2].bin[0] = binormal.x;
		outVertexVector[index - 2].bin[1] = binormal.y;
		outVertexVector[index - 2].bin[2] = binormal.z;

		outVertexVector[index - 3].tan[0] = tangent.x;
		outVertexVector[index - 3].tan[1] = tangent.y;
		outVertexVector[index - 3].tan[2] = tangent.z;

		outVertexVector[index - 3].bin[0] = binormal.x;
		outVertexVector[index - 3].bin[1] = binormal.y;
		outVertexVector[index - 3].bin[2] = binormal.z;
	}
	// Animation Processing

	// int numStacks = fbxScene->GetSrcObjectCount(FBX_TYPE(FbxAnimStack));// GetSrcObjectCount(FBX_TYPE(FbxAnimStack)); i++);



	return BinaryOut(outVertexVector, (*fbxheader), _Filename);
}

void FBXStuff::CalculateTangentBinormal(VERTEX vertex1, VERTEX vertex2, VERTEX vertex3, XMFLOAT3* tangent, XMFLOAT3* binormal)
{

	float vector1[3];
	float vector2[3];
	float tuVector[2];
	float tvVector[2];
	float den;
	float length;


	// Calculate the two vectors for this face.
	vector1[0] = vertex2.pos - vertex1.pos;
	vector2[0] = vertex3.pos - vertex1.pos;
	//vector1[0] = vertex2.pos.x - vertex1.pos.x;
	//vector1[1] = vertex2.pos.y - vertex1.pos.y;
	//vector1[2] = vertex2.pos.z - vertex1.pos.z;

	//vector2[0] = vertex3.pos.x - vertex1.pos.x;
	//vector2[1] = vertex3.pos.y - vertex1.pos.y;
	//vector2[2] = vertex3.pos.z - vertex1.pos.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.uvw - vertex1.uvw;
	//tuVector[0] = vertex2.uvw.x - vertex1.uvw.x;
	//tvVector[0] = vertex2.uvw.y - vertex1.uvw.y;

	tuVector[1] = vertex3.uvw - vertex1.uvw;
	//tuVector[1] = vertex3.uvw.x - vertex1.uvw.x;
	//tvVector[1] = vertex3.uvw.y - vertex1.uvw.y;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent->x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent->y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent->z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal->x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal->y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal->z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent->x * tangent->x) + (tangent->y * tangent->y) + (tangent->z * tangent->z));

	// Normalize the normal and then store it
	tangent->x = tangent->x / length;
	tangent->y = tangent->y / length;
	tangent->z = tangent->z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal->x * binormal->x) + (binormal->y * binormal->y) + (binormal->z * binormal->z));

	// Normalize the normal and then store it
	binormal->x = binormal->x / length;
	binormal->y = binormal->y / length;
	binormal->z = binormal->z / length;

	return;
}

void FBXStuff::CalculateNormal(XMFLOAT3 tangent, XMFLOAT3 binormal, XMFLOAT3& normal)
{
	float length;


	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;

	return;
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