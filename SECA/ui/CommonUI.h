#ifndef SECA_UI_COMMON_H_
#define SECA_UI_COMMON_H_


#include <string>


namespace seca
{
	namespace ui
	{
		class CommonUI
		{
		private:
			std::string m_ui_title = "not define";

		public:
			CommonUI() {}
			~CommonUI() {}

			virtual void render() {}
		};
	}
}

#endif // SECA_UI_COMMON_H_
