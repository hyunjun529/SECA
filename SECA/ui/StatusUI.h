#ifndef SECA_UI_STATUS_H_
#define SECA_UI_STATUS_H_


#include <string>


#include <imgui.h>


#include "CommonUI.h"


namespace seca
{
	namespace ui
	{
		class StatusUI : public CommonUI
		{
		public:
			ImVec4 *param_clearColor;

		private:
			std::string m_ui_title = "Status";

		public: 
			StatusUI() {}
			~StatusUI() {}

			void Render()
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
				ImGui::Begin("Status UI");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
					1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::Text("Color?");
				ImGui::ColorEdit3("clear color", (float*)param_clearColor);

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::Text("Mode?");

				ImGui::End();
			}
		};
	}
}

#endif // SECA_UI_STATUS_H_
