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
	
	// is the fine exixts=
//	if (BinaryIn(outVertexVector, (*fbxheader), _Filename) == S_OK)
//	{
//		return S_OK;
//	}

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
	

	// http://stackoverflow.com/questions/13566608/loading-skinning-information-from-fbx


	if (fbxRootNode)
	{
		ProcessGeometry(fbxRootNode);
	}
	
	


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

void FBXStuff::ProcessSkeletonHierarchy(FbxNode* inRootNode)
{
	int numChildren = inRootNode->GetChildCount();

	for (int childIndex = 0; childIndex < numChildren; ++childIndex)
	{
		FbxNode* currNode = inRootNode->GetChild(childIndex);
		ProcessSkeletonHierarchyRecursively(currNode, 0, 0, -1);
	}
}

void FBXStuff::ProcessJointsAndAnimations(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int numOfDeformers = currMesh->GetDeformerCount();
	// This geometry transform is something I cannot understand
	// I think it is from MotionBuilder
	// If you are using Maya for your models, 99% this is just an
	// identity matrix
	// But I am taking it into account anyways......
	FbxAMatrix geometryTransform = Utilities::GetGeometryTransformation(inNode);

	// A deformer is a FBX thing, which contains some clusters
	// A cluster contains a link, which is basically a joint
	// Normally, there is only one deformer in a mesh
	for (unsigned int deformerIndex = 0; deformerIndex < numOfDeformers; ++deformerIndex)
	{
		// There are many types of deformers in Maya,
		// We are using only skins, so we see if this is a skin
		FbxSkin* currSkin = reinterpret_cast<FbxSkin*>(currMesh->GetDeformer(deformerIndex, FbxDeformer::eSkin));
		if (!currSkin)
		{
			continue;
		}

		unsigned int numOfClusters = currSkin->GetClusterCount();
		for (unsigned int clusterIndex = 0; clusterIndex < numOfClusters; ++clusterIndex)
		{
			FbxCluster* currCluster = currSkin->GetCluster(clusterIndex);
			std::string currJointName = currCluster->GetLink()->GetName();
			unsigned int currJointIndex = FindJointIndexUsingName(currJointName);
			FbxAMatrix transformMatrix;
			FbxAMatrix transformLinkMatrix;
			FbxAMatrix globalBindposeInverseMatrix;

			currCluster->GetTransformMatrix(transformMatrix);	// The transformation of the mesh at binding time
			currCluster->GetTransformLinkMatrix(transformLinkMatrix);	// The transformation of the cluster(joint) at binding time from joint space to world space
			globalBindposeInverseMatrix = transformLinkMatrix.Inverse() * transformMatrix * geometryTransform;

			// Update the information in mSkeleton 
			mSkeleton.mJoints[currJointIndex].mGlobalBindposeInverse = globalBindposeInverseMatrix;
			mSkeleton.mJoints[currJointIndex].mNode = currCluster->GetLink();

			// Associate each joint with the control points it affects
			unsigned int numOfIndices = currCluster->GetControlPointIndicesCount();
			for (unsigned int i = 0; i < numOfIndices; ++i)
			{
				BlendingIndexWeightPair currBlendingIndexWeightPair;
				currBlendingIndexWeightPair.mBlendingIndex = currJointIndex;
				currBlendingIndexWeightPair.mBlendingWeight = currCluster->GetControlPointWeights()[i];
				mControlPoints[currCluster->GetControlPointIndices()[i]]->mBlendingInfo.push_back(currBlendingIndexWeightPair);
			}

			// Get animation information
			// Now only supports one take
			FbxAnimStack * test;

			FbxAnimStack* currAnimStack = fbxScene->GetCurrentAnimationStack();
			FbxString animStackName = currAnimStack->GetName();
			mAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			Keyframe** currAnim = &mSkeleton.mJoints[currJointIndex].mAnimation;
			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames24);
				*currAnim = new Keyframe();
				(*currAnim)->mFrameNum = i;
				FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * geometryTransform;
				(*currAnim)->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
				currAnim = &((*currAnim)->mNext);
			}
#if 0   // old API reference
			FbxAnimStack* currAnimStack = fbxScene->GetSrcObject<FbxAnimStack>(0);
			FbxString animStackName = currAnimStack->GetName();
			mAnimationName = animStackName.Buffer();
			FbxTakeInfo* takeInfo = fbxScene->GetTakeInfo(animStackName);
			FbxTime start = takeInfo->mLocalTimeSpan.GetStart();
			FbxTime end = takeInfo->mLocalTimeSpan.GetStop();
			mAnimationLength = end.GetFrameCount(FbxTime::eFrames24) - start.GetFrameCount(FbxTime::eFrames24) + 1;
			Keyframe** currAnim = &mSkeleton.mJoints[currJointIndex].mAnimation;

			for (FbxLongLong i = start.GetFrameCount(FbxTime::eFrames24); i <= end.GetFrameCount(FbxTime::eFrames24); ++i)
			{
				FbxTime currTime;
				currTime.SetFrame(i, FbxTime::eFrames24);
				*currAnim = new Keyframe();
				(*currAnim)->mFrameNum = i;
				FbxAMatrix currentTransformOffset = inNode->EvaluateGlobalTransform(currTime) * geometryTransform;
				(*currAnim)->mGlobalTransform = currentTransformOffset.Inverse() * currCluster->GetLink()->EvaluateGlobalTransform(currTime);
				currAnim = &((*currAnim)->mNext);
			}
#endif
		}
	}

	// Some of the control points only have less than 4 joints
	// affecting them.
	// For a normal renderer, there are usually 4 joints
	// I am adding more dummy joints if there isn't enough
	BlendingIndexWeightPair currBlendingIndexWeightPair;
	currBlendingIndexWeightPair.mBlendingIndex = 0;
	currBlendingIndexWeightPair.mBlendingWeight = 0;
	for (auto itr = mControlPoints.begin(); itr != mControlPoints.end(); ++itr)
	{
		for (unsigned int i = itr->second->mBlendingInfo.size(); i <= 4; ++i)
		{
			itr->second->mBlendingInfo.push_back(currBlendingIndexWeightPair);
		}
	}
}


