#include "Editor/ImGuiWrapper/ImGuiWrapper.h"
#include "Editor/Panels/SceneHierarchyPanel.h"
#include "Editor/IconManager/IconManager.h"

#include "Runtime/EcsFramework/Component/ComponentGroup.h"
#include "Runtime/Renderer/Texture.h"
#include "Runtime/Resource/ConfigManager/ConfigManager.h"
#include "Runtime/Utils/PlatformUtils.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <glm/gtc/type_ptr.hpp>

#include <filesystem>


namespace HEngine
{
    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Level>& context)
    {
		SetContext(context);
    }

    void SceneHierarchyPanel::SetContext(const Ref<Level>& context)
    {
        mContext = context;
        mSelectionContext = {};
    }

    void SceneHierarchyPanel::OnImGuiRender(bool* pOpen, bool* pOpenProperties)
    {
		if (*pOpen)
		{
			ImGui::Begin("Scene Hierarchy", pOpen);

			if (mContext)
			{
				mContext->mRegistry.each([&](auto entityID)
					{
						Entity entity = { entityID, mContext.get() };
						DrawEntityNode(entity);
					});

				if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
					mSelectionContext = {};

				// Right-click on blank space
				if (ImGui::BeginPopupContextWindow(0, 1, false))
				{
					if (ImGui::MenuItem("Create Empty Entity"))
						mContext->CreateEntity("Empty Entity");

					if (ImGui::MenuItem("Create Point Light"))
					{
						auto entity = mContext->CreateEntity("Light");
						entity.AddComponent<LightComponent>();
						SetSelectedEntity(entity);
					}

					ImGui::EndPopup();
				}
			}

			ImGui::End();
		}
		if (*pOpenProperties)
		{
			ImGui::Begin("Properties", pOpenProperties);
			if (mSelectionContext)
			{
				DrawComponents(mSelectionContext);
			}
			ImGui::End();
		}
    }

    void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
    {
		mSelectionContext = entity;
    }

    void SceneHierarchyPanel::DrawEntityNode(Entity entity)
    {
		const char* name = "Unnamed Entity";
		if (entity.HasComponent<TagComponent>()) 
		{
			name = entity.GetComponent<TagComponent>().Tag.c_str();
		}

        ImGuiTreeNodeFlags flags = ((mSelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		std::string label = std::string("##") + std::string(name);
        bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "");
        if (ImGui::IsItemClicked())
        {
            mSelectionContext = entity;
        }
        bool entityDeleted = false;
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Delete Entity"))
                entityDeleted = true;

            ImGui::EndPopup();
        }
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(1.0f, 1.0f, 1.0f, 0.0f));
		ImGui::Image((ImTextureID)IconManager::GetInstance().Get("EntityIcon")->GetRendererID(), ImVec2{lineHeight - 5.0f, lineHeight - 5.0f}, { 0, 1 }, { 1, 0 });
		ImGui::PopStyleColor(1);
		ImGui::SameLine();
		ImGui::Text(name);

        if (opened)
        {
            ImGui::TreePop();
        }

        if (entityDeleted)
        {
            mContext->DestroyEntity(entity);
            if (mSelectionContext == entity)
                mSelectionContext = {};
        }
    }

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto boldFont = io.Fonts->Fonts[0];

        ImGui::Columns(2, nullptr, false);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

		ImGui::BeginTable("table_padding", 3, ImGuiTableFlags_NoBordersInBody | ImGuiTableFlags_NoPadInnerX);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::SameLine();


		ImGui::TableSetColumnIndex(1);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::SameLine();

		ImGui::TableSetColumnIndex(2);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushFont(boldFont);
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopFont();
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");

        ImGui::PopStyleVar();

		ImGui::EndTable();

		ImGui::EndColumns();
    }

    template<typename T, typename UIFunction>
    static void DrawComponent(const std::string& name, Entity entity, UIFunction uiFunction)
    {
        const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        if (entity.HasComponent<T>())
        {
            auto& component = entity.GetComponent<T>();
            ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

			float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
			bool open = ImGuiWrapper::TreeNodeExStyle1((void*)typeid(T).hash_code(), name, treeNodeFlags);
            ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
			
            if (ImGui::ImageButton((ImTextureID)IconManager::GetInstance().GetSettingIcon()->GetRendererID(), ImVec2{lineHeight - 7.0f, lineHeight - 7.0f}))
            {
                ImGui::OpenPopup("ComponentSettings");
            }

            bool removeComponent = false;
            if (ImGui::BeginPopup("ComponentSettings"))
            {
                if (ImGui::MenuItem("Remove component"))
                    removeComponent = true;

                ImGui::EndPopup();
            }

            if (open)
            {
                uiFunction(component);
                ImGui::TreePop();
            }

            if (removeComponent)
                entity.RemoveComponent<T>();
        }
    }

    void SceneHierarchyPanel::DrawComponents(Entity entity)
    {
        if (entity.HasComponent<TagComponent>())
        {
            auto& tag = entity.GetComponent<TagComponent>().Tag;

            char buffer[256];
            memset(buffer, 0, sizeof(buffer));
            strcpy_s(buffer, sizeof(buffer), tag.c_str());
            if (ImGui::InputText("##Tag", buffer, sizeof(buffer))) 
            {
                tag = std::string(buffer);
            }
        }

        ImGui::SameLine();
        ImGui::PushItemWidth(-1);

        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("AddComponent");

        if (ImGui::BeginPopup("AddComponent"))
        {
			if (!mSelectionContext.HasComponent<CameraComponent>())
			{
				if (ImGui::MenuItem("Camera"))
				{
					mSelectionContext.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<SpriteRendererComponent>())
			{
				if (ImGui::MenuItem("Sprite Renderer"))
				{
					mSelectionContext.AddComponent<SpriteRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<CircleRendererComponent>())
			{
				if (ImGui::MenuItem("Circle Renderer"))
				{
					mSelectionContext.AddComponent<CircleRendererComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<Rigidbody2DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 2D"))
				{
					mSelectionContext.AddComponent<Rigidbody2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<BoxCollider2DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 2D"))
				{
					mSelectionContext.AddComponent<BoxCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<CircleCollider2DComponent>())
			{
				if (ImGui::MenuItem("Circle Collider 2D"))
				{
					mSelectionContext.AddComponent<CircleCollider2DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<MeshComponent>())
			{
				if (ImGui::MenuItem("Mesh Renderer"))
				{
					mSelectionContext.AddComponent<MeshComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<Rigidbody3DComponent>())
			{
				if (ImGui::MenuItem("Rigidbody 3D"))
				{
					mSelectionContext.AddComponent<Rigidbody3DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<BoxCollider3DComponent>())
			{
				if (ImGui::MenuItem("Box Collider 3D"))
				{
					mSelectionContext.AddComponent<BoxCollider3DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<SphereCollider3DComponent>())
			{
				if (ImGui::MenuItem("Sphere Collider 3D"))
				{
					mSelectionContext.AddComponent<SphereCollider3DComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<LightComponent>())
			{
				if (ImGui::MenuItem("Point Light"))
				{
					mSelectionContext.AddComponent<LightComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

			if (!mSelectionContext.HasComponent<PythonScriptComponent>())
			{
				if (ImGui::MenuItem("Python Script"))
				{
					mSelectionContext.AddComponent<PythonScriptComponent>();
					ImGui::CloseCurrentPopup();
				}
			}

            ImGui::EndPopup();
        }

        ImGui::PopItemWidth();

        DrawComponent<TransformComponent>("Transform", entity, [](auto& component)
            {
                DrawVec3Control("Translation", component.Translation);
                glm::vec3 rotation = glm::degrees(component.Rotation);
                DrawVec3Control("Rotation", rotation);
                component.Rotation = glm::radians(rotation);
                DrawVec3Control("Scale", component.Scale, 1.0f);
            });

		DrawComponent<CameraComponent>("Camera", entity, [](auto& component)
			{
				auto& camera = component.Camera;

				ImGui::Checkbox("Primary", &component.Primary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
				const char* currentProjectionTypeString = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionTypeString))
				{
					for (int i = 0; i < 2; i++)
					{
						bool isSelected = (currentProjectionTypeString == projectionTypeStrings[i]);
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionTypeString = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float perspectiveVerticalFov = glm::degrees(camera.GetPerspectiveVerticalFOV());
					if (ImGui::DragFloat("Vertical FOV", &perspectiveVerticalFov))
						camera.SetPerspectiveVerticalFOV(glm::radians(perspectiveVerticalFov));

					float perspectiveNear = camera.GetPerspectiveNearClip();
					if (ImGui::DragFloat("Near", &perspectiveNear))
						camera.SetPerspectiveNearClip(perspectiveNear);

					float perspectiveFar = camera.GetPerspectiveFarClip();
					if (ImGui::DragFloat("Far", &perspectiveFar))
						camera.SetPerspectiveFarClip(perspectiveFar);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					if (ImGui::DragFloat("Size", &orthoSize))
						camera.SetOrthographicSize(orthoSize);

					float orthoNear = camera.GetOrthographicNearClip();
					if (ImGui::DragFloat("Near", &orthoNear))
						camera.SetOrthographicNearClip(orthoNear);

					float orthoFar = camera.GetOrthographicFarClip();
					if (ImGui::DragFloat("Far", &orthoFar))
						camera.SetOrthographicFarClip(orthoFar);

					ImGui::Checkbox("Fixed Aspect Ratio", &component.FixedAspectRatio);
				}
			});

        DrawComponent<SpriteRendererComponent>("Sprite Render", entity, [](auto& component)
            {
                ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
                
				ImGui::Button("Texture", ImVec2(100.0f, 0.0f));
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
						const wchar_t* path = (const wchar_t*)payload->Data;
						std::filesystem::path texturePath = std::filesystem::path(ConfigManager::GetInstance().GetAssetsFolder()) / path;
						Ref<Texture2D> texture = Texture2D::Create(texturePath.string());
						if (texture->IsLoaded())
							component.Texture = texture;
						else
							HE_WARN("Could not load texture {0}", texturePath.filename().string());
					}
					ImGui::EndDragDropTarget();
				}

                ImGui::DragFloat("Tiling Factor", &component.TilingFactor, 0.1f, 0.0f, 100.0f);
            });

		DrawComponent<CircleRendererComponent>("Circle Render", entity, [](auto& component)
			{
				ImGui::ColorEdit4("Color", glm::value_ptr(component.Color));
				ImGui::DragFloat("Thickness", &component.Thickness, 0.025f, 0.0f, 1.0f);
				ImGui::DragFloat("Fade", &component.Fade, 0.00025f, 0.0f, 1.0f);
			});

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

		DrawComponent<Rigidbody3DComponent>("Rigidbody 3D", entity, [](auto& component)
			{
				const char* bodyTypeStrings[] = { "Static", "Dynamic", "Kinematic" };
				const char* currentBodyTypeString = bodyTypeStrings[(int)component.Type];

				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Body Type");
				ImGui::NextColumn();
				if (ImGui::BeginCombo("##Body Type", currentBodyTypeString))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentBodyTypeString == bodyTypeStrings[i];
						if (ImGui::Selectable(bodyTypeStrings[i], isSelected))
						{
							currentBodyTypeString = bodyTypeStrings[i];
							component.Type = (Rigidbody3DComponent::Body3DType)i;
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}
				ImGui::EndColumns();

				ImGuiWrapper::DrawTwoUI(
					[]() { ImGui::Text("mass"); }, 
					[&component = component]() { ImGui::SliderFloat("##masas", &component.mass, 0.0f, 10.0f, "%.2f"); }
				);
			});

		DrawComponent<BoxCollider3DComponent>("Box Collider 3D", entity, [](auto& component)
			{
				const auto& floatValueUI = [](const char* name, float& value) {
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Text(name);
					ImGui::NextColumn();
					std::string label = std::string("##") + std::string(name);
					ImGui::SliderFloat(label.c_str(), &value, 0.0f, 1.0f, "%.2f");
					ImGui::EndColumns();
				};

				floatValueUI("linearDamping", component.linearDamping);
				floatValueUI("angularDamping", component.angularDamping);
				floatValueUI("restitution", component.restitution);
				floatValueUI("friction", component.friction);
			});

		DrawComponent<SphereCollider3DComponent>("Sphere Collider 3D", entity, [](auto& component)
			{
				const auto& floatValueUI = [](const char* name, float& value) {
					ImGui::Columns(2, nullptr, false);
					ImGui::SetColumnWidth(0, 100.0f);
					ImGui::Text(name);
					ImGui::NextColumn();
					std::string label = std::string("##") + std::string(name);
					ImGui::SliderFloat(label.c_str(), &value, 0.0f, 1.0f, "%.2f");
					ImGui::EndColumns();
				};

				floatValueUI("linearDamping", component.linearDamping);
				floatValueUI("angularDamping", component.angularDamping);
				floatValueUI("restitution", component.restitution);
				floatValueUI("friction", component.friction);
			});

		DrawComponent<MeshComponent>("Static Mesh Renderer", entity, [](MeshComponent& component)
			{
				ImGui::Columns(2, nullptr, false);
				ImGui::SetColumnWidth(0, 100.0f);
				ImGui::Text("Mesh Path");
				ImGui::NextColumn();

				std::string standardPath = std::regex_replace(component.Path, std::regex("\\\\"), "/");
				ImGui::Text(standardPath.substr(standardPath.find_last_of("/") + 1, standardPath.length()).c_str());

				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					std::string filepath = FileDialogs::OpenFile("Model (*.obj *.fbx *.dae *.gltf)\0");
					if (filepath.find("Assets") != std::string::npos)
					{
						filepath = filepath.substr(filepath.find("Assets"), filepath.length());
					}
					else
					{
						// TODO: Import Mesh
						//HE_CORE_ASSERT(false, "HEngine Now Only support the model from Assets!");
						//filepath = "";
					}
					if (!filepath.empty())
					{
						component.mMesh = CreateRef<Mesh>(filepath);
						component.Path = filepath;
					}
				}
				ImGui::EndColumns();

				if (ImGuiWrapper::TreeNodeExStyle2((void*)"Material", "Material"))
				{
					const auto& materialNode = [&model = component.mMesh](const char* name, Ref<Texture2D>& tex, void(*func)(Ref<Mesh>& model)) {
						if (ImGui::TreeNode((void*)name, name))
						{
							ImGui::Image((ImTextureID)tex->GetRendererID(), ImVec2(64, 64), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
							static bool use = false;
							if (ImGui::BeginDragDropTarget())
							{
								if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
								{
									auto path = (const wchar_t*)payload->Data;
									std::string relativePath = (std::filesystem::path("Assets") / path).string();
									std::filesystem::path texturePath = ConfigManager::GetInstance().GetAssetsFolder() / path;
									relativePath = std::regex_replace(relativePath, std::regex("\\\\"), "/");
									tex = IconManager::GetInstance().LoadOrFindTexture(relativePath);
								}
								ImGui::EndDragDropTarget();
							}

							func(model);
							
							ImGui::TreePop();
						}
					};

					materialNode("Albedo", component.mMesh->mMaterial->mAlbedoMap, [](Ref<Mesh>& model) {
						ImGui::SameLine();
						ImGui::Checkbox("Use", &model->mMaterial->bUseAlbedoMap);

						if (ImGui::ColorEdit4("##albedo", glm::value_ptr(model->mMaterial->col)))
						{
							if (!model->mMaterial->bUseAlbedoMap)
							{
								unsigned char data[4];
								for (size_t i = 0; i < 4; i++)
								{
									data[i] = (unsigned char)(model->mMaterial->col[i] * 255.0f);
								}
								model->mMaterial->albedoRGBA->SetData(data, sizeof(unsigned char) * 4);
							}
						}
					});

					materialNode("Normal", component.mMesh->mMaterial->mNormalMap, [](Ref<Mesh>& model) {
						ImGui::SameLine();
						ImGui::Checkbox("Use", &model->mMaterial->bUseNormalMap);
					});

					materialNode("Metallic", component.mMesh->mMaterial->mMetallicMap, [](Ref<Mesh>& model) {
						ImGui::SameLine();

						if (ImGui::BeginTable("Metallic", 1))
						{
							ImGui::TableNextRow();
							ImGui::TableNextColumn();

							ImGui::Checkbox("Use", &model->mMaterial->bUseMetallicMap);

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							if (ImGui::SliderFloat("##Metallic", &model->mMaterial->metallic, 0.0f, 1.0f))
							{
								if (!model->mMaterial->bUseMetallicMap)
								{
									unsigned char data[4];
									for (size_t i = 0; i < 3; i++)
									{
										data[i] = (unsigned char)(model->mMaterial->metallic * 255.0f);
									}
									data[3] = (unsigned char)255.0f;
									model->mMaterial->metallicRGBA->SetData(data, sizeof(unsigned char) * 4);
								}
							}

							ImGui::EndTable();
						}
					});

					materialNode("Roughness", component.mMesh->mMaterial->mRoughnessMap, [](Ref<Mesh>& model) {
						ImGui::SameLine();
						
						if (ImGui::BeginTable("Roughness", 1))
						{
							ImGui::TableNextRow();
							ImGui::TableNextColumn();

							ImGui::Checkbox("Use", &model->mMaterial->bUseRoughnessMap);

							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							if (ImGui::SliderFloat("##Roughness", &model->mMaterial->roughness, 0.0f, 1.0f))
							{
								if (!model->mMaterial->bUseRoughnessMap)
								{
									unsigned char data[4];
									for (size_t i = 0; i < 3; i++)
									{
										data[i] = (unsigned char)(model->mMaterial->roughness * 255.0f);
									}
									data[3] = (unsigned char)255.0f;
									model->mMaterial->roughnessRGBA->SetData(data, sizeof(unsigned char) * 4);
								}
							}

							ImGui::EndTable();
						}
					});

					materialNode("Ambient Occlusion", component.mMesh->mMaterial->mAoMap, [](Ref<Mesh>& model) {
						ImGui::SameLine();
						ImGui::Checkbox("Use", &model->mMaterial->bUseAoMap);
					});

					ImGui::TreePop();
				}

				if (component.mMesh->bAnimated)
				{
					if (ImGuiWrapper::TreeNodeExStyle2((void*)"Animation", "Animation"))
					{
						ImGuiWrapper::DrawTwoUI(
							[&mesh = component.mMesh]() {
								static std::string label = "Play";
								if (ImGui::Button(label.c_str()))
								{
									mesh->bPlayAnim = !mesh->bPlayAnim;
									if (mesh->bPlayAnim)
										label = "Stop";
									else
									{
										label = "Play";
										mesh->mAnimator.Reset();
									}
								}
							},
							[&mesh = component.mMesh]() {
								static std::string label = "Pause";
								if (ImGui::Button(label.c_str()))
								{
									mesh->bStopAnim = !mesh->bStopAnim;
									if (mesh->bStopAnim)
										label = "Resume";
									else
										label = "Pause";
								}
							},
							88.0f
						);

						ImGui::Columns(2, nullptr, false);
						ImGui::Text("Speed");
						ImGui::NextColumn();
						ImGui::SliderFloat("##Speed", &component.mMesh->mAnimPlaySpeed, 0.1f, 10.0f);
						ImGui::EndColumns();

						ImGui::ProgressBar(component.mMesh->mAnimator.GetProgress(), ImVec2(0.0f, 0.0f));

						ImGui::TreePop();
					}
				}
			});

		DrawComponent<LightComponent>("Point Light", entity, [](auto& component)
			{
				ImGui::Text("Light Color");
				ImGui::SameLine();
				ImGui::DragFloat3("##Light Color", (float*)& component.LightColor, 2.0f, 0.0f, 10000.0f, "%.1f");
			});

		DrawComponent<PythonScriptComponent>("Python Script", entity, [](auto& component)
			{
				ImGui::Text("Python Script");
				ImGui::SameLine();
				ImGui::Text(component.Path.c_str());

				ImGui::SameLine();
				if (ImGui::Button("..."))
				{
					std::string filepath = FileDialogs::OpenFile("PythonScript (*.py)\0*.py\0");
					if (!filepath.empty())
					{
						filepath = std::regex_replace(filepath, std::regex("\\\\"), "/");
						filepath = filepath.substr(filepath.find_last_of("/") + 1, filepath.length());
						component.Path = filepath;
					}
				}
			});
    }
}
