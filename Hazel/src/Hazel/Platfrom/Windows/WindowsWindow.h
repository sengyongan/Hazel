#pragma once
#include "Hazel/Window.h"//继承
#include "Hazel/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>
//windows操作系统的窗口
namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
        //构造函数：接受窗口数据
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();
        //更新--调用再application
		void OnUpdate() override;
        //返回窗口数据
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }//设置窗口实际数据=事件
		void SetVSync(bool enabled) override;//限制游戏帧率
        //override显式地指示派生类中的函数是覆盖基类中的虚函数
		bool IsVSync() const override;
        //获取本地窗口
		inline virtual void* GetNativeWindow() const { return m_Window; }

//——————————————————————————————————————————————————————————————————————————————————
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();//关闭
	private:
		GLFWwindow* m_Window;//GLFW的函数
        //
        GraphicsContext* m_Context;//当前上下文
		//Scope<GraphicsContext> m_Context;
//——————————————————————————————————————————————————————————————————————————————————
        //窗口实际数据会传递给GLFW，由库创建窗口
        //WindowData定义
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;//结构对象
	};

}


