#ifndef SECA_UI_CONTROLCHARACTERANIMATION_H_
#define SECA_UI_CONTROLCHARACTERANIMATION_H_


#include <vector>
#include <string>


#include <imgui.h>


#include "CommonUI.h"


#include "render/Object.h"


#include "../objects/Character.h"


namespace seca
{
	namespace ui
	{
		class ControlCharacterAnimationUI : public CommonUI
		{
		public:
			objects::Character *m_character;

		private:
			std::string m_ui_title = "CharacterListUI";
			
			float frame_current = 0;
			float frame_length = 5.0f;

		public: 
			ControlCharacterAnimationUI() {}
			~ControlCharacterAnimationUI() {}

			void Render()
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
				ImGui::Begin(("animation control " + m_character->character_name).c_str());
				
				ImGui::SliderFloat("time", &frame_current, 0.0f, frame_length);
				ImGui::Button("Play");
				ImGui::Button("Pause");
				ImGui::Button("Reset");

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::End();
			}
		};
	}
}

#endif // SECA_UI_CONTROLCHARACTERANIMATION_H_