void FBXStuff::ProcessSkeletonHierarchyRecursively(FbxNode* inNode, int inDepth, int myIndex, int inParentIndex)
{
	if (inNode->GetNodeAttribute() && inNode->GetNodeAttribute()->GetAttributeType() && inNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
	{
		Joint currJoint;
		currJoint.mParentIndex = inParentIndex;
		currJoint.mName = inNode->GetName();
		mSkeleton.mJoints.push_back(currJoint);
	}
	for (int i = 0; i < inNode->GetChildCount(); i++)
	{
		ProcessSkeletonHierarchyRecursively(inNode->GetChild(i), inDepth + 1, mSkeleton.mJoints.size(), myIndex);
	}
}

void FBXStuff::ProcessControlPoints(FbxNode* inNode)
{
	FbxMesh* currMesh = inNode->GetMesh();
	unsigned int ctrlPointCount = currMesh->GetControlPointsCount();
	for (unsigned int i = 0; i < ctrlPointCount; ++i)
	{
		CtrlPoint* currCtrlPoint = new CtrlPoint();
		XMFLOAT3 currPosition;
		currPosition.x = static_cast<float>(currMesh->GetControlPointAt(i).mData[0]);
		currPosition.y = static_cast<float>(currMesh->GetControlPointAt(i).mData[1]);
		currPosition.z = static_cast<float>(currMesh->GetControlPointAt(i).mData[2]);
		currCtrlPoint->mPosition = currPosition;
		mControlPoints[i] = currCtrlPoint;
	}
}

void FBXStuff::ProcessGeometry(FbxNode* inNode)
{
	if (inNode->GetNodeAttribute())
	{
		switch (inNode->GetNodeAttribute()->GetAttributeType())
		{
		case FbxNodeAttribute::eMesh:
			ProcessControlPoints(inNode);
			if (mHasAnimation)
			{
				ProcessSkeletonHierarchy(fbxScene->GetRootNode());
				ProcessJointsAndAnimations(inNode);
			}
			//	ProcessMesh(inNode);
			//	AssociateMaterialToMesh(inNode);
			//	ProcessMaterials(inNode);  << REMOVED BECAUSE WE ARE USING OUR OWN 
			break;
		}
	}

	for (int i = 0; i < inNode->GetChildCount(); ++i)
	{
		ProcessGeometry(inNode->GetChild(i));
	}
}

unsigned int FBXStuff::FindJointIndexUsingName(const std::string& inJointName)
{
	for (unsigned int i = 0; i < mSkeleton.mJoints.size(); ++i)
	{
		if (mSkeleton.mJoints[i].mName == inJointName)
		{
			return i;
		}
	}

	throw std::exception("Skeleton information in FBX file is corrupted.");
}

void FBXStuff::CleanupFbxManager()
{
	//	fbxScene->Destroy();
	//fbxManager->Destroy();

	//mTriangles.clear();

	//mVertices.clear();

	//	mSkeleton.mJoints.clear();

	//for (auto itr = mMaterialLookUp.begin(); itr != mMaterialLookUp.end(); ++itr)
//	{
//		delete itr->second;
//	}
//	mMaterialLookUp.clear();
}

