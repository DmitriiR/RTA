#pragma once
#include "Renderer.h"


namespace RendererD3D
{
	/// A singleton class used to hold the index buffers used for rendering indexed geometry.
	class IndexBuffer
{
private:
	IndexBuffer(void);
	IndexBuffer(const IndexBuffer &) {}
	IndexBuffer &operator=(const IndexBuffer &) { return *this; }

	/// the one and only instance.
	static IndexBuffer *instancePtr;

	/// The buffer used for all indices.
	ID3D11Buffer *indexBufferPtr;

public:
	~IndexBuffer(void);

	/// Gets a reference to the one and only IndexBuffer instance.
	static IndexBuffer &GetReference();

	/// Deletes the instance of the IndexBuffer.
	static void DeleteInstance();

	UINT AddIndices(const UINT *_indices, UINT numIndices);

	inline ID3D11Buffer * const GetIndices() { return indexBufferPtr; }

};
}

namespace RendererD3D
{
	IndexBuffer *IndexBuffer::instancePtr = 0;

	IndexBuffer::IndexBuffer(void)
	{
		indexBufferPtr = 0;
	}

	IndexBuffer::~IndexBuffer(void)
	{
		ReleaseCOM(indexBufferPtr);
	}

	IndexBuffer &IndexBuffer::GetReference()
	{
		if (!instancePtr)
		{
			instancePtr = new IndexBuffer;
		}
		return *instancePtr;
	}

	void IndexBuffer::DeleteInstance()
	{
		delete instancePtr;
	}

	UINT IndexBuffer::AddIndices(const UINT *_indices, UINT _numIndices)
	{
		UINT ret = 0;

		if (!indexBufferPtr)
		{
			D3D11_BUFFER_DESC newBufferDesc = { 0 };
			//newBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			newBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			newBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			newBufferDesc.ByteWidth = _numIndices * sizeof(UINT);
			newBufferDesc.CPUAccessFlags = 0; // ????? no access  
			newBufferDesc.MiscFlags = 0;
			newBufferDesc.StructureByteStride = 0;

			// getting the data in there 
			D3D11_SUBRESOURCE_DATA resource_data = { 0 };
			resource_data.pSysMem = (void*)_indices;
			resource_data.SysMemPitch = 0;
			resource_data.SysMemSlicePitch = 0;
			if (RendererD3D::Renderer::theDevicePtr)
			{
				RendererD3D::Renderer::theDevicePtr->CreateBuffer(&newBufferDesc, &resource_data, &indexBufferPtr);
			}
			return 0; // thats where the indecies live inside the buffer, they are at the start of the buffer 
		}
		else
		{
			D3D11_BUFFER_DESC newBufferDesc;
			indexBufferPtr->GetDesc(&newBufferDesc);

			ID3D11Buffer * newBuffer = nullptr;

			D3D11_SUBRESOURCE_DATA resource_data = { 0 };
			UINT oldWidth = newBufferDesc.ByteWidth;
			resource_data.pSysMem = (void*)(((char*)_indices) - oldWidth);
			newBufferDesc.ByteWidth += _numIndices * sizeof(UINT);

			resource_data.SysMemPitch = 0;
			resource_data.SysMemSlicePitch = 0;
			if (RendererD3D::Renderer::theDevicePtr)
			{
				RendererD3D::Renderer::theDevicePtr->CreateBuffer(&newBufferDesc, &resource_data, &newBuffer);
			}
			RendererD3D::Renderer::theContextPtr->CopySubresourceRegion(newBuffer, 0, 0, 0, 0, indexBufferPtr, 0, 0);
			ReleaseCOM(indexBufferPtr);
			indexBufferPtr = newBuffer;
			ret = oldWidth / sizeof(UINT);
		}


		return ret;
	}
}