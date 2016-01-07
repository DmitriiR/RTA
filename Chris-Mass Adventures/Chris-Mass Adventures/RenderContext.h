#pragma once
#include "RenderNode.h"
#include "RenderMaterial.h"
#include "RenderSet.h"
#include <list>

class RenderContext : RenderNode
{
public:
	RenderContext();
	~RenderContext()
	{
		delete renderSetPtr;
	};

	// apply render states and shaders
	////////////////////////////////////////////////////////////////////////////
	// RenderFuncs
	////////////////////////////////////////////////////////////////////////////
	static void renderContextFunc(RenderNode &);

	inline void CreateRenderSet(bool bSorted = false, bool(*SortFunc)(RenderNode *left, RenderNode *right) = 0)
	{
		renderSetPtr = new RenderSet;
	}

	inline void AddRenderNode(RenderNode *nodePtr)
	{
		renderSetPtr->AddRenderNode(nodePtr); // breKING HERE !
	}

	RenderContext* RenderContext::LoadObjects()
	{
		RenderContext* pRenderContext = new RenderContext;
		pRenderContext->CreateRenderSet();
		return pRenderContext;
	}

	

	inline RenderSet &GetRenderSet(void) { return *renderSetPtr; }

	inline void RenderContext::ContextSharedRenderFunc(RenderNode &node)
	{

		RenderContext & contPtr = (RenderContext &)node;

		//Renderer::theContextPtr->IASetIndexBuffer(IndexBuffer::GetReference().GetIndices(), DXGI_FORMAT_R32_UINT, 0);
		//Renderer::theContextPtr->VSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetVertexShader(), NULL, NULL);
		//Renderer::theContextPtr->PSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetPixelShader(), NULL, NULL);
		//Renderer::theContextPtr->GSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetGeometryShader(), NULL, NULL);
		//Renderer::theContextPtr->DSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetDomainShader(), NULL, NULL);
		//Renderer::theContextPtr->HSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetHullShader(), NULL, NULL);

		//Renderer::Render(contPtr.GetRenderSet());
		contPtr.GetRenderSet();
		// use default as buffer descriotion
	}

private:
	RenderSet *renderSetPtr;
};