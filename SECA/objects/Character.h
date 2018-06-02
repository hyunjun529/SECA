#ifndef SECA_OBJECTS_CHARACTER_H_
#define SECA_OBJECTS_CHARACTER_H_


#include <string>


#include <GL/gl3w.h>
#include <glm/glm.hpp>


#include "render/Object.h"


namespace seca
{
	namespace objects
	{
		class Character
		{
		private:

		public:
			Character() {}
			~Character() {}


			std::string character_name = "unknown";
			render::Object object;
		};
	}
}

#endif // SECA_OBJECTS_CHARACTER_H_
