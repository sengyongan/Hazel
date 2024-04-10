#pragma once
//着色器opengl派生类
#include"Hazel/Renderer/Shader.h"
//#include<glad/glad.h>  Cannot open incude file:'glad/gladhs 
#include<glm/glm.hpp>
//TODO
typedef unsigned int GLenum;
namespace Hazel {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(const std::string& filepath);
        OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
        virtual ~OpenGLShader();
        virtual void Bind() const override;
        virtual void Unbind() const override;
        //调用下面的UploadUniformInt等
        virtual void SetInt(const std::string& name, int value) override;
        virtual void SetIntArray(const std::string& name, int* values,uint32_t count) override;
        virtual void SetFloat(const std::string& name, float value) override;
        virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
        virtual void SetFloat4(const std::string& name, const glm::vec4& value) override;
        virtual void SetMat4(const std::string& name, const glm::mat4& value) override;


        virtual const std::string& GetName() const override { return m_Name; }

        //name着色器中的全局常量unifrom名，value/matrix传递的值
        void UploadUniformInt(const std::string& name, int value);
        void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);

        void UploadUniformFloat(const std::string& name, float value);
        void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
        void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
        void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

        void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
        void UploadUniformMat4(const std::string& name, const glm::mat4& matrix); //将视图投影矩阵传递给顶点着色器u_ViewProjection
    private:
        std::string ReadFile(const std::string& filepath);//读取文件
        std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);//将字符串源码分隔开
        void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);//gl底层源码--for循环unordered_map
    private:
        uint32_t m_RendererID;//着色器程序
        std::string m_Name;//传入的文件名  / 直接传入的实参， 在OpenGLShader构造函数中被初始化
    };
}