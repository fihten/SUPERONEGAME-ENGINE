#pragma once
#include <string>
#include <list>

class Component
{
	std::string name = "";
public:
	virtual ~Component() {};
public:
	virtual void add(Component* component) {};
};

class Composite : public Component
{
	std::list<Component*> childs;
public:
	~Composite()
	{
		for (auto* child : childs)
			delete child;
	};
public:
	void add(Component* component) { childs.push_back(component); };
};

class Leaf : public Component
{

};

class CBUFFER : public Composite
{

};

class TYPE : public Composite
{

};

class FLOAT4X4 : public TYPE
{

};

class VOID : public TYPE
{

};

class FLOAT3 : public TYPE
{

};

class FLOAT4 : public TYPE
{

};

class STRUCT : public TYPE
{

};

class SEMANTIC : public Leaf
{

};

class SV_POSITION : public SEMANTIC
{

};

class SV_TARGET : public SEMANTIC
{

};

class FUNCTION_DECL : public Composite
{

};

class FUNCTION_CALL : public Composite
{

};

class MUL : public FUNCTION_CALL
{

};

class FLOAT4_CONSTRUCTOR : public FUNCTION_CALL
{

};

class FUNCTION : public Composite
{

};

class SET_VERTEX_SHADER : public Composite
{

};

class SET_PIXEL_SHADER : public Composite
{

};

class COMPILE_SHADER : public Composite
{

};

class VERTEX_SHADER_VERSION : public Leaf
{

};

class PIXEL_SHADER_VERSION : public Leaf
{

};

class MODIFIER : public Leaf
{

};

class IN : public MODIFIER
{

};

class OUT : public MODIFIER
{

};

class INOUT : public MODIFIER
{

};

class UNIFORM : public MODIFIER
{

};

class VARIABLE : public Leaf
{

};

class VARIABLE_DECL : public Composite
{

};

class OPERATOR : public Composite
{

};

class ASSIGNMENT : public Composite
{

};

class TECHNIQUE11 : public Composite
{

};

class PASS : public Composite
{

};

class ROUND_BRACKETS : public Composite
{

};

class SQUARE_BRACKETS : public Composite
{

};

class CURLY_BRACKETS : public Composite
{

};

class RETURN : public Composite
{

};
