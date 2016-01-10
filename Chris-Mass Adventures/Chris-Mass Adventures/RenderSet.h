#pragma once
#include "RenderNode.h"
#include <DirectXMath.h>
#include "Renderer.h"

class RenderSet
{
	friend class Renderer;
public:
	RenderSet();
	~RenderSet()
	{
		delete head;
	};

	RenderNode* head;

	inline RenderNode * GetHead() { return head; }

	inline void RenderSet::AddRenderNode(RenderNode *nodePtr)
	{
		nodePtr->SetNext(head);
		head = nodePtr;
	}



private:

};

