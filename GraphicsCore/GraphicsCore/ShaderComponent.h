#pragma once
#include <string>
#include <list>

class ShaderComposite;

class ShaderComponent
{
	std::string name = "";
	ShaderComponent* parent = nullptr;

private:
	void setParent(ShaderComponent* parent) { this->parent = parent; };
	ShaderComponent* getParent() { return parent; };

private:

	virtual std::list<ShaderComponent*>* getChilds() { return nullptr; };

	class iterator;

	friend class iterator;

	class iterator
	{
		ShaderComponent* current = nullptr;
		std::list<ShaderComponent*>::iterator it;
		std::stack<std::list<ShaderComponent*>::iterator> ancestorsIterators;
	public:
		iterator() {};
		~iterator() {};

		virtual iterator& operator++()
		{
			auto childs = current->getChilds();
			if (childs)
			{
				if (current->parent)
					ancestorsIterators.push(it);
				it = childs->begin();
				current = *it;

				return *this;
			}
			if (current && current->parent == nullptr)
			{
				current = nullptr;
				while (!ancestorsIterators.empty())
					ancestorsIterators.pop();
				return *this;
			}
			if (current && current->parent)
			{
				++it;
				auto childs = current->parent->getChilds();
				if (it == childs->end() && ancestorsIterators.empty())
				{
					current = nullptr;
					while (!ancestorsIterators.empty())
						ancestorsIterators.pop();
					return *this;
				}
				if (it == childs->end())
				{
					it = ancestorsIterators.top();
					ancestorsIterators.pop();
					
					++it;

					current = *it;

					return *this;
				}

				current = *it;

				return *this;
			}
			return *this;
		};
	};

	friend class ShaderComposite;

public:
	virtual ~ShaderComponent() {};
public:
	virtual void add(ShaderComponent* component) {};
	void setName(const std::string& name) { this->name = name; };
};

class ShaderComposite : public ShaderComponent
{
	std::list<ShaderComponent*> childs;
	std::list<ShaderComponent*>* getChilds() override { return &childs; };
public:
	~ShaderComposite()
	{
		for (auto* child : childs)
			delete child;
	};
public:
	void add(ShaderComponent* component) 
	{
		component->setParent(this);
		childs.push_back(component); 
	};
};

class Leaf : public ShaderComponent
{

};

class CBUFFER : public ShaderComposite
{

};

class TYPE : public ShaderComposite
{

};

class FLOAT4X4 : public TYPE
{

};

class VOID : public TYPE
{

};

class FLOAT : public TYPE
{

};

class FLOAT2 : public TYPE
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

class FUNCTION_DECL : public ShaderComposite
{

};

class FUNCTION_CALL : public ShaderComposite
{

};

class MUL : public FUNCTION_CALL
{

};

class FLOAT4_CONSTRUCTOR : public FUNCTION_CALL
{

};

class FUNCTION : public ShaderComposite
{

};

class SET_VERTEX_SHADER : public ShaderComposite
{

};

class SET_PIXEL_SHADER : public ShaderComposite
{

};

class COMPILE_SHADER : public ShaderComposite
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

class VARIABLE_DECL : public ShaderComposite
{

};

class BINARY_MINUS : public ShaderComposite
{

};

class BINARY_PLUS : public ShaderComposite
{

};

class BINARY_DIVIDE : public ShaderComposite
{

};

class BINARY_MULTIPLY : public ShaderComposite
{

};

class UNARY_MINUS : public ShaderComposite
{

};

class UNARY_PLUS : public ShaderComposite
{

};

class ASSIGNMENT : public ShaderComposite
{

};

class TECHNIQUE11 : public ShaderComposite
{

};

class PASS : public ShaderComposite
{

};

class ROUND_BRACKETS : public ShaderComposite
{

};

class SQUARE_BRACKETS : public ShaderComposite
{

};

class CURLY_BRACKETS : public ShaderComposite
{

};

class RETURN : public ShaderComposite
{

};