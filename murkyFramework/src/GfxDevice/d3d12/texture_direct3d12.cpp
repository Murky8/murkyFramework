//------------------------------------------------------------------------------
// 2015 J. Coelho.
// Platform: C++11
#include <murkyFramework/src/pch.hpp>

namespace murkyFramework {
#define deviceObj  g_appDebug->render->gfxDevice

    static u32 nextTextureID = { 0 };
    namespace GfxDevice
    {
        //https://github.com/Microsoft/DirectX-Graphics-Samples/blob/master/Samples/D3D1211On12/src/D3D1211On12.cpp
        TextureWrapper   createTextureObject(u8 * in_imageData, u32 width, u32 height)
        {
            TextureWrapper   texture;
            texture.iTexture = nextTextureID;
            ++nextTextureID;

            ComPtr<ID3D12Resource> textureUploadHeap;   // keep in scope until command list executed

            {//begin command list
                ThrowIfFailed(deviceObj->g_commandList->Reset(deviceObj->m_commandAllocator.Get(), deviceObj->m_pipelineState_pct.Get()));
            }//begin command list

            CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(deviceObj->m_srvHeap->GetCPUDescriptorHandleForHeapStart());
            srvHandle.Offset(texture.iTexture, deviceObj->m_srvDescriptorSize);

            u32 TexturePixelSize = 4;

            // Describe  Texture2D.
            D3D12_RESOURCE_DESC textureDesc = {};
            textureDesc.MipLevels = 1;
            textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            textureDesc.Width = width;
            textureDesc.Height = height;
            textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
            textureDesc.DepthOrArraySize = 1;
            textureDesc.SampleDesc.Count = 1;
            textureDesc.SampleDesc.Quality = 0;
            textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;

            ThrowIfFailed(deviceObj->m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
                D3D12_HEAP_FLAG_NONE,
                &textureDesc,
                D3D12_RESOURCE_STATE_COPY_DEST,
                nullptr,
                IID_PPV_ARGS(&deviceObj->m_texture[texture.iTexture])));

            const UINT64 uploadBufferSize = GetRequiredIntermediateSize(deviceObj->m_texture[texture.iTexture].Get(), 0, 1);

            // Create the GPU upload buffer.
            ThrowIfFailed(deviceObj->m_device->CreateCommittedResource(
                &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
                D3D12_HEAP_FLAG_NONE,
                &CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&textureUploadHeap)));

            // Copy data to the intermediate upload heap and then schedule a copy 
            // from the upload heap to the Texture2D.

            D3D12_SUBRESOURCE_DATA textureData = {};            
            textureData.pData       = in_imageData;
            textureData.RowPitch    = width * TexturePixelSize;
            textureData.SlicePitch  = textureData.RowPitch * height;

            UpdateSubresources(deviceObj->g_commandList.Get(), deviceObj->m_texture[texture.iTexture].Get(), textureUploadHeap.Get(), 0, 0, 1,
                &textureData);
            deviceObj->g_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(deviceObj->m_texture[texture.iTexture].Get(),
                D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

            // Describe and create a SRV for the texture.
            D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
            srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
            srvDesc.Format = textureDesc.Format;
            srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
            srvDesc.Texture2D.MipLevels = 1;

            deviceObj->m_device->CreateShaderResourceView(deviceObj->m_texture[texture.iTexture].Get(), &srvDesc, srvHandle);
            srvHandle.Offset(deviceObj->m_srvDescriptorSize);

            {//end command list
                ThrowIfFailed(deviceObj->g_commandList->Close());
                ID3D12CommandList* ppCommandLists[] = { deviceObj->g_commandList.Get() };
                deviceObj->m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

                deviceObj->waitForGPUFinish();
            }//end command list

            return texture;
        }

        void initilise_textureSystem()
        {
        }

        void deinitilise_textureSystem()
        {
        }    
    }//namespace GfxDevice
}//namespace murkyFramework