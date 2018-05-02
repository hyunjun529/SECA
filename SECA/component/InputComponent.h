#ifndef SECA_COMPONENT_INPUTCOMPONENT_H_
#define SECA_COMPONENT_INPUTCOMPONENT_H_

#include "Component.h"

namespace seca
{
	namespace component
	{
		class InputComponent : public Component
		{
		public:
			virtual void update() {}
		};
	}
}

#endif // SECA_COMPONENT_INPUTCOMPONENT_H_
