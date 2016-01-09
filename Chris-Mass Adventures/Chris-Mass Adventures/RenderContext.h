#pragma once
#include "RenderNode.h"
#include "RenderMaterial.h"
#include "RenderSet.h"
#include "Renderer.h"
#include "IndexBuffer.h"

#include <list>

using namespace RendererD3D;

class RenderContext : RenderNode
{
private:
	RenderSet *renderSetPtr;
	ID3D11PixelShader * PixelShaderPtr;
	std::list<RenderMaterial> materials;

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


	inline void RenderContext::ContextPosNormalUVRenderFunc(RenderNode &node)
	{

		RenderContext & contPtr = (RenderContext &)node;
		
		RendererD3D::Renderer::theContextPtr->IASetIndexBuffer(IndexBuffer::GetReference().GetIndices(), DXGI_FORMAT_R32_UINT, 0);
		
		//RendererD3D::Renderer::theContextPtr->VSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetVertexShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->PSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetPixelShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->GSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetGeometryShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->DSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetDomainShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->HSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetHullShader(), NULL, NULL);

		RendererD3D::Renderer::Render(contPtr.GetRenderSet());
	}

	inline void RenderContext::ContextTransparentPosNormalUVRenderFunc(RenderNode &node)
	{

		RenderContext & contPtr = (RenderContext &)node;

		RendererD3D::Renderer::theContextPtr->IASetIndexBuffer(IndexBuffer::GetReference().GetIndices(), DXGI_FORMAT_R32_UINT, 0);

		//RendererD3D::Renderer::theContextPtr->VSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetVertexShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->PSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetPixelShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->GSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetGeometryShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->DSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetDomainShader(), NULL, NULL);
		//RendererD3D::Renderer::theContextPtr->HSSetShader(contPtr.GetRenderStageTechnique()->GetPass(0)->GetHullShader(), NULL, NULL);

		RendererD3D::Renderer::Render(contPtr.GetRenderSet());
	}


};


// The job of the RenderContext is to put the rendering system into the correct state for rendering specific objects.
// This will consist of things like setting the vertex stream, turing on shaders, as well as changing engine states like the blend mode.
// Changing these states, can be very costly as they must use the motherboard bus to communicate between the CPU and GPU and these changes may be stalled, or cause stalls, depending on what the GPU is doing when a context change is applied.
// We will sort objects based on RenderContext to reduce how often we have to make these changes per - frame.
