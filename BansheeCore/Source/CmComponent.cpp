#include "CmComponent.h"
#include "CmSceneObject.h"
#include "CmComponentRTTI.h"

namespace BansheeEngine
{
	Component::Component(const HSceneObject& parent)
		:mParent(parent)
	{
		setName("Component");
	}

	Component::~Component()
	{

	}

	void Component::destroy()
	{
		SO()->destroyComponent(this);
	}

	RTTITypeBase* Component::getRTTIStatic()
	{
		return ComponentRTTI::instance();
	}

	RTTITypeBase* Component::getRTTI() const
	{
		return Component::getRTTIStatic();
	}
}