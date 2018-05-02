#ifndef SECA_COMPONENT_GRAPHICSCOMPONENT_H_
#define SECA_COMPONENT_GRAPHICSCOMPONENT_H_

#include "Component.h"

namespace seca
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

#endif // SECA_COMPONENT_GRAPHICSCOMPONENT_H_
