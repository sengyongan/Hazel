#include "hzpch.h"
#include "OrthographicCameraController.h"
#include"Hazel/Core/Input.h"
#include"Hazel/Core/KeyCodes.h"
//m_Camera(- m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, - m_ZoomLevel, m_ZoomLevel),
namespace Hazel {//float left, float right, float bottom, float top������m_ZoomLevel���ţ���4���������ı䣬����16 / 9 = 1.78{-1.78��1.78��-1��1}
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
        : m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top),
        m_AspectRatio(aspectRatio),m_Rotation(rotation)
    {
    }
    void OrthographicCameraController::OnUpdate(Timestep ts)
    {
        HZ_PROFILE_FUNCTION();

        //HZ_CLIENT_TRACE("time:{0}s", ts.GetSeconds());��ȡʱ��
        //float ts = ts;��ʽǿ��ת������������m_Time
        if (Input::IsKeyPressed(HZ_KEY_A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(HZ_KEY_D))
            m_CameraPosition.x += m_CameraTranslationSpeed * ts;
        if (Input::IsKeyPressed(HZ_KEY_W))
            m_CameraPosition.y += m_CameraTranslationSpeed * ts;
        else if (Input::IsKeyPressed(HZ_KEY_S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * ts;

        if (m_Rotation) 
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * ts;
            if (Input::IsKeyPressed(HZ_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * ts;

            m_Camera.SetRotation(m_CameraRotation);
        }
        m_Camera.SetPosition(m_CameraPosition);
        //
        m_CameraTranslationSpeed = m_ZoomLevel;//m_ZoomLevelԽС������ԽС������Խ�󣬾���Խ�����ƶ�Խ��
    }
    void OrthographicCameraController::OnEvent(Event& e)
    {
        HZ_PROFILE_FUNCTION();

        EventDispatcher dispatcher(e);//<MouseScrolledEvent>��ʾģ��ʵ��
        dispatcher.Dispatch <MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch <WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }
    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        m_ZoomLevel -= e.GetYOffset() * 0.5f;//����������ƫ��Ϊ���ż���
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);//��СΪ0.25����ֹԽ��ͼƬ��
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
        return false;
    }
    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        HZ_PROFILE_FUNCTION();

        m_AspectRatio = (float)e.GetWidth() / (float) e.GetHeight();
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
        return false;
    }
}