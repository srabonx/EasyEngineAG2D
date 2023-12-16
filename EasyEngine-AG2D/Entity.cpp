#include "EasyEngineAG2D.h"

namespace EE {
	Entity::Entity(enum RenderType renderType)
	{
		this->renderType = renderType;
		this->id = -1;
		this->name = "";
		this->visible = true;
		this->alive = true;
		this->objectType = 0;
		this->lifeTimeLength = 0;
		this->lifeTimeTimer.reset();
	}
};
