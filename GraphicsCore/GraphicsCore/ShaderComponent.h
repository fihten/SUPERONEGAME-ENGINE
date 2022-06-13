#pragma once
#include <string>
#include <list>

class ShaderComposite;
class ShaderVisitor;

class ShaderComponent
{
	std::string name = "";
	ShaderComponent* parent = nullptr;

private:
	void setParent(ShaderComponent* parent) { this->parent = parent; };
	ShaderComponent* getParent() { return parent; };

private:
	friend class ShaderComposite;

public:
	virtual ~ShaderComponent() {};
public:
	virtual void add(ShaderComponent* component) {};
	virtual void query(ShaderVisitor* visitor);
	void setName(const std::string& name) { this->name = name; };
	std::string getName() const { return name; };
};

class ShaderComposite : public ShaderComponent
{
protected:
	std::list<ShaderComponent*> childs;
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
	void query(ShaderVisitor* visitor);
};

class SHADER : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class Leaf : public ShaderComponent
{

};

class CBUFFER : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class TYPE : public ShaderComposite
{

};

class FLOAT4X4 : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class VOID : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class FLOAT : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class FLOAT2 : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class FLOAT3 : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class FLOAT4 : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class STRUCT : public TYPE
{
public:
	void query(ShaderVisitor* visitor);
};

class SEMANTIC : public Leaf
{
public:
	void query(ShaderVisitor* visitor);
};

class SV_POSITION : public SEMANTIC
{
public:
	void query(ShaderVisitor* visitor);
};

class SV_TARGET : public SEMANTIC
{
public:
	void query(ShaderVisitor* visitor);
};

class FUNCTION_DECL : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class FUNCTION_CALL : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class MUL : public FUNCTION_CALL
{
public:
	void query(ShaderVisitor* visitor);
};

class FLOAT4_CONSTRUCTOR : public FUNCTION_CALL
{
public:
	void query(ShaderVisitor* visitor);
};

class FUNCTION : public ShaderComposite
{

};

class SET_VERTEX_SHADER : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class SET_PIXEL_SHADER : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class COMPILE_SHADER : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class VERTEX_SHADER_VERSION : public Leaf
{
public:
	void query(ShaderVisitor* visitor);
};

class PIXEL_SHADER_VERSION : public Leaf
{
public:
	void query(ShaderVisitor* visitor);
};

class MODIFIER : public Leaf
{
public:
	void query(ShaderVisitor* visitor);
};

class IN : public MODIFIER
{
public:
	void query(ShaderVisitor* visitor);
};

class OUT : public MODIFIER
{
public:
	void query(ShaderVisitor* visitor);
};

class INOUT : public MODIFIER
{
public:
	void query(ShaderVisitor* visitor);
};

class UNIFORM : public MODIFIER
{
public:
	void query(ShaderVisitor* visitor);
};

class VARIABLE : public Leaf
{
public:
	void query(ShaderVisitor* visitor);
};

class VARIABLE_DECL : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class BINARY_MINUS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class BINARY_PLUS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class BINARY_DIVIDE : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class BINARY_MULTIPLY : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class UNARY_MINUS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class UNARY_PLUS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class ASSIGNMENT : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class TECHNIQUE11 : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class PASS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class ROUND_BRACKETS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class SQUARE_BRACKETS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class CURLY_BRACKETS : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};

class RETURN : public ShaderComposite
{
public:
	void query(ShaderVisitor* visitor);
};