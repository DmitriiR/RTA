#include "stdafx.h"
#include "Clip.h"


using namespace EDMath;


Clip* Clip::Load(const char *fileName, const char *ignored)
{
	//LOADING CLIP HERE 
	Clip *newAnim = new Clip;
	/* 
	string temp = ContentManager::theContentPath;
	temp += fileName;

	InternalOutput::GetReference() << "Loading " << temp << "...\n";

	ifstream fin(temp.c_str(), std::ios_base::in | std::ios_base::binary);

	if (!fin.is_open())
		InternalOutput::GetReference() << "Failed to open" << fileName << ", does the file exist\n";

	Clip *newAnim = new Clip;

	// read in length of animation
	unsigned int nameLength;
	fin.read((char *)&nameLength, sizeof(nameLength));

	// read in animation name
	char *animationName = new char[nameLength];
	fin.read(animationName, sizeof(char) * nameLength);

	// read animation duration
	fin.read((char *)&newAnim->duration, sizeof(newAnim->duration));

	// read number of bones
	fin.read((char *)&newAnim->numBones, sizeof(newAnim->numBones));

	// reserve memory for the bones to prevent resizing issues
	newAnim->bones.reserve(newAnim->numBones);

	newAnim->bindTransforms.resize(newAnim->numBones);

	// for each bone
	for (unsigned int boneIndex = 0; boneIndex < newAnim->numBones; ++boneIndex)
	{
		// read the bone index value from file
		unsigned int fileBoneIndex;
		fin.read((char *)&fileBoneIndex, sizeof(fileBoneIndex));

		// read the bone index value from file
		int parentIndex;
		fin.read((char *)&parentIndex, sizeof(parentIndex));

		unsigned int nameLen;
		fin.read((char*)&nameLen, sizeof(nameLen));

		char* nameBuffer = new char[nameLen + 1];
		nameBuffer[nameLen] = 0;
		fin.read(nameBuffer, nameLen);

		// read number of keyframes
		unsigned int numKeyframes;
		fin.read((char *)&numKeyframes, sizeof(numKeyframes));

		// Add a bone to work with
		newAnim->bones.push_back(AnimatedBone(parentIndex, nameBuffer));
		AnimatedBone &bone = newAnim->bones[newAnim->bones.size() - 1];

		delete[] nameBuffer;

		std::vector<KeyFrame> fileKeyFrames;
		fileKeyFrames.reserve(numKeyframes);
		// for each keyframe
		for (unsigned int keyFrameIndex = 0; keyFrameIndex < numKeyframes; ++keyFrameIndex)
		{
			// Read in the file key frame index
			unsigned int fileKeyIndex;
			fin.read((char *)&fileKeyIndex, sizeof(fileKeyIndex));

			// Read keyFrame time
			float keyTime;
			fin.read((char *)&keyTime, sizeof(keyTime));

			// Read in the local space matrix
			EDMath::Float4x4 local;
			fin.read((char *)&local, sizeof(local));

			// Build and insert keyframe
			KeyFrame keyFrame(keyTime, local);
			fileKeyFrames.push_back(keyFrame);
			//bone.AddKeyFrame(keyFrame);
		}
		newAnim->AddKeyFrames(bone, fileKeyFrames);

		EDMath::Float4x4 mat = fileKeyFrames[0].GetLocalTransform();
		EDMath::Float4x4 bindTransform = mat;

		EDMath::Float3 f3X = mat.XAxis;
		EDMath::Float3 f3Y = mat.YAxis;
		EDMath::Float3 f3Z = mat.ZAxis;
		EDMath::Float3 f3W = mat.WAxis;

		// new W axis is the negative dot of the old W axis with X,Y,Z
		bindTransform.Wx = -f3X.dotProduct(f3W);
		bindTransform.Wy = -f3Y.dotProduct(f3W);
		bindTransform.Wz = -f3Z.dotProduct(f3W);

		// transpose x,y,z axis
		bindTransform.transpose3x3();

		// Apply parenting if needed
		if (-1 != parentIndex)
			EDMath::Multiply(newAnim->bindTransforms[fileBoneIndex], bindTransform, newAnim->bindTransforms[parentIndex]);
		else
			newAnim->bindTransforms[fileBoneIndex] = bindTransform;
	}

	delete[] animationName;
	*/
	return newAnim;
}

