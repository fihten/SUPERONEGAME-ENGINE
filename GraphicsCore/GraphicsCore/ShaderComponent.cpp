#include "ShaderComponent.h"
#include "ShaderVisitor.h"

void ShaderComponent::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderComposite::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}