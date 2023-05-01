#pragma once

#include "ShaderConverter.h"
#include "ShaderComponent.h"
#include "HLSLShader.h"
#include <queue>
#include <stack>
#include <map>

class HLSLConverter :public ShaderConverter
{
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
		GREATER_THAN,
		LESS_THAN,
		CREATING_BINARY_MINUS,
		CREATING_BINARY_PLUS,
		CREATING_BINARY_DIVIDE,
		CREATING_BINARY_MULTIPLY,
		CREATING_GREATER_THAN,
		CREATING_LESS_THAN,
		VARIABLE,
		NUMBER,
		COMMENT,
		UNARY_MINUS,
		UNARY_PLUS,
		INCREMENT,
		FUNCTION_CALL,
		ARGUMENTS_LIST_OPEN_BRACKET,
		ARGUMENTS_LIST_CLOSE_BRACKET,
		ASSIGNMENT,
		VOID_,
		INT1,
		FLOAT1,
		FLOAT2,
		FLOAT3,
		FLOAT4,
		FLOAT3X3,
		FLOAT4X4,
		TEXTURE2D,
		USER_TYPE,
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
		FLOAT3_CONSTRUCTOR,
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
		INSERT_RETURN,
		IF,
		IF_CONDITION,
		INSERT_IF_CONDITION,
		IF_BODY,
		INSERT_IF_BODY,
		ELSE,
		ELSE_BODY,
		INSERT_ELSE_BODY,
		INSERT_ENTIRE_IF,
		ATTRIBUTE,
		FLATTEN,
		UNROLL,
		ADDITION_ASSIGN,
		DIVIDES_ASSIGN,
		MULTIPLIES_ASSIGN,
		STRUCT,
		STRUCT_NAME,
		STRUCT_BODY_OPEN_BRACKET,
		INSERT_STRUCT,
		DETERMINE_TYPE_OF_SELECTION,
		SELECTED_FIELD,
		SELECTED_METHOD,
		CAST,
		UPDATE_CAST,
		COUNT_OF_ELEMENTS,
		INSERT_COUNT_OF_ELEMENTS,
		SAMPLER_STATE,
		SAMPLER_STATE_NAME,
		INSERT_SAMPLER_STATE,
		SAMPLER_STATE_BODY_OPEN_BRACKET,
		FOR,
		FOR_EXPRESSION_OPEN_BRACKET,
		FOR_INIT_EXPRESSION,
		FOR_COND_EXPRESSION,
		FOR_LOOP_EXPRESSION,
		INSERT_FOR_EXPRESSION,
		FOR_BODY_OPEN_BRACKET,
		INSERT_FOR_INIT_EXPRESSION,
		INSERT_FOR_COND_EXPRESSION,
		INSERT_FOR_LOOP_EXPRESSION,
		INSERT_FOR
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

	void greaterThan();
	void creatingGreaterThan();

	void lessThan();
	void creatingLessThan();

	std::string userName = "";
	void variable();
	void number();
	void comment();

	void unaryMinus();
	void unaryPlus();
	void increment();

	void functionCall();
	void argumentsListOpenBracket();
	void argumentsListCloseBracket();

	void assignment();

	void voidState();
	void intState();
	void floatState();
	void float2State();
	void float3State();
	void float4State();
	void float3x3State();
	void float4x4State();
	void userTypeState();
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
	void float3constructor();
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

	struct StructDefinition
	{
		std::string name = "";
		ShaderUnits::ShaderComponent* body = nullptr;

		void clear() { name = ""; body = nullptr; }
	};
	StructDefinition structDecl;

	void structState();
	void structName();
	void insertStruct();
	void structBodyOpenBracket();
	std::map<std::string, ShaderUnits::ShaderComponent*> userTypes;
	ShaderUnits::ShaderComponent* userType = nullptr;

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

	void ifState();
	void ifCondition();
	void insertIfCondition();
	void ifBody();
	void insertIfBody();
	void insertEntireIf();
	void elseState();
	void elseBody();
	void insertElseBody();

	void attribute();
	void flatten();
	ShaderUnits::FLATTEN* pFlatten = nullptr;
	void unroll();
	ShaderUnits::UNROLL* pUnroll = nullptr;

	void additionAssign();
	void dividesAssign();
	void multipliesAssign();

	void determineTypeOfSelection();
	void selectedField();
	void selectedMethod();
	// FM stands for field/method
	std::stack<ShaderUnits::ShaderComponent*> selectedFM_head;
	std::stack<ShaderUnits::ShaderComponent*> selectedFM_tail;

	struct DeclarationFunctionOrVariable
	{
		ShaderUnits::ShaderComponent* modifier = nullptr;
		ShaderUnits::ShaderComponent* type = nullptr;
		std::string name = "";
		ShaderUnits::ShaderComponent* countOfElements = nullptr;
		ShaderUnits::ShaderComponent* signature = nullptr;
		ShaderUnits::ShaderComponent* semantic = nullptr;
		ShaderUnits::ShaderComponent* body = nullptr;
		ShaderUnits::ShaderComponent* value = nullptr;
	};
	std::stack<HLSLConverter::DeclarationFunctionOrVariable> decls;

	void cast();
	void updateCast();

	void countOfElements();
	void insertCountOfElements();

	void texture2dState();

	void samplerStateState();
	void samplerStateName();
	void insertSamplerState();
	void samplerStateBodyOpenBracket();

	struct SamplerStateDefinition
	{
		std::string name = "";
		ShaderUnits::ShaderComponent* body = nullptr;

		void clear() { name = ""; body = nullptr; }
	};
	SamplerStateDefinition samplerStateDecl;

	void forState();
	void forExpressionOpenBracket();
	void forInitExpression();
	void forCondExpression();
	void forLoopExpression();
	void insertForExpression();
	void forBodyOpenBracket();
	void insertForInitExpression();
	void insertForCondExpression();
	void insertForLoopExpression();
	void insertFor();

private:
	bool isOperationState(State state) const;
	bool isCast(const std::queue<std::string>& words) const;
	bool isType(const std::string& str) const;
	bool isModifier(const std::string& str) const;

public:
	HLSLConverter();

	void convertWord(const std::string& word);
	void getShader(HLSLShader& hlslShader);
};