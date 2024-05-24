#pragma once
//管理整个场景的 实体/ 组件 的 创建/销毁/更新
#include "entt.hpp"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Renderer/EditorCamera.h"
#include "Hazel/Core/UUID.h"

class b2World;

namespace Hazel {

    class Entity;

    class Scene
    {
    public:
        Scene();
        ~Scene();

        static Ref<Scene> Copy(Ref<Scene> other);//拷贝场景

        Entity CreateEntity(const std::string& name = std::string());
        Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
        void DestroyEntity(Entity entity);

        void OnRuntimeStart();//创建销毁2d刚体
        void OnRuntimeStop();

        void OnUpdateEditor(Timestep ts,EditorCamera& camera);
        void OnUpdateRuntime(Timestep ts);
        void OnViewportResize(uint32_t width, uint32_t height);

        void DuplicateEntity(Entity entity);//拷贝实体

        Entity GetPrimaryCameraEntity();//找到主摄像机
    private:
        template<typename T>
        void OnComponentAdded(Entity entity, T& component);
    private:
        entt::registry m_Registry;
        uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;

        b2World* m_PhysicsWorld = nullptr;//物理世界

        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;
    };

}