﻿//绘制部分面板
#include"hzpch.h"
#include "SceneHierarchyPanel.h"
#include"Hazel/Scene/Components.h"
#include "Hazel/Scripting/ScriptEngine.h"

#include<imgui/imgui.h>
#include<imgui/imgui_internal.h>

#include<glm/gtc/type_ptr.hpp>

#include <cstring>

/* The Microsoft C++ compiler is non-compliant with the C++ standard and needs
 * the following definition to disable a security warning on std::strncpy().
 */
#ifdef _MSVC_LANG
#define _CRT_SECURE_NO_WARNINGS
#endif

namespace Hazel {
    extern const std::filesystem::path g_AssetPath;//asset路径

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
    {
        SetContext(context);
    }
    void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
    {
        m_Context = context;
        m_SelectionContext = {};//重设上下文（场景指针）
    }
    void SceneHierarchyPanel::OnImGuiRender()
    {   
        //Hierarchy层级面板////////////////////////////////////////////////////////////////////////
        ImGui::Begin("Scene Hierarchy");
        if (m_Context)
        {
            m_Context->m_Registry.each(
                [&](auto entityID) {
                    Entity entity{ entityID, m_Context.get() };//m_Context.get()返回场景指针
                    DrawEntityNode(entity);
                }
            );
            if (ImGui::IsMouseClicked(0) && ImGui::IsWindowHovered()) {//点击空白，
                m_SelectionContext = {};//没有选中实体
            }
            //创建实体菜单
            if (ImGui::BeginPopupContextWindow(0, 1, false)) {//0是ID 1是右键
                if (ImGui::MenuItem("create Empty Entity")) {
                    m_Context->CreateEntity("Empty Entity");
                }
                ImGui::EndPopup();//当前帧结束块后，下一帧重新创建
            }
        }

        ImGui::End();
        //属性面板//////////////////////////////////////////////////////////////////////////////////
        ImGui::Begin("Properties");
        if (m_SelectionContext) {
            DrawComponents(m_SelectionContext);//建立被选中节点的属性面板
        }
        ImGui::End();
    }
    void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
    {
        m_SelectionContext = entity;
    }
    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
        auto& tag = entity.GetComponent< TagComponent>().Tag;
        //typedef int 标志，==的这个节点有了ImGuiTreeNodeFlags_Selected标志
        ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;//节点被选中 | 用户点击箭头时，该节点将展开或折叠
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;//可以在非按钮同一行进行选择
        
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity,flags,tag.c_str());//该节点创建四叉树

        if (ImGui::IsItemClicked()) {//当点击哪个节点，表示选中某节点，m_SelectionContex更新为该节点

            m_SelectionContext = entity;
        }
        //删除实体菜单
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem()) {//有默认初始值
            if (ImGui::MenuItem("Delete Empty Entity")) {
                entityDeleted = true;
            }
            ImGui::EndPopup();
        }

        if (opened) {//该节点被打开
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;//节点被选中 | 用户点击箭头时，该节点将展开或折叠
            bool opened = ImGui::TreeNodeEx((void*)1, flags, tag.c_str());//创建子节点
            if (opened) {//已经打开
                ImGui::TreePop();//关闭子节点（代码折叠，不绘制节点）

            }
            ImGui::TreePop();//关闭父节点
        }
        if (entityDeleted) {
            m_Context->DestroyEntity(entity);
            if (m_SelectionContext == entity)m_SelectionContext = {};//正好选中当前被删除的节点，将选择变为空
        }
    }
    //标签，实际值，重置值，标签宽度
    static void DrawVec3Control(const std::string& lable, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f) {
        //默认字体
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];
        //
        ImGui::PushID(lable.c_str());//用户与 UI 交互时能够正确地处理和更新相应的数据

        ImGui::Columns(2);//第一列标签，第二列3个控件
        ImGui::SetColumnWidth(0,columnWidth);
        ImGui::Text(lable.c_str());
        ImGui::NextColumn();
        
        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));//缩进间距

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;//行高度
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };
        //////////////
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        //按钮
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);
        //控件
        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");//speed,min,max,格式
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ////////////
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });

        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ///////////
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });

        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);
        ImGui::PopFont();

        ImGui::SameLine();// 返回上一行，继续横向布局
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();

        ImGui::Columns(1);
        ImGui::PopID();
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////
    //绘制移除组件，组件名，实体，绘制组件函数（在DrawComponents被调用）
    template <typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction) {
        //标志
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | 
            ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
        //
        if (entity.HasComponent<T>()) {
            //
            auto& component  = entity.GetComponent< T>();
            //
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
            float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
            ImGui::Separator();
            bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
            ImGui::PopStyleVar();
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
            //移除组件按钮(应位于渲染imgui之前）
            if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight })) {
                ImGui::OpenPopup("ComponentSettings");
            }
            //
            bool removeComponenet = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Removecomponent")) {
                    removeComponenet = true;
                }
                ImGui::EndPopup();
            }

            //标识符，标志，标签
            //添加ImGuiTreeNodeFlags_DefaultOpen标志（默认被打开），节点名字为Transform
            if (open) {
                uiFunction(component);//调用自定义函数，并传入组件
                ImGui::TreePop();//结束当前节点的绘制。
            }
            //
            if (removeComponenet) {
                entity.RemoveComponent< T>();
            }
        }
    }
    /////////////////////DrawComponents//////////////////////////////////////
    /////////////////////////////////////////
    void SceneHierarchyPanel::DrawComponents(Entity entity)//被选中节点
    {
        //////////////////////////////////////////////////////////////////////////////////////////////////
        if (entity.HasComponent<TagComponent>()) {
            auto& tag = entity.GetComponent< TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));//buffer数组的所有元素都设置为0。
            strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));//将tag字符串复制到buffer数组中。

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) {//如果输入文本(##表示注释掉）
                tag = std::string(buffer);//将输入的文本给标签
            }
        }
        //AddComponent//////////////////////////////////////////////////////////////////////////
        ImGui::SameLine();
        ImGui::PushItemWidth(-1);
        //添加组件按钮///////////////////////////////////////////////
        if (ImGui::Button("Add Component")) {
            ImGui::OpenPopup("AddComponent");
        }
        if (ImGui::BeginPopup("AddComponent")) 
        {
            DisplayAddComponentEntry<CameraComponent>("Camera");
            DisplayAddComponentEntry<ScriptComponent>("Script");
            DisplayAddComponentEntry<SpriteRendererComponent>("Sprite Renderer");
            DisplayAddComponentEntry<CircleRendererComponent>("Circle Renderer");
            DisplayAddComponentEntry<Rigidbody2DComponent>("Rigidbody 2D");
            DisplayAddComponentEntry<BoxCollider2DComponent>("Box Collider 2D");
            DisplayAddComponentEntry<CircleCollider2DComponent>("Circle Collider 2D");

            ImGui::EndPopup();
        }
        ImGui::PopItemWidth();
        //////////////////////////////////////////////////////////////////////////////////////////////////
        DrawComponent< TransformComponent>("Transform", entity,
            [](auto& component) {//自定义函数
                DrawVec3Control("Translation", component.Translation);//传入初始值
                glm::vec3 rotation = glm::degrees(component.Rotation);//转为角度制
                DrawVec3Control("Rotation", rotation);
                component.Rotation = glm::radians(rotation);//转回弧度制
                DrawVec3Control("Scale", component.Scale, 1.0f);
            }
        );
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        DrawComponent< CameraComponent>("Camera", entity,
            [](auto& component) {//自定义函数
                //
                auto& camera = component.Camera;

                ImGui::Checkbox("Primary", &component.Primary);
                //
                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };//透视 / 正交
                const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];//设置选择默认值：数组索引 0 / 1 
                if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {//选择

                    for (int i = 0; i < 2; i++) {//选中哪个，currentProjectionTypeString更新为哪个
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];//当前选中的 == 数组中的这一位时，isSelected为真,否则右边不成立为否
                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {//创建可选择元素：元素内容，当用户点击该元素时，isSelected更新为true
                            //currentProjectionTypeString = projectionTypeStrings[i];
                            component.Camera.SetProjectionType((SceneCamera::ProjectionType)i);
                        }
                        if (isSelected) {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                //更改投影矩阵大小（面板）
                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective) {//透视
                    float verticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());//保存上一次数据
                    if (ImGui::DragFloat("Vertical FOV", &verticalFov))//每帧重新绘制更新
                        camera.SetPerspectiveVerticalFOV(glm::radians(verticalFov));

                    float orthoNear = camera.GetPerspectiveNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear))
                        camera.SetPerspectiveNearClip(orthoNear);

                    float orthoFar = camera.GetPerspectiveFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar))
                        camera.SetPerspectiveFarClip(orthoFar);
                }

                if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic) {//正交
                    float orthoSize = camera.GetOrthographicSize();
                    if (ImGui::DragFloat("Size", &orthoSize)) {
                        camera.SetOrthographicSize(orthoSize);
                    }

                    float orthoNear = camera.GetOrthographicNearClip();
                    if (ImGui::DragFloat("Near", &orthoNear)) {
                        camera.SetOrthographicNearClip(orthoNear);
                    }

                    float orthoFar = camera.GetOrthographicFarClip();
                    if (ImGui::DragFloat("Far", &orthoFar)) {
                        camera.SetOrthographicFarClip(orthoFar);
                    }
                    ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
                }
            }
        );
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        DrawComponent<ScriptComponent>("Script", entity, [entity, scene = m_Context](auto& component) mutable
            {
                bool scriptClassExists = ScriptEngine::EntityClassExists(component.ClassName);//ScriptComponent组件的名称，是一个entity子类

                static char buffer[64];
                strcpy_s(buffer, sizeof(buffer), component.ClassName.c_str());

                if (!scriptClassExists)//不是子类，颜色为红色（异常）
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.3f, 1.0f));

                if (ImGui::InputText("Class", buffer, sizeof(buffer)))//创建文本可以设置组件的值
                    component.ClassName = buffer;

                // Fields
                bool sceneRunning = scene->IsRunning();
                if (sceneRunning)
                {
                    Ref<ScriptInstance> scriptInstance = ScriptEngine::GetEntityScriptInstance(entity.GetUUID());
                    if (scriptInstance)
                    {
                        const auto& fields = scriptInstance->GetScriptClass()->GetFields();

                        for (const auto& [name, field] : fields)
                        {
                            if (field.Type == ScriptFieldType::Float)
                            {
                                float data = scriptInstance->GetFieldValue<float>(name);//T模板
                                if (ImGui::DragFloat(name.c_str(), &data))
                                {
                                    scriptInstance->SetFieldValue(name, data);//面板上设置字段
                                }
                            }
                        }
                    }

                }
                else {//没有运行，但是脚本输入名称是子类
                    if (scriptClassExists) {
                        Ref<ScriptClass> entityClass = ScriptEngine::GetEntityClass(component.ClassName);//获取到对应的ScriptClass
                        const auto& fields = entityClass->GetFields();//获取到fields哈希
                        auto& entityFields = ScriptEngine::GetScriptFieldMap(entity);//获取实体对应的ScriptFieldMap
                        
                        for (const auto& [name, field] : fields)//循环每个字段
                        {
                            // 字段已在编辑器中设置
                            if (entityFields.find(name) != entityFields.end())//能在ScriptFieldMap找到
                            {
                                ScriptFieldInstance& scriptField = entityFields.at(name);//获取到ScriptFieldInstance

                                // 显示控制，也许可以设置
                                if (field.Type == ScriptFieldType::Float)
                                {
                                    float data = scriptField.GetValue<float>();//m_Buffer
                                    if (ImGui::DragFloat(name.c_str(), &data))
                                        scriptField.SetValue(data);
                                }
                            }
                            else//没有设置
                            {
                                //显示控制，也许可以设置
                                if (field.Type == ScriptFieldType::Float)
                                {
                                    float data = 0.0f;//创建新的值
                                    if (ImGui::DragFloat(name.c_str(), &data))
                                    {
                                        ScriptFieldInstance& fieldInstance = entityFields[name];//在ScriptFieldMap存放
                                        fieldInstance.Field = field;
                                        fieldInstance.SetValue(data);//初始为0
                                    }
                                }
                            }
                        }
                    }
                }
                if (!scriptClassExists)//不是子类
                    ImGui::PopStyleColor();
            });

        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        DrawComponent< SpriteRendererComponent>("Sprite Renderer", entity,
            [](auto& component) {//自定义函数
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));//颜色面板,
                //Texture
                ImGui::Button("Texture", ImVec2(100.0f, 0.0f));

                if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))//获取到拖放的资源路径
                    {
                        const wchar_t* path = (const wchar_t*)payload->Data;
                        std::filesystem::path texturePath = std::filesystem::path(g_AssetPath) / path;//取得完整路径
                        Ref<Texture2D> texture = Texture2D::Create(texturePath.string());//创建纹理
                        if (texture->IsLoaded()) {//读取图像文件存在
                            component.Texture = texture;
                        }
                        else
                            HZ_CLIENT_WARN("Could not load texture {0}", texturePath.filename().string());
                    }
                    ImGui::EndDragDropTarget();
                }

                ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);//Tiling Factor
            }
        );
        DrawComponent<CircleRendererComponent>("Circle Renderer", entity, [](auto& component)
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
                ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
                ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
            });
        //////////////////////////////////////////////////////////////////////////////////////////////////////////
        DrawComponent<Rigidbody2DComponent>("Rigidbody 2D", entity, [](auto& component)
            {
                const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
                const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];
                if (ImGui::BeginCombo("Body Type", currentBodyTypeString))
                {
                    for (int i = 0; i < 2; i++)
                    {
                        bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
                        if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
                        {
                            currentBodyTypeString = bodyTypeStrings[i];
                            component.Type = (Rigidbody2DComponent::BodyType)i;
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                ImGui::Checkbox("Fixed Rotation", &component.FixedRotation);
            });

        DrawComponent<BoxCollider2DComponent>("Box Collider 2D", entity, [](auto& component)
            {
                ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
                ImGui::DragFloat2("Size", glm::value_ptr(component.Offset));
                ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
            });
        DrawComponent<CircleCollider2DComponent>("Circle Collider 2D", entity, [](auto& component)
            {
                ImGui::DragFloat2("Offset", glm::value_ptr(component.Offset));
                ImGui::DragFloat("Radius", &component.Radius);
                ImGui::DragFloat("Density", &component.Density, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Friction", &component.Friction, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution", &component.Restitution, 0.01f, 0.0f, 1.0f);
                ImGui::DragFloat("Restitution Threshold", &component.RestitutionThreshold, 0.01f, 0.0f);
            });

    }
    ///////////////////////////////////////////////////////////////////////////
    template<typename T>
    void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& entryName) {
        if (!m_SelectionContext.HasComponent<T>())
        {
            if (ImGui::MenuItem(entryName.c_str()))
            {
                m_SelectionContext.AddComponent<T>();
                ImGui::CloseCurrentPopup();
            }
        }
    }
}