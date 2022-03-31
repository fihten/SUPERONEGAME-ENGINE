#pragma once
#include <string>
#include <list>
#include <queue>
#include <stack>

class Component
{
	std::string name = "";
public:
	virtual ~Component() {};
public:
	virtual void add(Component* component) {};
	void setName(const std::string& name) { this->name = name; };
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

class BINARY_MINUS : public Composite
{

};

class BINARY_PLUS : public Composite
{

};

class BINARY_DIVIDE : public Composite
{

};

class BINARY_MULTIPLY : public Composite
{

};

class UNARY_MINUS : public Composite
{

};

class UNARY_PLUS : public Composite
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

class ShaderBuilder
{
	const char skipSymbols[5] =
	{
		32, // space
		9,  // tab
		10, // line feed
		13, // carriage return
		0   
	};

	const char stopSymbols[17] = 
	{
			32, // space
			9,  // tab
			10, // line feed
			13, // carriage return
			'(',
			')',
			'{',
			'}',
			',',
			';',
			':',
			'+',
			'-',
			'*',
			'/',
			'=',
			0
	};

	std::string shaderText = "";
	int currentIndex = 0;
	
	std::queue<std::string> words;

	enum State
	{
		UNKNOWN,
		BRACKETS_UNARY_OPERATOR_OPEN,
		BRACKETS_UNARY_OPERATOR_CLOSE,
		FINISH_EXPRESSION,
		BINARY_MINUS,
		BINARY_PLUS,
		BINARY_MULTIPLY,
		BINARY_DIVIDE,
		CREATING_BINARY_MINUS,
		CREATING_BINARY_PLUS,
		CREATING_BINARY_DIVIDE,
		CREATING_BINARY_MULTIPLY,
		VARIABLE,
		UNARY_MINUS,
		UNARY_PLUS,
		FUNCTION_CALL,
		ARGUMENTS_LIST_OPEN_BRACKET,
		ARGUMENTS_LIST_CLOSE_BRACKET,
		ASSIGNMENT
	};
	State currentState = State::UNKNOWN;
	std::stack<State> statesStack;

	std::stack<Component*> componentStack;

	void unknown();

	void bracketsUnaryOperatorOpen();
	void bracketsUnaryOperatorClose();

	void finishExpression();

	void binaryMinus();
	void creatingBinaryMinus();

	void binaryPlus();
	void creatingBinaryPlus();

	void binaryDivide();
	void creatingBinaryDivide();

	void binaryMultiply();
	void creatingBinaryMultiply();

	std::string userName = "";
	void variable();

	void unaryMinus();
	void unaryPlus();

	void functionCall();
	void argumentsListOpenBracket();
	void argumentsListCloseBracket();

	void assignment();

private:
	bool isOperationState(State state) const;

public:
	void build();
};