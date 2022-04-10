#include "hepch.h"
#include "ContentBrowserPanel.h"

#include <imgui/imgui.h>

namespace HEngine
{
	// Once we have projects, change this
	static const std::filesystem::path s_AssetPath = "assets";

	ContentBrowserPanel::ContentBrowserPanel()
		: m_CurrentDirectory(s_AssetPath)
	{
	}

	void ContentBrowserPanel::OnImGuiRender(bool* pOpen)
	{
		static bool bOpen = false;

		// from imgui_demo.cpp: ShowAboutWindow function, should we add flag ImGuiWindowFlags_AlwaysAutoResize?
		if (!ImGui::Begin("Content Browser", pOpen))
		{
			ImGui::End();
			return;
		}

		if (m_CurrentDirectory != std::filesystem::path(s_AssetPath))
		{
			if (ImGui::Button("<-"))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}

		for (auto& directoryEntry : std::filesystem::directory_iterator(s_AssetPath))
		{
			const auto& path = directoryEntry.path();
			auto relativePath = std::filesystem::relative(path, s_AssetPath);
			std::string filenameString = relativePath.filename().string();

			if (directoryEntry.is_directory())
			{
				if (ImGui::Button(filenameString.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
			else
			{
				if (ImGui::Button(filenameString.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}
}