void Clip::AddKeyFrames(AnimatedBone &bone, std::vector<KeyFrame> &fileKeyFrames)
{
	// We should always have one bind pose keyframe, and at least one set of keyframes to interpolate through
	assert(fileKeyFrames.size() > 2);

	// Add the bind pose frame
	bone.AddKeyFrame(fileKeyFrames[0]);

	// Add the first keyFrame
	bone.AddKeyFrame(fileKeyFrames[1]);

	// Test each additional keyframe
	unsigned int prevIndex = 1;
	unsigned int currIndex = 2;
	unsigned int nextIndex = 3;
	size_t numKeyFrames = fileKeyFrames.size();

	while (currIndex < numKeyFrames)
	{
		if (nextIndex >= numKeyFrames)
		{
			bone.AddKeyFrame(fileKeyFrames[currIndex]);
			break;
		}

		float threshold = 0.000265f;//0.0265f;

		float totalTime = fileKeyFrames[nextIndex].GetTime() - bone.GetKeyFrames().back().GetTime(); //fileKeyFrames[prevIndex].GetTime();
		float deltaTime = fileKeyFrames[currIndex].GetTime() - bone.GetKeyFrames().back().GetTime();//fileKeyFrames[prevIndex].GetTime();
		//float totalTime = fileKeyFrames[nextIndex].GetTime() - fileKeyFrames[prevIndex].GetTime();
		//float deltaTime = fileKeyFrames[currIndex].GetTime() - fileKeyFrames[prevIndex].GetTime();
		float delta = deltaTime / totalTime;
		
		Float4x4 interp = Interpolate(bone.GetKeyFrames().back().GetLocalTransform()/*fileKeyFrames[prevIndex].GetLocalTransform()*/,
			fileKeyFrames[nextIndex].GetLocalTransform(), delta);

		//EDMath::Float4x4 interp = EDMath::Interpolate(fileKeyFrames[prevIndex].GetLocalTransform(),
		//fileKeyFrames[nextIndex].GetLocalTransform(), delta);

		bool bDifferent = !fileKeyFrames[currIndex].GetLocalTransform().CompareComponents(
			interp, threshold);

		if (bDifferent)
		{
			bone.AddKeyFrame(fileKeyFrames[currIndex]);
		}

		prevIndex = currIndex;
		currIndex = nextIndex;
		++nextIndex;
	}
}

