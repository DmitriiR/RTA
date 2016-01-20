#pragma once
#include "RenderNode.h"
#include <DirectXMath.h>
#include "Renderer.h"

class RenderSet
{
	friend class Renderer;
public:
	RenderSet(){};
	~RenderSet()
	{
		//if (headPtr)
		//{
		//delete headPtr;

		//}
	};

	RenderSet *renderSetPtr;

	/// The first item in the render batch.
	RenderNode *headPtr;
	/// The last item in the render batch.
	RenderNode *tailPtr;

	inline RenderNode * GetHead() { return headPtr; }

	inline void RenderSet::AddRenderNode(RenderNode *nodePtr)
	{
		nodePtr->SetNext(headPtr);
		headPtr = nodePtr;
	}

	inline void RenderSet::ClearRenderSet(void)
	{

		headPtr = tailPtr = nullptr;
	}



};

