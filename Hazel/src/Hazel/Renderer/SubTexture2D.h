#pragma once
//�����в�������
#include"Texture.h"
#include<glm/glm.hpp>

namespace Hazel {
    class SubTexture2D {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);//����xy������xy���꣬

        const Ref<Texture2D> GetTexture() const { return m_Texture; }
        const glm::vec2* GetTexCoords() const { return m_TexCoords; }
        //�������������������꣬����������С����������
        static Ref<SubTexture2D> CreatFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellsize,const glm::vec2& spriteSize = { 1,1 });
    private:
        Ref<Texture2D> m_Texture;
        glm::vec2 m_TexCoords[4];//��������
    };
}