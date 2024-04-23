#include"hzpch.h"
#include"Scene.h"
#include"Components.h"
#include"Hazel/Renderer/Renderer2D.h"
#include"Entity.h"
#include<glm/glm.hpp>
namespace Hazel {
    static void DoMaths(const glm::mat4 Transform) {

    }
    static void OnTransformConstruct(entt::registry& registry, entt::entity entity) {

    }
    Scene::Scene()
    {
#if 0
        entt::entity entity = m_Registry.create();//实体管理创建实体
        m_Registry.emplace< TransformComponent>(entity, glm::mat4(1.0f));//将组件与实体关联

        //当TransformComponent被构造时，链接函数作为回调函数
        m_Registry.on_construct< TransformComponent>().connect<&OnTransformConstruct>();

        //检查实体是否具有指定类型的组件
        if (m_Registry.has< TransformComponent>(entity)) {
            TransformComponent& transform = m_Registry.get< TransformComponent>(entity);
        }
        //所有具有指定类型组件的实体
        auto view = m_Registry.view< TransformComponent>();
        for (auto entity : view) {
            TransformComponent& transform = view.get< TransformComponent>(entity);
        }
        //所有具有组件（两个）的实体的组
        auto group = m_Registry.group< TransformComponent>(entt::get< MeshComponent>);
        for (auto entity : group) {
            auto&[Transform, mesh] = group.get< TransformComponent, MeshComponent>(entity);
            
        }
#endif
    }
    Scene::~Scene()
    {
    }
    Entity Scene::CreateEntity(const std::string& name)
    {
        Entity entity = { m_Registry.create(),this };//创建实体类
        entity.AddComponent< TransformComponent>();//为实体添加组件
        auto& tag = entity.AddComponent< TagComponent>();
        tag.Tag = name.empty() ? "Entity" : name;//传入组件的成员
        return entity;//返回实体类
    }
    void Scene::OnUpdate(Timestep ts)
    {
        //找到具有组件的实体
        Camera* mainCamera = nullptr;
        glm::mat4* cameraTransform = nullptr;

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();
            for (auto entity : view) {//赋值给左边变量
                auto& [transform, camera] = view.get< TransformComponent, CameraComponent>(entity);

                if (camera.Primary) {//唯一的主相机实体
                    mainCamera = &camera.Camera;
                    cameraTransform = &transform.Transform;
                    break;
                }
            }
        }
        //找到具有多个组件的组的实体
        if(mainCamera)
        {
            Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
            auto group = m_Registry.group< TransformComponent>(entt::get< SpriteRendererComponent>);//所有具有组件（两个）的实体的组
            for (auto entity : group) {
                auto& [transform, sprite] = group.get< TransformComponent, SpriteRendererComponent>(entity);
                Renderer2D::DrawQuad(transform, sprite.Color);
            }
            Renderer2D::EndScene();
        }
    }
    void Scene::OnViewportResize(uint32_t width, uint32_t height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
        auto view = m_Registry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio) {//非固定纵横比
                cameraComponent.Camera.SetViewportSize(width, height);
            }
        }
    } 
}