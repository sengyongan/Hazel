#pragma once
//GameLayer�㡪������2d�����С��
#include "Hazel.h"

#include "Level.h"
#include <imgui/imgui.h>

class GameLayer : public Hazel::Layer
{
public:
    GameLayer();
    virtual ~GameLayer() = default;

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Hazel::Timestep ts) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;

    bool OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e);
    bool OnWindowResize(Hazel::WindowResizeEvent& e);
private:
    void CreateCamera(uint32_t width, uint32_t height);
private:
    Hazel::Scope<Hazel::OrthographicCamera> m_Camera;
    Level m_Level;//�ؿ�
    ImFont* m_Font;//����������Ⱦ
    float m_Time = 0.0f;//��ʾʱ��
    bool m_Blink = false;//��˸״̬

    enum class GameState//��Ϸ״̬
    {
        Play = 0, MainMenu = 1, GameOver = 2
    };

    GameState m_State = GameState::MainMenu;
};
