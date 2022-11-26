#include "ShaderComponent.h"
#include "ShaderVisitor.h"

void ShaderUnits::ShaderComponent::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::ShaderComposite::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::CBUFFER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT4X4::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::VOID_::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT1::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT2::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT3::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT4::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::STRUCT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::SEMANTIC::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::SV_POSITION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::SV_TARGET::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::FUNCTION_DECL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::FUNCTION_CALL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::MUL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::FLOAT4_CONSTRUCTOR::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::SET_VERTEX_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::SET_PIXEL_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::COMPILE_SHADER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::VERTEX_SHADER_VERSION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::PIXEL_SHADER_VERSION::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::MODIFIER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::IN_MODIFIER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::OUT_MODIFIER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::INOUT_MODIFIER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::UNIFORM::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::VARIABLE::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::NUMBER::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::COMMENT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	visitor->finishVisit(this);
}

void ShaderUnits::VARIABLE_DECL::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::BINARY_MINUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::BINARY_PLUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::BINARY_DIVIDE::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::BINARY_MULTIPLY::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::GREATER_THAN::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::UNARY_MINUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::UNARY_PLUS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::ASSIGNMENT::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::TECHNIQUE11::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::PASS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::ROUND_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::SQUARE_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::CURLY_BRACKETS::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::RETURN::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}

void ShaderUnits::IF_NODE::query(ShaderVisitor* visitor)
{
	visitor->startVisit(this);
	for (auto ch : childs)
		ch->query(visitor);
	visitor->finishVisit(this);
}