#ifndef SECA_UI_CHARACTERLIST_H_
#define SECA_UI_CHARACTERLIST_H_


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
		class CharacterListUI : public CommonUI
		{
		public:
			std::vector<objects::Character> *m_character_list;

		private:
			std::string m_ui_title = "CharacterListUI";


		public: 
			CharacterListUI() {}
			~CharacterListUI() {}

			void Render()
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
				ImGui::Begin("Loaded Characters");
				
				std::vector<objects::Character>::iterator itr;

				for (itr = m_character_list->begin();
					itr != m_character_list->end();
					itr++)
				{
					ImGui::Text("%s", itr->character_name.c_str());
				}

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::End();
			}
		};
	}
}

#endif // SECA_UI_OBJECTLIST_H_
