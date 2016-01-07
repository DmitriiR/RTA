#pragma once
#include "RenderNode.h"
#include "RenderMaterial.h"
#include <list>

class RenderContext : RenderNode
{
public:
	RenderContext();
	~RenderContext();

	// apply render states and shaders
	////////////////////////////////////////////////////////////////////////////
	// RenderFuncs
	////////////////////////////////////////////////////////////////////////////
	static void renderContextFunc(RenderNode &);

private:

};