#pragma once

class RenderNode;

typedef void(*RenderFunc)(RenderNode &curNode);

class RenderNode
{
protected: RenderNode * next;
public:
	RenderNode::RenderNode(){ next = nullptr; }

	RenderFunc Renderfunc;

	void RenderProcess(){ Renderfunc(*this); }

	RenderNode *GetNext(void) { return next; }
	void SetNext(RenderNode* _next){ next = _next; }

};