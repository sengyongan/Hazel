#include"hzpch.h"
#include"Buffer.h"
#include"Renderer.h"
#include"Hazel/Platfrom/Opengl/OpenGLBuffer.h"
namespace Hazel {
    Ref<VertexBuffer>  VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);//调用类
        } 
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);//调用类
        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {

        switch (Renderer::GetAPI()) {
        case RendererAPI::API::None: HZ_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");return nullptr;
        case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
        }
        HZ_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}