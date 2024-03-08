workspace"Hazel"
architecture"x64"
configurations--���õ���
{
	"Debug",
	"Release",
	"Dist"
}	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--����Ŀ¼���������Ŀ¼����Ե�Ŀ¼
IncludeDir = {}
IncludeDir["GLFW"] = "Hazel/vendor/GLFW/include"--IncludeDir����
IncludeDir["Glad"] = "Hazel/vendor/Glad/include"
IncludeDir["ImGui"] = "Hazel/vendor/imgui"

include "Hazel/vendor/GLFW"
include "Hazel/vendor/Glad"
include "Hazel/vendor/imgui"

project "Hazel"
	location"Hazel"
	kind"SharedLib"--��̬��
	language"C++"
	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
    --��ÿ���ļ��Զ�����pchheader��ָ��Դ�ļ�
	pchheader "hzpch.h"
	pchsource "Hazel/src/hzpch.cpp"
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs--����Ŀ¼
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}
	links
	{
		"GLFW",
		"Glad",
	"ImGui",
		"opengl32.lib"
	}

	filter"system:windows"
	cppdialect"C++17"
	staticruntime"On"
	systemversion"latest"
	defines
	{
		"HZ_PLATFROM_WINDOWS",
		"HZ_BUILD_DLL",
       -- "HZ_ENABLE_ASSERTS"
       "GLFW_INCLUDE_NONE",
       "HZ_BIND_EVENT_FN"
	}
	postbuildcommands
	{
		("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
	}
	filter"configurations:Debug"
	defines "HZ_DEBUG"
    buildoptions "/MDd"--���̵߳���dll
	symbols"On"

	filter"configurations:Release"
	defines "HZ_RELEASE"
    buildoptions "/MD"
	optimize"On"

	filter"configurations:Dist"
	defines "HZ_DIST"
    buildoptions "/MD"
	optimize"On"

project "Sandbox"
	location"Sandbox"
	kind"ConsoleApp"
	language"C++"
	targetdir("bin/"..outputdir.."/%{prj.name}")
	objdir("bin-int/"..outputdir.."/%{prj.name}")
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}
	includedirs
	{
		"Hazel/vendor/spdlog/include",
		"Hazel/src"
	}

	filter"system:windows"
	cppdialect"C++17"
	staticruntime"On"
	systemversion"latest"
	defines
	{
		"HZ_PLATFROM_WINDOWS"
	}
	links
	{
		"Hazel"
	}
	filter"configurations:Debug"
	defines "HZ_DEBUG"
        buildoptions "/MDd"
	symbols"On"

	filter"configurations:Release"
	defines "HZ_RELEASE"
        buildoptions "/MD"
	optimize"On"

	filter"configurations:Dist"
	defines "HZ_DIST"
        buildoptions "/MD"
	optimize"On"