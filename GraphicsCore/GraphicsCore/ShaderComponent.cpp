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

void SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void CBUFFER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void FLOAT4X4::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void VOID::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void FLOAT1::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void FLOAT2::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void FLOAT3::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void FLOAT4::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void STRUCT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void SEMANTIC::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void SV_POSITION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void SV_TARGET::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void FUNCTION_DECL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void FUNCTION_CALL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void MUL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void FLOAT4_CONSTRUCTOR::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void SET_VERTEX_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void SET_PIXEL_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void COMPILE_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void VERTEX_SHADER_VERSION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void PIXEL_SHADER_VERSION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void MODIFIER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void IN::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void OUT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void INOUT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void UNIFORM::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void VARIABLE::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void VARIABLE_DECL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void BINARY_MINUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void BINARY_PLUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void BINARY_DIVIDE::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void BINARY_MULTIPLY::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void UNARY_MINUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void UNARY_PLUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ASSIGNMENT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void TECHNIQUE11::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void PASS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ROUND_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void SQUARE_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void CURLY_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void RETURN::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}