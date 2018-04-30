#ifndef KATA_COMPONENT_PHYSICSCOMPONENT_H_
#define KATA_COMPONENT_PHYSICSCOMPONENT_H_

#include "Component.h"

namespace kata
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

#endif // KATA_COMPONENT_PHYSICSCOMPONENT_H_
