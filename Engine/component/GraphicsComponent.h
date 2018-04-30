#ifndef KATA_COMPONENT_GRAPHICSCOMPONENT_H_
#define KATA_COMPONENT_GRAPHICSCOMPONENT_H_

#include "Component.h"

namespace kata
{
	namespace component
	{
		class GraphicsComponent : public Component
		{
		public:
			virtual void render() {}
		};
	}
}

#endif // KATA_COMPONENT_GRAPHICSCOMPONENT_H_
