#include "hzpch.h"
#include "WindowsWindow.h"
#include"Hazel/core.h"
#include"Hazel/Events/ApplicationEvent.h"
#include"Hazel/Events/KeyEvent.h"
#include"Hazel/Events/MouseEvent.h"

#include<glad/glad.h>
namespace Hazel {
    //bool�Ƿ��ʼ��
	static bool s_GLFWInitiallized = false;
    //
    static void GLFWErrorCallback(int error, const char* description)
    {
        HZ_CORE_ERROR("GLFW  Error  ({0}) : {1}", error, description);
    }

    //����ʵ������
    //window�о�̬����ʵ��
// ���ĵ��á�������󡪡���ʼ�������������ݣ���������GLFW��ʵ�����ݡ�����������
	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);//��������󣺰���WindowsWindow���к���
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{

		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
        Shutdown();
	}
    //����ʵ������
    void WindowsWindow::Init(const WindowProps& props)
    {
        //Ϊʵ�����ݸ�ֵ=�ɱ���������
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        //��־�еĺ�
        HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);
        //�ж��Ƿ��ʼ��
        if (s_GLFWInitiallized == false)
        {
            int success = glfwInit();//��glfw������ʱ
            //���successû�гɹ����������ر�ϵͳ��

            HZ_CORE_ASSERT(success, "Could not initialize GLFW!");//������޷���ʼ�� GLFW��
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitiallized = true;

        }
        //����=ʵ�ʴ���
        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
        glfwMakeContextCurrent(m_Window);//��ǰ������
        int stauts = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);//��ȡ�����ַ
        HZ_CORE_ASSERT(stauts, "Failed to initialize glad");
        //GLFW�еĺ���
        glfwSetWindowUserPointer(m_Window, &m_Data);//���û��Զ���������봰�ڹ�������
        SetVSync(true);

        // GLFW callback
        //���ô��ڴ�С�ص�����
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int  height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowResizeEvent  event(width, height);//ApplicationEvent�д��ڵ�����С�¼�
                data.Width = width;
                data.Height = height;
                data.EventCallback(event);

            });//�������ǲ����ͺ�����
        //
        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent event;
                data.EventCallback(event);

            });
        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                         case GLFW_PRESS://����
                        {  
                            KeyPressedEvent event(key, 0);
                            data.EventCallback(event);
                        
                            break;
                        }
                        case GLFW_RELEASE://����
                        {
                            KeyReleasedEvent event(key);
                            data.EventCallback(event);
                            break;
                        }
                        case GLFW_REPEAT://�ظ�
                        {
                            KeyPressedEvent event(key,1);
                            data.EventCallback(event);
                            break;
                        }
               
                }
            });
        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)//�����ַ���
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                KeyTypedEvent event(keycode);
                data.EventCallback(event);

            });
        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                switch (action)
                {
                        case GLFW_PRESS:
                        {
                            MouseButtonPressedEvent event(button);
                            data.EventCallback(event);
                            break;

                        }
                        case GLFW_RELEASE:
                        {
                            MouseButtonReleasedEvent event(button);
                            data.EventCallback(event);
                            break;

                        }
                }

            });
        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseScrolledEvent event((float)xoffset, (float)yoffset);
                data.EventCallback(event);

            });
        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                MouseMovedEvent event((float)xpos, (float)ypos);
                data.EventCallback(event);

            });
    }

	void WindowsWindow::Shutdown()
	{

		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()//����
	{

		glfwPollEvents();
        glfwSwapBuffers(m_Window);//����������
	}
    //֡��
	void WindowsWindow::SetVSync(bool enabled)
	{

		if (enabled)
			glfwSwapInterval(1);//��ֱͬ��
		else
			glfwSwapInterval(0);//����

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;//����VSync boolֵ
	}

}