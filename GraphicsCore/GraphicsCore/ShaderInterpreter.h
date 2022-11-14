#pragma once
#include <string>
#include <queue>
#include <stack>

#include "ShaderComponent.h"

class ShaderInterpreter
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
		NUMBER,
		UNARY_MINUS,
		UNARY_PLUS,
		FUNCTION_CALL,
		ARGUMENTS_LIST_OPEN_BRACKET,
		ARGUMENTS_LIST_CLOSE_BRACKET,
		ASSIGNMENT,
		VOID_,
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		FLOAT4X4,
		CUSTOM_NAME,
		FUNCTION_DECLARATION,
		SIGNATURE_OPEN_BRACKET,
		SIGNATURE_CLOSE_BRACKET,
		SEMANTIC,
		SV_POSITION_,
		SV_TARGET_,
		CUSTOM_SEMANTIC,
		FUNCTION_BODY_OPEN_BRACKET,
		FUNCTION_BODY_CLOSE_BRACKET,
		INSERT_FUNCTION_DECLARATION,
		VARIABLE_DECLARATION,
		INSERT_VARIABLE_DECLARATION,
		IN_,
		OUT_,
		INOUT_,
		UNIFORM_,
		MUL,
		FLOAT4_CONSTRUCTOR,
		CBUFFER,
		CBUFFER_NAME,
		INSERT_CBUFFER,
		CBUFFER_BODY_OPEN_BRACKET,
		TECHNIQUE11,
		INSERT_TECHNIQUE11,
		PASS,
		INSERT_PASS,
		SET_VERTEX_SHADER,
		SET_PIXEL_SHADER,
		COMPILE_SHADER,
		VS_5_0,
		PS_5_0,
		RETURN,
		INSERT_RETURN
	};
	State currentState = State::UNKNOWN;
	std::stack<State> statesStack;

	std::stack<ShaderUnits::ShaderComponent*> componentStack;

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

	void number();

	void unaryMinus();
	void unaryPlus();

	void functionCall();
	void argumentsListOpenBracket();
	void argumentsListCloseBracket();

	void assignment();

	void voidState();
	void floatState();
	void float2State();
	void float3State();
	void float4State();
	void float4x4State();
	void customName();
	
	void functionDeclaration();
	void signatureOpenBracket();
	void signatureCloseBracket();
	void semantic();
	void svPosition();
	void svTarget();
	void customSemantic();
	void functionBodyOpenBracket();
	void functionBodyCloseBracket();
	void insertFunctionDeclaration();

	void variableDeclaration();
	void insertVariableDeclaration();

	void inState();
	void outState();
	void inoutState();
	void uniformState();
	ShaderUnits::ShaderComponent* modifier = nullptr;

	void mulState();
	void float4constructor();

	struct CbufferDefinition
	{
		std::string name = "";
		ShaderUnits::ShaderComponent* body = nullptr;

		void clear() { name = ""; body = nullptr; }
	};
	CbufferDefinition cbufferDecl;

	void cbufferState();
	void cbufferName();
	void insertCbuffer();
	void cbufferBodyOpenBracket();

	ShaderUnits::ShaderComponent* tech = nullptr;
	void technique11state();
	void insertTechnique11();

	ShaderUnits::ShaderComponent* pass = nullptr;
	void passState();
	void insertPass();

	void setVertexShaderState();
	void setPixelShaderState();
	void compileShader();

	void vs_5_0_state();
	void ps_5_0_state();

	void returnState();
	void insertReturn();

	struct DeclarationFunctionOrVariable
	{
		ShaderUnits::ShaderComponent* modifier = nullptr;
		ShaderUnits::ShaderComponent* type = nullptr;
		std::string name = "";
		ShaderUnits::ShaderComponent* signature = nullptr;
		ShaderUnits::ShaderComponent* semantic = nullptr;
		ShaderUnits::ShaderComponent* body = nullptr;
		ShaderUnits::ShaderComponent* value = nullptr;
	};
	std::stack<ShaderInterpreter::DeclarationFunctionOrVariable> decls;

private:
	bool isOperationState(State state) const;

public:
	void setShaderText(const std::string& shaderText)
	{
		this->shaderText = shaderText;
		this->currentIndex = 0;
	}
	ShaderUnits::SHADER* build();
};