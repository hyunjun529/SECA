#ifndef SECA_COMPONENT_PHYSICSCOMPONENT_H_
#define SECA_COMPONENT_PHYSICSCOMPONENT_H_

#include "Component.h"

namespace seca
{
	namespace component
	{
		class PhysicsComponent : public Component
		{
		public:
			virtual void update() {}
		};
	}
}

#endif // SECA_COMPONENT_PHYSICSCOMPONENT_H_
