#ifndef SECA_UI_OBJECTLIST_H_
#define SECA_UI_OBJECTLIST_H_


#include <vector>
#include <string>


#include <imgui.h>


#include "CommonUI.h"


#include "render/Object.h"


namespace seca
{
	namespace ui
	{
		class ObjectListUI : public CommonUI
		{
		public:
			std::vector<render::Object> *m_object_lists;

		private:
			std::string m_ui_title = "ObjectList";


		public: 
			ObjectListUI() {}
			~ObjectListUI() {}

			void render()
			{
				ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
				ImGui::Begin("ObjectList UI");
				
				std::vector<render::Object>::iterator itr;

				for (itr = m_object_lists->begin();
					itr != m_object_lists->end();
					itr++)
				{
					ImGui::Text("test");
				}

				ImGui::Spacing(); ImGui::Separator(); ImGui::Spacing();

				ImGui::End();
			}
		};
	}
}

#endif // SECA_UI_OBJECTLIST_H_
