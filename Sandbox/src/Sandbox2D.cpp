#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>
Sandbox2D::Sandbox2D()
    : Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}
void Sandbox2D::OnAttach()
{
    HZ_PROFILE_FUNCTION();

    m_Texture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");

}

void Sandbox2D::OnDetach()
{
    HZ_PROFILE_FUNCTION();

}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{

    HZ_PROFILE_FUNCTION("Sandbox2D::OnUpdate");

    //camera
    m_CameraController.OnUpdate(ts);

    // Render
    Hazel::Renderer2D::ResetStats();
    {
        HZ_PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Hazel::RenderCommand::Clear();
    }

    {
        static float rotation = 0.0f;
        rotation += ts * 50.0f;

        HZ_PROFILE_SCOPE("Renderer Draw");
        Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Hazel::Renderer2D::DrawRotatedQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, glm::radians(rotation), { 0.8f,0.2f,0.3f,1.0f });
        Hazel::Renderer2D::DrawQuad({ -1.0f,0.0f }, { 1.0f,1.0f }, { 0.8f,0.2f,0.3f,1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.5f,0.5f }, { 0.5f,0.5f }, { 0.2f,0.2f,0.8f,1.0f });
        Hazel::Renderer2D::DrawQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, m_Texture,10.0f);
        Hazel::Renderer2D::DrawRotatedQuad({ 0.0f,0.0f,-0.1f }, { 10.0f,10.0f }, glm::radians(0.0f), m_Texture, 20.0f);

        for (float y = -0.5f; y < 5.0f; y += 0.5f) {
            for (float x = -0.5f; x < 5.0f; x += 0.5f) {
                glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f,0.5f };//0.5��������1.0֮�����ɫ
                Hazel::Renderer2D::DrawQuad({ x, y }, { 0.45f,0.45f }, color);
            }
        }
        Hazel::Renderer2D::EndScene();
    }

}

void Sandbox2D::OnImGuiRender()
{
    //////////////////////////////////////////////////////////////////////////////////
    static bool dockspaceOpen = true;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // DockSpace
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            // Disabling fullscreen would allow the window to be moved to the front of other windows,
            // which we can't undo at the moment without finer window depth/z control.
            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
            ImGui::MenuItem("Padding", NULL, &opt_padding);
            ImGui::Separator();

            if (ImGui::MenuItem("Exit"))Hazel::Application::Get().Close();
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
    //////////////////////////////////////////////////////////////////////////////////////
    HZ_PROFILE_FUNCTION();
    ImGui::Begin("Setting");

    auto statis = Hazel::Renderer2D::GetStats();
    ImGui::Text("Renderer2D statis");
    ImGui::Text("Draw Calls %d", statis.DrawCalls);
    ImGui::Text("Quads Calls %d", statis.QuadCount);
    ImGui::Text("Vertices Calls %d", statis.GetTotalVertexCount());
    ImGui::Text("Indies Calls %d", statis.GetTotalIndexCount());

    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

    uint32_t textureID = m_Texture->GetRendererID();
    ImGui::Image((void*)textureID, ImVec2{ 256.0f, 256.0f });

    ImGui::End();



}

void Sandbox2D::OnEvent(Hazel::Event& event)
{
    m_CameraController.OnEvent(event);

}
