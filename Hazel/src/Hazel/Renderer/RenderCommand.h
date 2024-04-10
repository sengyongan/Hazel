#pragma once
//RendererAPI��ת
//����s_RendererAPI����Ⱦ������
#include"RendererAPI.h"
namespace Hazel {
    class RenderCommand {
    public:
        inline static void Init() {

            s_RendererAPI->Init();
        }
        inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
            s_RendererAPI->SetViewport(x, y, width, height);
        }
        inline static void SetClearColor(const glm::vec4& color) {//��̬--ȫ����������
            s_RendererAPI->SetClearColor(color);
        }
        inline static void Clear() {
            s_RendererAPI->Clear();
        }

        inline static void DrawIndexed(const Ref<VertexArray>& vertexArray,uint32_t count = 0) {
            s_RendererAPI->DrawIndexed(vertexArray,count);//����OpenGLRendererAPI��DrawIndexed
        }
    private:
        static RendererAPI* s_RendererAPI;
    };
}