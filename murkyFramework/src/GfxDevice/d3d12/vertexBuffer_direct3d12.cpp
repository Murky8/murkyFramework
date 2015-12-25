//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>
#define deviceObj  g_appDebug->render->gfxDevice

namespace murkyFramework {
    namespace GfxDevice
    {
        using namespace DirectX;
        using namespace Microsoft::WRL;

        // constructors	
        VertexBufferWrapper::VertexBufferWrapper(
            VertexType vertexType, PrimativeType primativeType,
            ShaderWrapper shaderId, TextureWrapper texture,
            u32 nVerts) :
            vertexType(vertexType), primativeType(primativeType),
            shaderId(shaderId), texture(texture),
            capacity(nVerts)
        {
            HRESULT hr;
            u32 sizeVertex = getVertexSizeInBytes(vertexType);

            int sizeBytes = capacity*sizeVertex;
            hr = deviceObj->m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(sizeBytes),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_vertexBuffer));

            if (FAILED(hr))
                triggerBreakpoint();

            // Initialize the vertex buffer view.
            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.StrideInBytes = sizeVertex;
            m_vertexBufferView.SizeInBytes = sizeBytes;
        }
        
        // methods
        s32 VertexBufferWrapper::getCapacityBytes() const
        {
            s32 s = -1;
            switch (this->vertexType)
            {
            case  VertexType::posColTex:  s = capacity*sizeof(Triangle_pct);
                break;
            default:
                triggerBreakpoint();
            }
            return s;
        }

        void	VertexBufferWrapper::draw(void *vertexData, u32 nPrimatives)
        {
            if (nPrimatives >= capacity)
                triggerBreakpoint();

            u32 sizeVertex = getVertexSizeInBytes(vertexType);
            u32 nVerticiesPerPrimative;
            getPrimativeInfo(primativeType, &nVerticiesPerPrimative);

            D3D_PRIMITIVE_TOPOLOGY primTop;
            switch (primativeType)
            {
            case PrimativeType::triangle:
                primTop = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                break;
            case PrimativeType::line:
                primTop = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                break;
            default:// Catch usage of unimplemented			            
                triggerBreakpoint();
            }
            HRESULT hr;
            //const UINT vertexBufferSize = nPrimatives*sizeof(Vert_pct);		
            static UINT8* pVertexDataBegin;// todo ?
            hr = m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
            ThrowIfFailed(hr);
            memcpy(pVertexDataBegin, vertexData, nPrimatives*nVerticiesPerPrimative*sizeVertex);
            m_vertexBuffer->Unmap(0, nullptr);   // Todo not needed!!!

            // set current texture
            CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandle(deviceObj->m_srvHeap->GetGPUDescriptorHandleForHeapStart());
            srvGPUHandle.Offset(this->texture.iTexture, deviceObj->m_srvDescriptorSize);
            deviceObj->g_commandList->SetGraphicsRootDescriptorTable(RootParameterTexture, srvGPUHandle);
            deviceObj->g_commandList->IASetPrimitiveTopology(primTop);

            switch (this->vertexType)
            {
            case VertexType::posColTex:
                deviceObj->g_commandList->SetPipelineState(deviceObj->m_pipelineState_pct.Get());
                break;

            case VertexType::posCol:
                deviceObj->g_commandList->SetPipelineState(deviceObj->m_pipelineState_pc.Get());
                break;
            }

            deviceObj->g_commandList->IASetVertexBuffers(0, 1, &(m_vertexBufferView));

            deviceObj->g_commandList->DrawInstanced(nVerticiesPerPrimative*nPrimatives, 1, 0, 0);
        }

         VertexIndexBufferWrapper::VertexIndexBufferWrapper(
            VertexType vertexType, PrimativeType primativeType,
            ShaderWrapper shaderId, TextureWrapper texture,
            u32 nVerts, u32 nIndices) :
            vertexType(vertexType), primativeType(primativeType),
            shaderId(shaderId), texture(texture),
            vertexBufElementCapacity(nVerts),
            indexBufElementCapacity(nIndices)
        {
            HRESULT hr;
            u32 sizeVertex = getVertexSizeInBytes(vertexType);

            int sizeVertexBufBytes = vertexBufElementCapacity*sizeVertex;
            hr = deviceObj->m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(sizeVertexBufBytes),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_vertexBuffer));
            if (FAILED(hr))
                triggerBreakpoint();

            // Initialize the vertex buffer view.
            m_vertexBufferView.BufferLocation = m_vertexBuffer->GetGPUVirtualAddress();
            m_vertexBufferView.StrideInBytes = sizeVertex;
            m_vertexBufferView.SizeInBytes = sizeVertexBufBytes;

            int sizeIndexBufBytes = indexBufElementCapacity*sizeof(u16);
            hr = deviceObj->m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(sizeIndexBufBytes),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&m_indexBuffer));
            if (FAILED(hr))
                triggerBreakpoint();

            // Initialize the vertex buffer view.
            m_indexBufferView.BufferLocation = m_indexBuffer->GetGPUVirtualAddress();
            m_indexBufferView.Format = DXGI_FORMAT_R16_UINT;
            m_indexBufferView.SizeInBytes = sizeIndexBufBytes;
        }

        void	VertexIndexBufferWrapper::draw(void *vertices, u16 nVertices, u16 *indices, u32 nPrimatives)        
        {
            u32 sizeVertex = getVertexSizeInBytes(vertexType);
            u32 nVerticiesPerPrimative;
            getPrimativeInfo(primativeType, &nVerticiesPerPrimative);

            if (nPrimatives >= vertexBufElementCapacity*nVerticiesPerPrimative)
                triggerBreakpoint();

            D3D_PRIMITIVE_TOPOLOGY primTop;
            switch (primativeType)
            {
            case PrimativeType::triangle:
                primTop = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
                break;
            case PrimativeType::line:
                primTop = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
                break;
            default:// Catch usage of unimplemented			            
                triggerBreakpoint();
            }
            HRESULT hr;

            // vertex buf
            static UINT8* pVertexDataBegin;
            hr = m_vertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pVertexDataBegin));
            ThrowIfFailed(hr);
            memcpy(pVertexDataBegin, vertices, nVertices*sizeVertex);
            m_vertexBuffer->Unmap(0, nullptr);
            // vertex buf ^
                                       
            // index buf
            static UINT8* pIndexDataBegin;
            hr = m_indexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&pIndexDataBegin));
            ThrowIfFailed(hr);
            memcpy(pIndexDataBegin, indices, nPrimatives*nVerticiesPerPrimative*sizeof(u16));
            m_indexBuffer->Unmap(0, nullptr);
            // index buf ^

            // set current texture
            CD3DX12_GPU_DESCRIPTOR_HANDLE srvGPUHandle(deviceObj->m_srvHeap->GetGPUDescriptorHandleForHeapStart());
            srvGPUHandle.Offset(this->texture.iTexture, deviceObj->m_srvDescriptorSize);
            deviceObj->g_commandList->SetGraphicsRootDescriptorTable(RootParameterTexture, srvGPUHandle);
            deviceObj->g_commandList->IASetPrimitiveTopology(primTop);

            switch (this->vertexType)
            {
            case VertexType::posColTex:
                deviceObj->g_commandList->SetPipelineState(deviceObj->m_pipelineState_pct.Get());
                break;

            case VertexType::posCol:
                deviceObj->g_commandList->SetPipelineState(deviceObj->m_pipelineState_pc.Get());
                break;
            }

            deviceObj->g_commandList->IASetVertexBuffers(0, 1, &(m_vertexBufferView));
            deviceObj->g_commandList->IASetIndexBuffer(&(m_indexBufferView));
            deviceObj->g_commandList->DrawIndexedInstanced(
                nVerticiesPerPrimative*nPrimatives, 1, 0, 0, 0 );
        }

    }//namespace GfxDevice
}//namespace murkyFramework