#ifndef KATA_COMPONENT_IMGUICOMPONENT_H_
#define KATA_COMPONENT_IMGUICOMPONENT_H_

#include "../Component.h"
#include "../InputComponent.h"
#include "../PhysicsComponent.h"
#include "../GraphicsComponent.h"

#include <imgui.h>
#include <imgui_impl_glfw_gl3.h>


namespace kata
{
	namespace component
	{
		class ImguiInputComponent : public InputComponent
		{
		private:
			

		public:
			bool show_demo_window = false;
			
			bool funcReset = false;
			bool funcLoad = false;
			int param_load_obj = 0;

			ImVec4 param_color_clear = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

			ImguiInputComponent()
			{

			}

			void render()
			{
				// need check ImGui context is exist

				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
				ImGui::Begin("contorller");
				ImGui::Text("Hell world!");
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
					1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				if (ImGui::Button("Demo Window")) show_demo_window ^= 1;

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::ColorEdit3("clear color", (float*)&param_color_clear);

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::Text("reset or load .obj file");
				if (ImGui::Button("reset")) funcReset = true;
				if (ImGui::Button("load cube")) { funcLoad = true; param_load_obj = 1; }
				if (ImGui::Button("load bunny")) { funcLoad = true; param_load_obj = 2; }
				if (ImGui::Button("load teapot")) { funcLoad = true; param_load_obj = 3; }
				if (ImGui::Button("load capsule")) { funcLoad = true; param_load_obj = 4; }
				if (ImGui::Button("load white_oak")) { funcLoad = true; param_load_obj = 5; }
				if (ImGui::Button("load Kizuna A.I")) { funcLoad = true; param_load_obj = 6; }


				if (show_demo_window)
				{
					ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
					ImGui::ShowDemoWindow(&show_demo_window);
				}

				ImGui::End();
			}

			const ImVec4* getVec4Clear() {
				return &param_color_clear;
			}
		};
	}
}

#endif // KATA_COMPONENT_IMGUICOMPONENT_H_