Float4x4 Clip::Interpolate(const Float4x4 &matA, const Float4x4 &matB, float delta)
{
	Float4x4 iMat(matA);
	//result = matB;

	// Inverse of MatrixA
	//EDMath::Float4x4 mat = iMat;

	Float3 f3X = iMat.XAxis;
	Float3 f3Y = iMat.YAxis;
	Float3 f3Z = iMat.ZAxis;
	Float3 f3W = iMat.WAxis;

	// new W axis is the negative dot of the old W axis with X,Y,Z
	iMat.Wx = -f3X.dotProduct(f3W);
	iMat.Wy = -f3Y.dotProduct(f3W);
	iMat.Wz = -f3Z.dotProduct(f3W);

	// transpose x,y,z axis
	iMat.transpose3x3();

	// Remove MatrixA's transformation from MatrixB
	Multiply(iMat, iMat, matB);

	// iMat is now the intermediary transformation from MatrixA to MatrixB
	// ie: iMat * MatrixA = MatrixB


	// The trace of our matrix
	float trace = 1.0f + iMat.ma[0] + iMat.ma[5] + iMat.ma[10];

	float quatResult[4];

	// Calculate the quaternion of iMat
	// If trace is greater than 0, but consider small values that
	// might result in 0 when operated upon due to floating point error
	if (trace > 0.00000001f)
	{
		float S = sqrt(trace) * 2;
		quatResult[0] = (iMat.ma[9] - iMat.ma[6]) / S;
		quatResult[1] = (iMat.ma[2] - iMat.ma[8]) / S;
		quatResult[2] = (iMat.ma[4] - iMat.ma[1]) / S;
		quatResult[3] = 0.25f * S;
	}
	else
	{
		if (iMat.ma[0] > iMat.ma[5] && iMat.ma[0] > iMat.ma[10])
		{
			float S = sqrt(1.0f + iMat.ma[0] - iMat.ma[5] - iMat.ma[10]) * 2;
			quatResult[0] = 0.25f * S;
			quatResult[1] = (iMat.ma[4] + iMat.ma[1]) / S;
			quatResult[2] = (iMat.ma[2] + iMat.ma[8]) / S;
			quatResult[3] = (iMat.ma[9] - iMat.ma[6]) / S;
		}
		else if (iMat.ma[5] > iMat.ma[10])
		{
			float S = sqrt(1.0f + iMat.ma[5] - iMat.ma[0] - iMat.ma[10]) * 2;
			quatResult[0] = (iMat.ma[4] + iMat.ma[1]) / S;
			quatResult[1] = 0.25f * S;
			quatResult[2] = (iMat.ma[9] + iMat.ma[6]) / S;
			quatResult[3] = (iMat.ma[2] - iMat.ma[8]) / S;
		}
		else
		{
			float S = sqrt(1.0f + iMat.ma[10] - iMat.ma[0] - iMat.ma[5]) * 2;
			quatResult[0] = (iMat.ma[2] + iMat.ma[8]) / S;
			quatResult[1] = (iMat.ma[9] + iMat.ma[6]) / S;
			quatResult[2] = 0.25f * S;
			quatResult[3] = (iMat.ma[4] - iMat.ma[1]) / S;
		}
	}

	// Get the magnitude of our quaternion
	float quatMagnitude = sqrt(quatResult[0] * quatResult[0] + quatResult[1] * quatResult[1] + quatResult[2] * quatResult[2] + quatResult[3] * quatResult[3]);

	// Normalize our quaternion
	float invQuatMag = 1.0f / quatMagnitude;
	float quatNormalized[4] = { quatResult[0] * invQuatMag, quatResult[1] * invQuatMag, quatResult[2] * invQuatMag, quatResult[3] * invQuatMag };

	// Calculate the angles relevant to our quaternion
	float cos_a = quatNormalized[3];
	float angle = acos(cos_a) * 2;
	float sin_a = sqrt(1.0f - cos_a * cos_a);

	// If there was no rotation between matrices, calculation
	// of the rotation matrix will end badly. So just do the linear
	// interpolation of the translation component and return
	if (angle == 0.0f)
	{
		return Float4x4(
			matA.Xx, matA.Xy, matA.Xz, matA.Xw,
			matA.Yx, matA.Yy, matA.Yz, matA.Yw,
			matA.Zx, matA.Zy, matA.Zz, matA.Zw,

			matA.m[3][0] + ((matB.m[3][0] - matA.m[3][0])*delta),
			matA.m[3][1] + ((matB.m[3][1] - matA.m[3][1])*delta),
			matA.m[3][2] + ((matB.m[3][2] - matA.m[3][2])*delta),
			matA.Ww);
	}


	// Our axis of abitrary rotation
	Float3 axis;

	if (fabs(sin_a) < 0.0005f)
		sin_a = 1;

	axis.x = quatNormalized[0] / sin_a;
	axis.y = quatNormalized[1] / sin_a;
	axis.z = quatNormalized[2] / sin_a;

	// Get the portion of the angle to rotate by
	angle *= delta;

	axis.normalize();

	// Calculate the quaternion for our new (partial) angle of rotation
	sin_a = sin(angle / 2);
	cos_a = cos(angle / 2);
	quatNormalized[0] = axis.x * sin_a;
	quatNormalized[1] = axis.y * sin_a;
	quatNormalized[2] = axis.z * sin_a;
	quatNormalized[3] = cos_a;

	quatMagnitude = sqrt(quatNormalized[0] * quatNormalized[0] + quatNormalized[1] * quatNormalized[1] + quatNormalized[2] * quatNormalized[2] + quatNormalized[3] * quatNormalized[3]);
	quatNormalized[0] /= quatMagnitude;
	quatNormalized[1] /= quatMagnitude;
	quatNormalized[2] /= quatMagnitude;
	quatNormalized[3] /= quatMagnitude;

	// Calculate our partial rotation matrix
	float xx = quatNormalized[0] * quatNormalized[0];
	float xy = quatNormalized[0] * quatNormalized[1];
	float xz = quatNormalized[0] * quatNormalized[2];
	float xw = quatNormalized[0] * quatNormalized[3];
	float yy = quatNormalized[1] * quatNormalized[1];
	float yz = quatNormalized[1] * quatNormalized[2];
	float yw = quatNormalized[1] * quatNormalized[3];
	float zz = quatNormalized[2] * quatNormalized[2];
	float zw = quatNormalized[2] * quatNormalized[3];

	Float4x4 result;
	result.makeIdentity();

	result.ma[0] = 1 - 2 * (yy + zz);
	result.ma[1] = 2 * (xy - zw);
	result.ma[2] = 2 * (xz + yw);
	result.ma[4] = 2 * (xy + zw);
	result.ma[5] = 1 - 2 * (xx + zz);
	result.ma[6] = 2 * (yz - xw);
	result.ma[8] = 2 * (xz - yw);
	result.ma[9] = 2 * (yz + xw);
	result.ma[10] = 1 - 2 * (xx + yy);
	result.ma[3] = result.ma[7] = result.ma[11] = result.ma[12] = result.ma[13] = result.ma[14] = 0;
	result.ma[15] = 1;

	// Combine our partial rotation with MatrixA
	Multiply(result, matA, result);

	// Linear interpolation of the translation components of the matrices
	result.m[3][0] = matA.m[3][0] + ((matB.m[3][0] - matA.m[3][0])*delta);
	result.m[3][1] = matA.m[3][1] + ((matB.m[3][1] - matA.m[3][1])*delta);
	result.m[3][2] = matA.m[3][2] + ((matB.m[3][2] - matA.m[3][2])*delta);

	return result;
}