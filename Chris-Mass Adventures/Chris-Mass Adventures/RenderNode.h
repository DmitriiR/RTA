#pragma once

class RenderNode;


typedef void(*RenderFunc)(RenderNode &rNode);
class RenderNode
{
public:

	RenderNode * next = nullptr;
	RenderFunc func;

	RenderNode();
	~RenderNode();

	inline void renderProcess() { func(*this); };

private:

};

