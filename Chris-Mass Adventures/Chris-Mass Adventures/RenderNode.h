#pragma once

class RenderNode;

typedef void(*RenderFunc)(RenderNode &curNode);

class RenderNode
{
protected: RenderNode * next;
public:
	RenderNode::RenderNode(){ next = nullptr; }

	RenderFunc func;

	void Process(){ func(*this); }

	RenderNode *GetNext(void) { return next; }
	void SetNext(RenderNode* _next){ next = _next; }

};