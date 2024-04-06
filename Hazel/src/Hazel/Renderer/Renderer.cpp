#include"hzpch.h"
#include "Renderer.h"
#include "Hazel/Platfrom/Opengl/OpenGLShader.h"
#include"Renderer2D.h"
namespace Hazel {
    Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;
    void Renderer::Init()
    {
        HZ_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }
    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {
        RenderCommand::SetViewport(0, 0, width, height);
    }
    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_SceneData->ViewProjectionMatrix = camera.GetViewProjextionMatrix();//��ΪViewProjextionMatrix
    }
    void Renderer::EndScene()
    {
    }
    void Renderer::Submit(const Ref<Shader>& shader,const Ref<VertexArray>& vertexArray, const glm::mat4& transfrom)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transfrom", transfrom);

        //mi->Bind();

        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
}