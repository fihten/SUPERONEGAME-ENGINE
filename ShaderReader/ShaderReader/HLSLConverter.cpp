#include "pch.h"
#include "HLSLConverter.h"
#include "StringUtils.h"
#include <utility>

HLSLConverter::HLSLConverter()
{
	words = std::queue<std::string>();
}

void HLSLConverter::convertWord(const std::string& word)
{
	words.push(word);
}

void HLSLConverter::getShader(HLSLShader& hlslShader)
{
	statesStack = std::stack<State>();

	componentStack = std::stack<ShaderUnits::ShaderComponent*>();
	componentStack.push(new ShaderUnits::SHADER());

	currentState = State::UNKNOWN;
	while (!words.empty() || currentState != State::UNKNOWN)
	{
		switch (currentState)
		{
		case State::UNKNOWN:
			unknown();
			break;

		case State::BRACKETS_UNARY_OPERATOR_OPEN:
			bracketsUnaryOperatorOpen();
			break;

		case State::BRACKETS_UNARY_OPERATOR_CLOSE:
			bracketsUnaryOperatorClose();
			break;

		case State::FINISH_EXPRESSION:
			finishExpression();
			break;

		case State::BINARY_MINUS:
			binaryMinus();
			break;

		case State::CREATING_BINARY_MINUS:
			creatingBinaryMinus();
			break;

		case State::BINARY_PLUS:
			binaryPlus();
			break;

		case State::CREATING_BINARY_PLUS:
			creatingBinaryPlus();
			break;

		case State::BINARY_DIVIDE:
			binaryDivide();
			break;

		case State::CREATING_BINARY_DIVIDE:
			creatingBinaryDivide();
			break;

		case State::BINARY_MULTIPLY:
			binaryMultiply();
			break;

		case State::CREATING_BINARY_MULTIPLY:
			creatingBinaryMultiply();
			break;

		case State::GREATER_THAN:
			greaterThan();
			break;

		case State::CREATING_GREATER_THAN:
			creatingGreaterThan();
			break;

		case State::LESS_THAN:
			lessThan();
			break;

		case State::CREATING_LESS_THAN:
			creatingLessThan();
			break;

		case State::VARIABLE:
			variable();
			break;

		case State::NUMBER:
			number();
			break;

		case State::COMMENT:
			comment();
			break;

		case State::UNARY_MINUS:
			unaryMinus();
			break;

		case State::UNARY_PLUS:
			unaryPlus();
			break;

		case State::FUNCTION_CALL:
			functionCall();
			break;

		case State::ARGUMENTS_LIST_OPEN_BRACKET:
			argumentsListOpenBracket();
			break;

		case State::ARGUMENTS_LIST_CLOSE_BRACKET:
			argumentsListCloseBracket();
			break;

		case State::ASSIGNMENT:
			assignment();
			break;

		case State::VOID_:
			voidState();
			break;

		case State::INT1:
			intState();
			break;

		case State::FLOAT1:
			floatState();
			break;

		case State::FLOAT2:
			float2State();
			break;

		case State::FLOAT3:
			float3State();
			break;

		case State::FLOAT4:
			float4State();
			break;

		case State::FLOAT3X3:
			float3x3State();
			break;

		case State::FLOAT4X4:
			float4x4State();
			break;

		case State::TEXTURE2D:
			texture2dState();
			break;

		case State::USER_TYPE:
			userTypeState();
			break;

		case State::CUSTOM_NAME:
			customName();
			break;

		case State::FUNCTION_DECLARATION:
			functionDeclaration();
			break;

		case State::SIGNATURE_OPEN_BRACKET:
			signatureOpenBracket();
			break;

		case State::SIGNATURE_CLOSE_BRACKET:
			signatureCloseBracket();
			break;

		case State::SEMANTIC:
			semantic();
			break;

		case State::SV_POSITION_:
			svPosition();
			break;

		case State::SV_TARGET_:
			svTarget();
			break;

		case State::CUSTOM_SEMANTIC:
			customSemantic();
			break;

		case State::FUNCTION_BODY_OPEN_BRACKET:
			functionBodyOpenBracket();
			break;

		case State::FUNCTION_BODY_CLOSE_BRACKET:
			functionBodyCloseBracket();
			break;

		case State::INSERT_FUNCTION_DECLARATION:
			insertFunctionDeclaration();
			break;

		case State::VARIABLE_DECLARATION:
			variableDeclaration();
			break;

		case State::INSERT_VARIABLE_DECLARATION:
			insertVariableDeclaration();
			break;

		case State::IN_:
			inState();
			break;

		case State::OUT_:
			outState();
			break;

		case State::INOUT_:
			inoutState();
			break;

		case State::UNIFORM_:
			uniformState();
			break;

		case State::MUL:
			mulState();
			break;

		case State::FLOAT3_CONSTRUCTOR:
			float3constructor();
			break;

		case State::FLOAT4_CONSTRUCTOR:
			float4constructor();
			break;

		case State::CBUFFER:
			cbufferState();
			break;

		case State::CBUFFER_NAME:
			cbufferName();
			break;

		case State::INSERT_CBUFFER:
			insertCbuffer();
			break;

		case State::CBUFFER_BODY_OPEN_BRACKET:
			cbufferBodyOpenBracket();
			break;

		case State::SAMPLER_STATE:
			samplerStateState();
			break;

		case State::SAMPLER_STATE_NAME:
			samplerStateName();
			break;

		case State::INSERT_SAMPLER_STATE:
			insertSamplerState();
			break;

		case State::SAMPLER_STATE_BODY_OPEN_BRACKET:
			samplerStateBodyOpenBracket();
			break;

		case State::TECHNIQUE11:
			technique11state();
			break;

		case State::INSERT_TECHNIQUE11:
			insertTechnique11();
			break;

		case State::PASS:
			passState();
			break;

		case State::INSERT_PASS:
			insertPass();
			break;

		case State::SET_VERTEX_SHADER:
			setVertexShaderState();
			break;

		case State::SET_PIXEL_SHADER:
			setPixelShaderState();
			break;

		case State::COMPILE_SHADER:
			compileShader();
			break;

		case State::VS_5_0:
			vs_5_0_state();
			break;

		case State::PS_5_0:
			ps_5_0_state();
			break;

		case State::RETURN:
			returnState();
			break;

		case State::INSERT_RETURN:
			insertReturn();
			break;

		case State::IF:
			ifState();
			break;

		case State::IF_CONDITION:
			ifCondition();
			break;

		case State::INSERT_IF_CONDITION:
			insertIfCondition();
			break;

		case State::IF_BODY:
			ifBody();
			break;

		case State::INSERT_IF_BODY:
			insertIfBody();
			break;

		case State::INSERT_ENTIRE_IF:
			insertEntireIf();
			break;

		case State::ELSE:
			elseState();
			break;

		case ELSE_BODY:
			elseBody();
			break;

		case INSERT_ELSE_BODY:
			insertElseBody();
			break;

		case State::ATTRIBUTE:
			attribute();
			break;

		case State::FLATTEN:
			flatten();
			break;

		case State::UNROLL:
			unroll();
			break;

		case State::ADDITION_ASSIGN:
			additionAssign();
			break;

		case State::DIVIDES_ASSIGN:
			dividesAssign();
			break;

		case State::MULTIPLIES_ASSIGN:
			multipliesAssign();
			break;

		case State::STRUCT:
			structState();
			break;

		case State::STRUCT_NAME:
			structName();
			break;

		case State::STRUCT_BODY_OPEN_BRACKET:
			structBodyOpenBracket();
			break;

		case State::INSERT_STRUCT:
			insertStruct();
			break;

		case State::DETERMINE_TYPE_OF_SELECTION:
			determineTypeOfSelection();
			break;

		case State::SELECTED_FIELD:
			selectedField();
			break;

		case State::SELECTED_METHOD:
			selectedMethod();
			break;

		case State::CAST:
			cast();
			break;

		case State::UPDATE_CAST:
			updateCast();
			break;

		case State::COUNT_OF_ELEMENTS:
			countOfElements();
			break;

		case State::INSERT_COUNT_OF_ELEMENTS:
			insertCountOfElements();
			break;

		case State::FOR:
			forState();
			break;

		case State::FOR_EXPRESSION_OPEN_BRACKET:
			forExpressionOpenBracket();
			break;

		case State::FOR_INIT_EXPRESSION:
			forInitExpression();
			break;

		case State::FOR_COND_EXPRESSION:
			forCondExpression();
			break;

		case State::FOR_LOOP_EXPRESSION:
			forLoopExpression();
			break;

		case State::INSERT_FOR_EXPRESSION:
			insertForExpression();
			break;

		case State::FOR_BODY_OPEN_BRACKET:
			forBodyOpenBracket();
			break;

		case State::INSERT_FOR_INIT_EXPRESSION:
			insertForInitExpression();
			break;

		case State::INSERT_FOR_COND_EXPRESSION:
			insertForCondExpression();
			break;

		case State::INSERT_FOR_LOOP_EXPRESSION:
			insertForLoopExpression();
			break;

		case State::INSERT_FOR:
			insertFor();
			break;
		}
	}

	hlslShader.shader = dynamic_cast<ShaderUnits::SHADER*>(componentStack.top());
}

void HLSLConverter::unknown()
{
	std::string word = words.front();

	if (isCast(words))
	{
		words.pop();
		currentState = State::CAST;

		return;
	}

	// open round brackets
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::BRACKETS_UNARY_OPERATOR_OPEN;

		return;
	}
	if (word == std::string("["))
	{
		words.pop();
		currentState = State::ATTRIBUTE;

		return;
	}
	if (word == std::string(")") && !statesStack.empty() && statesStack.top() == State::BRACKETS_UNARY_OPERATOR_OPEN)
	{
		words.pop();
		currentState = State::BRACKETS_UNARY_OPERATOR_CLOSE;

		return;
	}
	if (word == std::string(")") && !statesStack.empty() && statesStack.top() == State::ARGUMENTS_LIST_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::ARGUMENTS_LIST_CLOSE_BRACKET;

		return;
	}
	if (word == std::string(")") && !statesStack.empty() && statesStack.top() == State::SIGNATURE_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::SIGNATURE_CLOSE_BRACKET;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::FUNCTION_BODY_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_CLOSE_BRACKET;

		return;
	}
	if ((word == std::string(";") || word == std::string(",") || word == std::string(")"))
		&& !statesStack.empty() && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
	if (word == std::string("-"))
	{
		words.pop();
		currentState = State::UNARY_MINUS;

		return;
	}
	if (word == std::string("+"))
	{
		words.pop();
		currentState = State::UNARY_PLUS;

		return;
	}
	if (word == std::string("void"))
	{
		words.pop();
		currentState = State::VOID_;

		return;
	}
	if (word == std::string("int"))
	{
		words.pop();
		currentState = State::INT1;

		return;
	}
	if (word == std::string("float"))
	{
		words.pop();
		currentState = State::FLOAT1;

		return;
	}
	if (word == std::string("float2"))
	{
		words.pop();
		currentState = State::FLOAT2;

		return;
	}
	if (word == std::string("float3"))
	{
		words.pop();
		currentState = State::FLOAT3;

		return;
	}
	if (word == std::string("float4"))
	{
		words.pop();
		currentState = State::FLOAT4;

		return;
	}
	if (word == std::string("float3x3"))
	{
		words.pop();
		currentState = State::FLOAT3X3;

		return;
	}
	if (word == std::string("float4x4"))
	{
		words.pop();
		currentState = State::FLOAT4X4;

		return;
	}
	if (word == std::string("Texture2D"))
	{
		words.pop();
		currentState = State::TEXTURE2D;

		return;
	}
	if (userTypes.count(word))
	{
		words.pop();
		currentState = State::USER_TYPE;

		userType = userTypes[word];

		return;
	}
	if (word == std::string("in"))
	{
		words.pop();
		currentState = State::IN_;

		return;
	}
	if (word == std::string("out"))
	{
		words.pop();
		currentState = State::OUT_;

		return;
	}
	if (word == std::string("inout"))
	{
		words.pop();
		currentState = State::INOUT_;

		return;
	}
	if (word == std::string("uniform"))
	{
		words.pop();
		currentState = State::UNIFORM_;

		return;
	}
	if (word == std::string("mul"))
	{
		words.pop();
		currentState = State::MUL;

		return;
	}
	if (word == std::string("cbuffer"))
	{
		words.pop();
		currentState = State::CBUFFER;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::CBUFFER_BODY_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::INSERT_CBUFFER;

		return;
	}
	if (word == std::string("SamplerState"))
	{
		words.pop();
		currentState = State::SAMPLER_STATE;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::SAMPLER_STATE_BODY_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::INSERT_SAMPLER_STATE;

		return;
	}
	if (word == std::string("struct"))
	{
		words.pop();
		currentState = State::STRUCT;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::STRUCT_BODY_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::INSERT_STRUCT;

		return;
	}
	if (word == std::string("technique11"))
	{
		words.pop();
		currentState = State::TECHNIQUE11;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::TECHNIQUE11)
	{
		words.pop();
		currentState = State::INSERT_TECHNIQUE11;

		return;
	}
	if (word == std::string("pass"))
	{
		words.pop();
		currentState = State::PASS;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::PASS)
	{
		words.pop();
		currentState = State::INSERT_PASS;

		return;
	}
	if (word == std::string("SetVertexShader"))
	{
		words.pop();
		currentState = State::SET_VERTEX_SHADER;

		return;
	}
	if (word == std::string("SetPixelShader"))
	{
		words.pop();
		currentState = State::SET_PIXEL_SHADER;

		return;
	}
	if (word == std::string("CompileShader"))
	{
		words.pop();
		currentState = State::COMPILE_SHADER;

		return;
	}
	if (word == std::string("vs_5_0"))
	{
		words.pop();
		currentState = State::VS_5_0;

		return;
	}
	if (word == std::string("ps_5_0"))
	{
		words.pop();
		currentState = State::PS_5_0;

		return;
	}
	if (word == std::string("return"))
	{
		words.pop();
		currentState = State::RETURN;

		return;
	}
	if (word == std::string(";") && !statesStack.empty() && statesStack.top() == State::RETURN)
	{
		currentState = State::INSERT_RETURN;
		return;
	}
	if (word == std::string("if"))
	{
		words.pop();
		currentState = State::IF;

		return;
	}
	if (word == std::string(")") && !statesStack.empty() && statesStack.top() == State::IF_CONDITION)
	{
		words.pop();
		currentState = State::INSERT_IF_CONDITION;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::IF_BODY)
	{
		words.pop();
		currentState = State::INSERT_IF_BODY;

		return;
	}
	if (word == std::string(";") && !statesStack.empty() && statesStack.top() == State::IF)
	{
		words.pop();
		currentState = State::INSERT_IF_BODY;

		return;
	}
	if (word == std::string(";") && !statesStack.empty() && statesStack.top() == State::ELSE)
	{
		words.pop();
		currentState = INSERT_ELSE_BODY;

		return;
	}
	if (word == std::string("}") && !statesStack.empty() && statesStack.top() == State::ELSE_BODY)
	{
		words.pop();
		currentState = INSERT_ELSE_BODY;

		return;
	}
	if (word == std::string("for"))
	{
		words.pop();
		currentState = State::FOR;

		return;
	}
	if (word == std::string(";") && !statesStack.empty() && statesStack.top() == State::FOR_INIT_EXPRESSION)
	{
		words.pop();
		currentState = State::INSERT_FOR_INIT_EXPRESSION;

		return;
	}
	if (word == std::string(";") && !statesStack.empty() && statesStack.top() == State::FOR_COND_EXPRESSION)
	{
		words.pop();
		currentState = State::INSERT_FOR_COND_EXPRESSION;

		return;
	}
	if (word == std::string(")") && !statesStack.empty() && statesStack.top() == State::FOR_LOOP_EXPRESSION)
	{
		words.pop();
		currentState = State::INSERT_FOR_LOOP_EXPRESSION;

		return;
	}
	if (
		(word == std::string("}") && !statesStack.empty() &&
			statesStack.top() == State::FOR_BODY_OPEN_BRACKET)
		||
		(word == std::string(";") && !statesStack.empty() &&
			statesStack.top() == State::FOR)
		)
	{
		words.pop();
		currentState = State::INSERT_FOR;

		return;
	}
	if (word == std::string("]") && !statesStack.empty() && statesStack.top() == State::COUNT_OF_ELEMENTS)
	{
		words.pop();
		currentState = INSERT_COUNT_OF_ELEMENTS;

		return;
	}
	if (word == std::string(";") || word == std::string(",") || word == std::string(")") || word == std::string("}"))
	{
		words.pop();
		currentState = State::UNKNOWN;

		return;
	}

	userName = word;
	words.pop();
	word = std::string("");
	if (!words.empty())
		word = words.front();
	if (word != std::string("(") && !isNumber(userName.c_str()) && !isComment(userName.c_str()))
	{
		currentState = State::VARIABLE;
		return;
	}
	if (word != std::string("(") && isNumber(userName.c_str()))
	{
		currentState = State::NUMBER;
		return;
	}
	if (isComment(userName.c_str()))
	{
		currentState = State::COMMENT;
		return;
	}
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::FUNCTION_CALL;
		return;
	}
}

void HLSLConverter::bracketsUnaryOperatorOpen()
{
	statesStack.push(State::BRACKETS_UNARY_OPERATOR_OPEN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::bracketsUnaryOperatorClose()
{
	std::string word = words.front();

	statesStack.pop();
	if (word == std::string(";") || word == std::string(",") || word == std::string(")") || word == std::string(":"))
		currentState = State::FINISH_EXPRESSION;
	if (word == std::string("+"))
		currentState = State::BINARY_PLUS;
	if (word == std::string("-"))
		currentState = State::BINARY_MINUS;
	if (word == std::string("*"))
		currentState = State::BINARY_MULTIPLY;
	if (word == std::string("/"))
		currentState = State::BINARY_DIVIDE;
	if (word == std::string(">"))
		currentState = State::GREATER_THAN;
	if (word == std::string("<"))
		currentState = State::LESS_THAN;
}

void HLSLConverter::finishExpression()
{
	std::string word = words.front();
	bool op = false;
	if (!statesStack.empty())
		op = isOperationState(statesStack.top());

	if (op)
	{
		ShaderUnits::ShaderComponent* operand = componentStack.top();
		componentStack.pop();

		ShaderUnits::ShaderComponent* operation = componentStack.top();
		operation->add(operand);
	}

	if (op && 
		(
			word == std::string(";") ||
			word == std::string(",") ||
			word == std::string(")") ||
			word == std::string("]")
			))
	{
		statesStack.pop();
		return;
	}

	if (op && word == std::string("+"))
	{
		statesStack.pop();
		currentState = State::BINARY_PLUS;
		return;
	}
	if (op && word == std::string("-"))
	{
		statesStack.pop();
		currentState = State::BINARY_MINUS;
		return;
	}
	if (op && word == std::string("*"))
	{
		statesStack.pop();
		currentState = State::BINARY_MULTIPLY;
		return;
	}
	if (op && word == std::string("/"))
	{
		statesStack.pop();
		currentState = State::BINARY_DIVIDE;
		return;
	}
	if (op && word == std::string(">"))
	{
		statesStack.pop();
		currentState = State::GREATER_THAN;
		return;
	}
	if (op && word == std::string("<"))
	{
		statesStack.pop();
		currentState = State::LESS_THAN;
		return;
	}

	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	currentState = State::UNKNOWN;
	if ((word != std::string(")") || lastState != State::BRACKETS_UNARY_OPERATOR_OPEN) &&
		((word != std::string(")") && word != std::string(";") && word != std::string(",")) || lastState != State::VARIABLE_DECLARATION))
	{
		ShaderUnits::ShaderComponent* componentToAdd = componentStack.top();
		componentStack.pop();

		ShaderUnits::ShaderComponent* composite = componentStack.top();
		composite->add(componentToAdd);
	}
	if ((word == std::string(")") || word == std::string(";") || word == std::string(",")) && lastState == State::VARIABLE_DECLARATION)
	{
		decls.top().value = componentStack.top();
		componentStack.pop();
	}
}

bool HLSLConverter::isOperationState(State state) const
{
	if (state == State::BINARY_DIVIDE ||
		state == State::BINARY_MINUS ||
		state == State::BINARY_MULTIPLY ||
		state == State::BINARY_PLUS ||
		state == State::UNARY_MINUS ||
		state == State::UNARY_PLUS ||
		state == State::CAST ||
		state == State::GREATER_THAN ||
		state == State::LESS_THAN ||
		state == State::ASSIGNMENT ||
		state == State::ADDITION_ASSIGN ||
		state == State::DIVIDES_ASSIGN ||
		state == State::MULTIPLIES_ASSIGN
		)
		return true;

	return false;
}

bool HLSLConverter::isCast(const std::queue<std::string>& words) const
{
	if (words.size() < 3)
		return false;
	auto it = words._Get_container().begin();
	if (std::strcmp(it->c_str(), "(") != 0)
		return false;
	if (!isType(*(it + 1)))
		return false;
	if (std::strcmp((it + 2)->c_str(), ")") != 0)
		return false;
	return true;
}

bool HLSLConverter::isType(const std::string& str) const
{
	if (userTypes.count(str))
		return true;
	if (std::strcmp(str.c_str(), "int"))
		return true;
	if (std::strcmp(str.c_str(), "float"))
		return true;
	if (std::strcmp(str.c_str(), "float2"))
		return true;
	if (std::strcmp(str.c_str(), "float3"))
		return true;
	if (std::strcmp(str.c_str(), "float4"))
		return true;
	if (std::strcmp(str.c_str(), "float3x3"))
		return true;
	if (std::strcmp(str.c_str(), "float4x4"))
		return true;
	return false;
}

void HLSLConverter::binaryMinus()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE ||
		lastState == State::BINARY_MULTIPLY ||
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_BINARY_MINUS;
}

void HLSLConverter::creatingBinaryMinus()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* binaryMinusOp = new ShaderUnits::BINARY_MINUS();
	binaryMinusOp->add(leftOperand);

	componentStack.push(binaryMinusOp);

	statesStack.push(State::BINARY_MINUS);
	currentState = State::UNKNOWN;
}

void HLSLConverter::binaryPlus()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE ||
		lastState == State::BINARY_MULTIPLY ||
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_BINARY_PLUS;
}

void HLSLConverter::creatingBinaryPlus()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* binaryPlusOp = new ShaderUnits::BINARY_PLUS();
	binaryPlusOp->add(leftOperand);

	componentStack.push(binaryPlusOp);

	statesStack.push(State::BINARY_PLUS);
	currentState = State::UNKNOWN;
}

void HLSLConverter::binaryDivide()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = CREATING_BINARY_DIVIDE;
}

void HLSLConverter::creatingBinaryDivide()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* binaryDivideOp = new ShaderUnits::BINARY_DIVIDE();
	binaryDivideOp->add(leftOperand);

	componentStack.push(binaryDivideOp);

	statesStack.push(State::BINARY_DIVIDE);
	currentState = State::UNKNOWN;
}

void HLSLConverter::binaryMultiply()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE ||
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_BINARY_MULTIPLY;
}

void HLSLConverter::creatingBinaryMultiply()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* binaryMultiplyOp = new ShaderUnits::BINARY_MULTIPLY();
	binaryMultiplyOp->add(leftOperand);

	componentStack.push(binaryMultiplyOp);

	statesStack.push(State::BINARY_MULTIPLY);
	currentState = State::UNKNOWN;
}

void HLSLConverter::greaterThan()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE ||
		lastState == State::BINARY_MULTIPLY ||
		lastState == State::BINARY_MINUS ||
		lastState == State::BINARY_PLUS ||
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_GREATER_THAN;
}

void HLSLConverter::creatingGreaterThan()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* greaterThanOp = new ShaderUnits::GREATER_THAN();
	greaterThanOp->add(leftOperand);

	componentStack.push(greaterThanOp);

	statesStack.push(State::GREATER_THAN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::lessThan()
{
	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE ||
		lastState == State::BINARY_MULTIPLY ||
		lastState == State::BINARY_MINUS ||
		lastState == State::BINARY_PLUS ||
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_LESS_THAN;
}

void HLSLConverter::creatingLessThan()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* lessThanOp = new ShaderUnits::LESS_THAN();
	lessThanOp->add(leftOperand);

	componentStack.push(lessThanOp);

	statesStack.push(State::LESS_THAN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::variable()
{
	// It is hack
	if (words.empty())
	{
		currentState = State::UNKNOWN;
		return;
	}

	if (userName[0] == '.')
	{
		currentState = State::DETERMINE_TYPE_OF_SELECTION;
		userName = userName.substr(1);
		return;
	}

	if (!userName.empty())
	{
		ShaderUnits::ShaderComponent* var = new ShaderUnits::VARIABLE();

		std::string varName = userName;
		var->setName(varName);
		componentStack.push(var);

		int pos = varName.find(".", 0);
		if (pos != varName.npos)
		{
			varName = varName.substr(0, pos);
			userName = userName.substr(pos + 1);

			var->setName(varName);

			selectedFM_head.push(nullptr);
			selectedFM_tail.push(nullptr);

			currentState = State::DETERMINE_TYPE_OF_SELECTION;
			return;
		}

		userName = std::string("");
	}
	else
	{
		ShaderUnits::ShaderComponent* var = componentStack.top();
		var->add(selectedFM_head.top());
		selectedFM_head.pop();
		selectedFM_tail.pop();
	}

	std::string word = words.front();
	if (word == std::string(",") || word == std::string(";") ||
		word == std::string(")") || word == std::string(":") ||
		word == std::string("}") || word == std::string("]"))
	{
		currentState = State::FINISH_EXPRESSION;
	}
	if (word == std::string("-"))
		currentState = State::BINARY_MINUS;
	if (word == std::string("+"))
		currentState = State::BINARY_PLUS;
	if (word == std::string("*"))
		currentState = State::BINARY_MULTIPLY;
	if (word == std::string("/"))
		currentState = State::BINARY_DIVIDE;
	if (word == std::string("="))
		currentState = State::ASSIGNMENT;
	if (word == std::string("+="))
		currentState = State::ADDITION_ASSIGN;
	if (word == std::string("/="))
		currentState = State::DIVIDES_ASSIGN;
	if (word == std::string("*="))
		currentState = State::MULTIPLIES_ASSIGN;
	if (word == std::string(">"))
		currentState = State::GREATER_THAN;
	if (word == std::string("<"))
		currentState = State::LESS_THAN;
}

void HLSLConverter::number()
{
	ShaderUnits::ShaderComponent* var = new ShaderUnits::NUMBER();
	var->setName(userName);
	userName = std::string("");

	componentStack.push(var);

	std::string word = words.front();
	if (word == std::string(",") || word == std::string(";") ||
		word == std::string(")") || word == std::string(":") ||
		word == std::string("}") || word == std::string("]"))
		currentState = State::FINISH_EXPRESSION;
	if (word == std::string("-"))
		currentState = State::BINARY_MINUS;
	if (word == std::string("+"))
		currentState = State::BINARY_PLUS;
	if (word == std::string("*"))
		currentState = State::BINARY_MULTIPLY;
	if (word == std::string("/"))
		currentState = State::BINARY_DIVIDE;
	if (word == std::string("="))
		currentState = State::ASSIGNMENT;
	if (word == std::string("+="))
		currentState = State::ADDITION_ASSIGN;
	if (word == std::string("/="))
		currentState = State::DIVIDES_ASSIGN;
	if (word == std::string("*="))
		currentState = State::MULTIPLIES_ASSIGN;
	if (word == std::string(">"))
		currentState = State::GREATER_THAN;
	if (word == std::string("<"))
		currentState = State::LESS_THAN;
}

void HLSLConverter::comment()
{
	ShaderUnits::ShaderComponent* var = new ShaderUnits::COMMENT();
	var->setName(userName);
	userName = std::string("");

	componentStack.top()->add(var);
	currentState = State::UNKNOWN;
}

void HLSLConverter::unaryMinus()
{
	statesStack.push(State::UNARY_MINUS);
	currentState = State::UNKNOWN;

	ShaderUnits::ShaderComponent* unaryMinusOp = new ShaderUnits::UNARY_MINUS();
	componentStack.push(unaryMinusOp);
}

void HLSLConverter::unaryPlus()
{
	statesStack.push(State::UNARY_PLUS);
	currentState = State::UNKNOWN;

	ShaderUnits::ShaderComponent* unaryPlusOp = new ShaderUnits::UNARY_PLUS();
	componentStack.push(unaryPlusOp);
}

void HLSLConverter::functionCall()
{
	statesStack.push(State::FUNCTION_CALL);

	if (userName[0] == '.')
	{
		userName = userName.substr(1);
		currentState = State::DETERMINE_TYPE_OF_SELECTION;
		return;
	}

	if (userName.find('.') != userName.npos)
	{
		currentState = State::VARIABLE;
		return;
	}

	ShaderUnits::ShaderComponent* functionCallOp = new ShaderUnits::FUNCTION_CALL();
	functionCallOp->setName(userName);
	userName = std::string("");

	componentStack.push(functionCallOp);

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;
}

void HLSLConverter::argumentsListOpenBracket()
{
	ShaderUnits::ShaderComponent* arguments = new ShaderUnits::ROUND_BRACKETS();
	componentStack.push(arguments);

	statesStack.push(State::ARGUMENTS_LIST_OPEN_BRACKET);
	currentState = State::UNKNOWN;
}

void HLSLConverter::argumentsListCloseBracket()
{
	std::string word = words.front();

	// Define if we have selected method
	if (selectedFM_tail.top() != nullptr)
	{
		statesStack.pop();
		statesStack.pop();

		ShaderUnits::ShaderComponent* arguments = componentStack.top();
		componentStack.pop();

		ShaderUnits::ShaderComponent* func = selectedFM_tail.top();
		func->add(arguments);

		if (word[0] == '.')
		{
			currentState = State::UNKNOWN;
			return;
		}

		if (statesStack.top() == State::ARGUMENTS_LIST_CLOSE_BRACKET)
		{
			selectedFM_tail.top() = nullptr;
			currentState = State::ARGUMENTS_LIST_CLOSE_BRACKET;
			statesStack.pop();
			return;
		}

		currentState = State::VARIABLE;
		return;
	}

	if (word[0] == '.')
	{
		statesStack.push(State::ARGUMENTS_LIST_CLOSE_BRACKET);
		currentState = State::UNKNOWN;
		return;
	}

	statesStack.pop();
	statesStack.pop();

	ShaderUnits::ShaderComponent* arguments = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* func = componentStack.top();
	func->add(arguments);

	if (selectedFM_head.top() != nullptr)
		func->add(selectedFM_head.top());
	selectedFM_head.pop();
	selectedFM_tail.pop();

	if (word == std::string(",") || word == std::string(";") || word == std::string(")"))
		currentState = State::FINISH_EXPRESSION;
	if (word == std::string("+"))
		currentState = State::BINARY_PLUS;
	if (word == std::string("-"))
		currentState = State::BINARY_MINUS;
	if (word == std::string("*"))
		currentState = State::BINARY_MULTIPLY;
	if (word == std::string("/"))
		currentState = State::BINARY_DIVIDE;
	if (word == std::string(">"))
		currentState = State::GREATER_THAN;
	if (word == std::string("<"))
		currentState = State::LESS_THAN;
}

void HLSLConverter::assignment()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftValue = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* assignmentOp = new ShaderUnits::ASSIGNMENT();
	assignmentOp->add(leftValue);
	componentStack.push(assignmentOp);

	statesStack.push(State::ASSIGNMENT);
	currentState = State::UNKNOWN;
}

void HLSLConverter::voidState()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::VOID_();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void HLSLConverter::intState()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::INT1* type = new ShaderUnits::INT1();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::INT1();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void HLSLConverter::floatState()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT1* type = new ShaderUnits::FLOAT1();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT1();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void HLSLConverter::float2State()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT2* type = new ShaderUnits::FLOAT2();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT2();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void HLSLConverter::float3State()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT3* type = new ShaderUnits::FLOAT3();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT3();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::FLOAT3_CONSTRUCTOR;

		return;
	}
}

void HLSLConverter::float4State()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT4* type = new ShaderUnits::FLOAT4();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT4();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		return;
	}
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::FLOAT4_CONSTRUCTOR;

		return;
	}
}

void HLSLConverter::float3x3State()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT3X3* type = new ShaderUnits::FLOAT3X3();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT3X3();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		return;
	}
}

void HLSLConverter::float4x4State()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		ShaderUnits::FLOAT4X4* type = new ShaderUnits::FLOAT4X4();
		componentStack.push(type);

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = new ShaderUnits::FLOAT4X4();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		return;
	}
}

void HLSLConverter::userTypeState()
{
	std::string word = words.front();
	if (!statesStack.empty() && statesStack.top() == State::CAST)
	{
		words.pop();

		componentStack.push(userType);
		userType = nullptr;

		currentState = State::UPDATE_CAST;

		return;
	}
	if (word != std::string("("))
	{
		words.pop();

		HLSLConverter::DeclarationFunctionOrVariable decl;

		decl.type = userType;
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		userType = nullptr;

		return;
	}
}

void HLSLConverter::customName()
{
	std::string word = words.front();
	if (word == std::string("("))
	{
		currentState = State::FUNCTION_DECLARATION;
		return;
	}

	currentState = State::VARIABLE_DECLARATION;
	return;
}

void HLSLConverter::functionDeclaration()
{
	std::string word = words.front();
	if (word == std::string("("))
	{
		words.pop();

		statesStack.push(State::FUNCTION_DECLARATION);
		currentState = State::SIGNATURE_OPEN_BRACKET;

		return;
	}
}

void HLSLConverter::signatureOpenBracket()
{
	ShaderUnits::ShaderComponent* signature = new ShaderUnits::ROUND_BRACKETS();
	decls.top().signature = signature;
	componentStack.push(signature);

	statesStack.push(State::SIGNATURE_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::signatureCloseBracket()
{
	statesStack.pop();

	std::string word = words.front();
	componentStack.pop();

	if (word == std::string(":"))
	{
		words.pop();
		currentState = State::SEMANTIC;
		return;
	}
	if (word == std::string(";"))
	{
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
}

void HLSLConverter::semantic()
{
	std::string word = words.front();
	if (word == std::string("SV_POSITION"))
	{
		words.pop();
		currentState = State::SV_POSITION_;
		return;
	}
	if (word == std::string("SV_TARGET"))
	{
		words.pop();
		currentState = State::SV_TARGET_;
		return;
	}
	// CUSTOM_SEMANTIC
	{
		words.pop();
		userName = word;
		currentState = State::CUSTOM_SEMANTIC;
		return;
	}
}

void HLSLConverter::svPosition()
{
	std::string word = words.front();

	ShaderUnits::ShaderComponent* semanticElement = new ShaderUnits::SV_POSITION();
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == std::string("{") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == std::string(";") || word == std::string(",") || word == std::string(")")) && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void HLSLConverter::svTarget()
{
	std::string word = words.front();

	ShaderUnits::ShaderComponent* semanticElement = new ShaderUnits::SV_TARGET();
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == std::string("{") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == std::string(";") || word == std::string(",") || word == std::string(")")) && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void HLSLConverter::customSemantic()
{
	std::string word = words.front();

	ShaderUnits::ShaderComponent* semanticElement = new ShaderUnits::SEMANTIC();
	semanticElement->setName(userName);
	userName = std::string("");
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == std::string("{") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == std::string(";") || word == std::string(",") || word == std::string(")")) && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void HLSLConverter::functionBodyOpenBracket()
{
	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	decls.top().body = body;
	componentStack.push(body);

	currentState = State::UNKNOWN;
	statesStack.push(State::FUNCTION_BODY_OPEN_BRACKET);
}

void HLSLConverter::functionBodyCloseBracket()
{
	componentStack.pop();

	statesStack.pop();
	statesStack.pop();

	DeclarationFunctionOrVariable& funcDeclStruct = decls.top();

	ShaderUnits::FUNCTION_DECL* funcDecl = new ShaderUnits::FUNCTION_DECL();
	funcDecl->add(funcDeclStruct.type);
	funcDecl->setName(funcDeclStruct.name);
	funcDecl->add(funcDeclStruct.signature);
	if (funcDeclStruct.semantic)
		funcDecl->add(funcDeclStruct.semantic);
	if (funcDeclStruct.body)
		funcDecl->add(funcDeclStruct.body);

	decls.pop();
	componentStack.top()->add(funcDecl);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertFunctionDeclaration()
{
	statesStack.pop();

	DeclarationFunctionOrVariable& funcDeclStruct = decls.top();

	ShaderUnits::FUNCTION_DECL* funcDecl = new ShaderUnits::FUNCTION_DECL();
	funcDecl->add(funcDeclStruct.type);
	funcDecl->setName(funcDeclStruct.name);
	funcDecl->add(funcDeclStruct.signature);
	if (funcDeclStruct.semantic)
		funcDecl->add(funcDeclStruct.semantic);
	if (funcDeclStruct.body)
		funcDecl->add(funcDeclStruct.body);

	decls.pop();
	componentStack.top()->add(funcDecl);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::variableDeclaration()
{
	if (modifier)
	{
		decls.top().modifier = modifier;
		modifier = nullptr;
	}

	std::string word = words.front();
	statesStack.push(State::VARIABLE_DECLARATION);
	if (word == std::string("="))
	{
		words.pop();
		currentState = State::UNKNOWN;
		return;
	}
	if (word == std::string(":"))
	{
		words.pop();
		currentState = State::SEMANTIC;
		return;
	}
	if (word == std::string(";") || word == std::string(")") || word == std::string(","))
	{
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
	if (word == std::string("["))
	{
		words.pop();
		currentState = State::COUNT_OF_ELEMENTS;
		return;
	}
}

void HLSLConverter::insertVariableDeclaration()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pVariableDeclaration = new ShaderUnits::VARIABLE_DECL();

	if (decls.top().modifier)
		pVariableDeclaration->add(decls.top().modifier);
	pVariableDeclaration->add(decls.top().type);
	pVariableDeclaration->setName(decls.top().name);
	if (decls.top().semantic)
		pVariableDeclaration->add(decls.top().semantic);
	if (decls.top().countOfElements)
		pVariableDeclaration->add(decls.top().countOfElements);
	if (decls.top().value)
		pVariableDeclaration->add(decls.top().value);

	decls.pop();

	componentStack.top()->add(pVariableDeclaration);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::inState()
{
	modifier = new ShaderUnits::IN_MODIFIER();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::outState()
{
	modifier = new ShaderUnits::OUT_MODIFIER();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::inoutState()
{
	modifier = new ShaderUnits::INOUT_MODIFIER();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::uniformState()
{
	modifier = new ShaderUnits::UNIFORM();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::mulState()
{
	statesStack.push(State::MUL);

	ShaderUnits::ShaderComponent* pMul = new ShaderUnits::MUL();
	componentStack.push(pMul);

	std::string word = words.front();
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

		selectedFM_head.push(nullptr);
		selectedFM_tail.push(nullptr);

		return;
	}
}

void HLSLConverter::float3constructor()
{
	ShaderUnits::ShaderComponent* pFloat3Constructor = new ShaderUnits::FLOAT3_CONSTRUCTOR();
	componentStack.push(pFloat3Constructor);

	statesStack.push(State::FLOAT3_CONSTRUCTOR);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	return;
}

void HLSLConverter::float4constructor()
{
	ShaderUnits::ShaderComponent* pFloat4Constructor = new ShaderUnits::FLOAT4_CONSTRUCTOR();
	componentStack.push(pFloat4Constructor);

	statesStack.push(State::FLOAT4_CONSTRUCTOR);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	return;
}

void HLSLConverter::cbufferState()
{
	statesStack.push(State::CBUFFER);
	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::CBUFFER_BODY_OPEN_BRACKET;
		return;
	}
	// non-predefined word
	{
		currentState = State::CBUFFER_NAME;
		return;
	}
}

void HLSLConverter::cbufferName()
{
	std::string cbufferName = words.front();
	words.pop();

	cbufferDecl.name = cbufferName;

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::CBUFFER_BODY_OPEN_BRACKET;

		return;
	}
	if (word == std::string(";"))
	{
		currentState = State::INSERT_CBUFFER;
		return;
	}
}

void HLSLConverter::insertCbuffer()
{
	if (statesStack.top() == State::CBUFFER_BODY_OPEN_BRACKET)
		statesStack.pop();
	if (statesStack.top() == State::CBUFFER)
		statesStack.pop();

	ShaderUnits::ShaderComponent* pCbuffer = new ShaderUnits::CBUFFER();
	if (cbufferDecl.name != std::string(""))
		pCbuffer->setName(cbufferDecl.name);
	if (cbufferDecl.body)
		pCbuffer->add(cbufferDecl.body);
	cbufferDecl.clear();

	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pCbuffer);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::cbufferBodyOpenBracket()
{
	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	cbufferDecl.body = body;
	componentStack.push(body);

	statesStack.push(State::CBUFFER_BODY_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::technique11state()
{
	std::string techName = words.front();
	words.pop();
	words.pop();

	tech = new ShaderUnits::TECHNIQUE11();
	tech->setName(techName);

	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	tech->add(body);

	componentStack.push(body);
	statesStack.push(State::TECHNIQUE11);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertTechnique11()
{
	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(tech);
	tech = nullptr;

	statesStack.pop();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::passState()
{
	std::string passName = words.front();
	words.pop();
	words.pop();

	pass = new ShaderUnits::PASS();
	pass->setName(passName);

	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	pass->add(body);

	componentStack.push(body);
	statesStack.push(State::PASS);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertPass()
{
	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pass);
	pass = nullptr;

	statesStack.pop();
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::setVertexShaderState()
{
	ShaderUnits::ShaderComponent* pSetVertexShader = new ShaderUnits::SET_VERTEX_SHADER();
	componentStack.push(pSetVertexShader);

	statesStack.push(State::SET_VERTEX_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	return;
}

void HLSLConverter::setPixelShaderState()
{
	ShaderUnits::ShaderComponent* pSetPixelShader = new ShaderUnits::SET_PIXEL_SHADER();
	componentStack.push(pSetPixelShader);

	statesStack.push(State::SET_PIXEL_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	return;
}

void HLSLConverter::compileShader()
{
	ShaderUnits::ShaderComponent* pCompileShader = new ShaderUnits::COMPILE_SHADER();
	componentStack.push(pCompileShader);

	statesStack.push(State::COMPILE_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	selectedFM_head.push(nullptr);
	selectedFM_tail.push(nullptr);

	return;
}

void HLSLConverter::vs_5_0_state()
{
	ShaderUnits::ShaderComponent* pVS_5_0 = new ShaderUnits::VERTEX_SHADER_VERSION();
	pVS_5_0->setName(std::string("vs_5_0"));

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pVS_5_0);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::ps_5_0_state()
{
	ShaderUnits::ShaderComponent* pPS_5_0 = new ShaderUnits::PIXEL_SHADER_VERSION();
	pPS_5_0->setName(std::string("ps_5_0"));

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pPS_5_0);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::returnState()
{
	std::string word = words.front();

	ShaderUnits::ShaderComponent* pReturn = new ShaderUnits::RETURN();
	componentStack.push(pReturn);

	statesStack.push(State::RETURN);

	if (word == std::string(";"))
	{
		currentState = State::INSERT_RETURN;
		return;
	}

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertReturn()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pReturn = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pReturn);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::ifState()
{
	ShaderUnits::ShaderComponent* pIf = new ShaderUnits::IF_NODE();
	if (pFlatten)
	{
		pIf->add(pFlatten);
		pFlatten = nullptr;
	}

	componentStack.push(pIf);
	statesStack.push(State::IF);

	words.pop();
	currentState = State::IF_CONDITION;

	return;
}

void HLSLConverter::ifCondition()
{
	ShaderUnits::ShaderComponent* pIfCondition = new ShaderUnits::ROUND_BRACKETS();
	pIfCondition->setName(std::string("if_condition"));
	componentStack.push(pIfCondition);
	statesStack.push(State::IF_CONDITION);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertIfCondition()
{
	ShaderUnits::ShaderComponent* pIfCondition = componentStack.top();
	componentStack.pop();
	statesStack.pop();

	ShaderUnits::ShaderComponent* pIf = componentStack.top();
	pIf->add(pIfCondition);

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::IF_BODY;

		return;
	}
	currentState = State::UNKNOWN;
}

void HLSLConverter::ifBody()
{
	ShaderUnits::ShaderComponent* pIfBody = new ShaderUnits::CURLY_BRACKETS();
	componentStack.push(pIfBody);
	statesStack.push(State::IF_BODY);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertIfBody()
{
	if (!statesStack.empty() && statesStack.top() == State::IF_BODY)
	{
		ShaderUnits::ShaderComponent* pIfBody = componentStack.top();
		componentStack.pop();

		pIfBody->setName(std::string("if_body"));
		ShaderUnits::ShaderComponent* pIf = componentStack.top();
		pIf->add(pIfBody);

		statesStack.pop();
	}

	std::string word = words.front();
	if (word == std::string("else"))
	{
		words.pop();
		currentState = State::ELSE;

		return;
	}

	currentState = State::INSERT_ENTIRE_IF;

	return;
}

void HLSLConverter::insertEntireIf()
{
	ShaderUnits::ShaderComponent* pIf = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pParent = componentStack.top();
	pParent->add(pIf);

	statesStack.pop();

	if (!statesStack.empty() && statesStack.top() == State::IF)
	{
		currentState = INSERT_IF_BODY;
		return;
	}
	if (!statesStack.empty() && statesStack.top() == State::ELSE)
	{
		currentState = INSERT_ELSE_BODY;
		return;
	}

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::elseState()
{
	statesStack.push(State::ELSE);

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::ELSE_BODY;

		return;
	}
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::elseBody()
{
	ShaderUnits::ShaderComponent* pElseBody = new ShaderUnits::CURLY_BRACKETS();

	componentStack.push(pElseBody);
	statesStack.push(State::ELSE_BODY);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertElseBody()
{
	State prevState = statesStack.top();
	statesStack.pop();
	if (prevState == State::ELSE_BODY)
	{
		statesStack.pop();

		ShaderUnits::ShaderComponent* pElseBody = componentStack.top();
		componentStack.pop();

		pElseBody->setName("else_body");
		ShaderUnits::ShaderComponent* pIf = componentStack.top();
		pIf->add(pElseBody);
	}

	currentState = State::INSERT_ENTIRE_IF;

	return;
}

void HLSLConverter::attribute()
{
	std::string word = words.front();
	if (word == std::string("flatten"))
	{
		words.pop();
		currentState = State::FLATTEN;

		return;
	}
	if (word == std::string("unroll"))
	{
		words.pop();
		currentState = State::UNROLL;

		return;
	}
}

void HLSLConverter::flatten()
{
	pFlatten = new ShaderUnits::FLATTEN();
	currentState = State::UNKNOWN;
	words.pop();
}

void HLSLConverter::unroll()
{
	pUnroll = new ShaderUnits::UNROLL();
	currentState = State::UNKNOWN;
	words.pop();
}

void HLSLConverter::additionAssign()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftValue = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* additionAssignOp = new ShaderUnits::ADDITION_ASSIGN();
	additionAssignOp->add(leftValue);
	componentStack.push(additionAssignOp);

	statesStack.push(State::ADDITION_ASSIGN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::dividesAssign()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftValue = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* dividesAssignOp = new ShaderUnits::DIVIDES_ASSIGN();
	dividesAssignOp->add(leftValue);
	componentStack.push(dividesAssignOp);

	statesStack.push(State::DIVIDES_ASSIGN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::multipliesAssign()
{
	words.pop();

	ShaderUnits::ShaderComponent* leftValue = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* multipliesAssignOp = new ShaderUnits::MULTIPLIES_ASSIGN();
	multipliesAssignOp->add(leftValue);
	componentStack.push(multipliesAssignOp);

	statesStack.push(State::MULTIPLIES_ASSIGN);
	currentState = State::UNKNOWN;
}

void HLSLConverter::structState()
{
	statesStack.push(State::STRUCT);
	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::STRUCT_BODY_OPEN_BRACKET;
		return;
	}
	// non-predefined word
	{
		currentState = State::STRUCT_NAME;
		return;
	}
}

void HLSLConverter::structName()
{
	std::string structName = words.front();
	words.pop();

	structDecl.name = structName;

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::STRUCT_BODY_OPEN_BRACKET;

		return;
	}
	if (word == std::string(";"))
	{
		currentState = State::INSERT_STRUCT;
		return;
	}
}

void HLSLConverter::insertStruct()
{
	if (statesStack.top() == State::STRUCT_BODY_OPEN_BRACKET)
		statesStack.pop();
	if (statesStack.top() == State::STRUCT)
		statesStack.pop();

	ShaderUnits::ShaderComponent* pStruct = new ShaderUnits::STRUCT();
	if (structDecl.name != std::string(""))
		pStruct->setName(structDecl.name);
	if (structDecl.body)
		pStruct->add(structDecl.body);

	userTypes[structDecl.name] = pStruct;

	structDecl.clear();

	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pStruct);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::structBodyOpenBracket()
{
	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	structDecl.body = body;
	componentStack.push(body);

	statesStack.push(State::STRUCT_BODY_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::determineTypeOfSelection()
{
	int pos = userName.find('.');
	if (pos != userName.npos || statesStack.top() != State::FUNCTION_CALL)
	{
		currentState = State::SELECTED_FIELD;
		return;
	}
	if (statesStack.top() == State::FUNCTION_CALL)
	{
		currentState = State::SELECTED_METHOD;
		return;
	}
}

void HLSLConverter::selectedField()
{
	std::string fieldName = userName;
	ShaderUnits::ShaderComponent* field = new ShaderUnits::SELECTED_FIELD();
	field->setName(fieldName);
	if (selectedFM_tail.top())
	{
		selectedFM_tail.top()->add(field);
		selectedFM_tail.top() = field;
	}
	else
	{
		selectedFM_head.top() = field;
		selectedFM_tail.top() = field;
	}

	int pos = userName.find(".", 0);
	if (pos != userName.npos)
	{
		fieldName = fieldName.substr(0, pos);
		userName = userName.substr(pos + 1);

		field->setName(fieldName);

		currentState = State::DETERMINE_TYPE_OF_SELECTION;
		return;
	}
	if (statesStack.top() == State::ARGUMENTS_LIST_CLOSE_BRACKET)
	{
		selectedFM_tail.top() = nullptr;
		statesStack.pop();
		currentState = State::ARGUMENTS_LIST_CLOSE_BRACKET;
		return;
	}

	userName = "";
	currentState = State::VARIABLE;

	return;
}

void HLSLConverter::selectedMethod()
{
	std::string methodName = userName;
	ShaderUnits::ShaderComponent* method = new ShaderUnits::SELECTED_METHOD();
	method->setName(methodName);
	if (selectedFM_tail.top())
	{
		selectedFM_tail.top()->add(method);
		selectedFM_tail.top() = method;
	}
	else
	{
		selectedFM_head.top() = method;
		selectedFM_tail.top() = method;
	}

	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	return;
}

void HLSLConverter::cast()
{
	ShaderUnits::CAST* castOp = new ShaderUnits::CAST();
	componentStack.push(castOp);
	statesStack.push(State::CAST);

	std::string word = words.front();
	if (word == std::string("int"))
	{
		words.pop();
		currentState = State::INT1;

		return;
	}
	if (word == std::string("float"))
	{
		words.pop();
		currentState = State::FLOAT1;

		return;
	}
	if (word == std::string("float2"))
	{
		words.pop();
		currentState = State::FLOAT2;

		return;
	}
	if (word == std::string("float3"))
	{
		words.pop();
		currentState = State::FLOAT3;

		return;
	}
	if (word == std::string("float4"))
	{
		words.pop();
		currentState = State::FLOAT4;

		return;
	}
	if (word == std::string("float3x3"))
	{
		words.pop();
		currentState = State::FLOAT3X3;

		return;
	}
	if (word == std::string("float4x4"))
	{
		words.pop();
		currentState = State::FLOAT4X4;

		return;
	}
	if (userTypes.count(word))
	{
		words.pop();
		currentState = State::USER_TYPE;

		userType = userTypes[word];

		return;
	}
}

void HLSLConverter::updateCast()
{
	ShaderUnits::ShaderComponent* type = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* castOp = componentStack.top();
	castOp->add(type);

	currentState = State::UNKNOWN;
}

void HLSLConverter::countOfElements()
{
	ShaderUnits::SQUARE_BRACKETS* pCountOfElements = new ShaderUnits::SQUARE_BRACKETS();
	componentStack.push(pCountOfElements);

	statesStack.push(State::COUNT_OF_ELEMENTS);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::insertCountOfElements()
{
	statesStack.pop();
	statesStack.pop();

	decls.top().countOfElements = componentStack.top();
	componentStack.pop();

	currentState = State::VARIABLE_DECLARATION;

	return;
}

void HLSLConverter::texture2dState()
{
	std::string word = words.front();
	words.pop();

	HLSLConverter::DeclarationFunctionOrVariable decl;

	decl.type = new ShaderUnits::TEXTURE2D();
	decl.name = word;

	decls.push(decl);

	currentState = State::CUSTOM_NAME;
}

void HLSLConverter::samplerStateState()
{
	statesStack.push(State::SAMPLER_STATE);
	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::SAMPLER_STATE_BODY_OPEN_BRACKET;
		return;
	}
	// non-predefined word
	{
		currentState = State::SAMPLER_STATE_NAME;
		return;
	}
}

void HLSLConverter::samplerStateName()
{
	std::string samplerStateName = words.front();
	words.pop();

	samplerStateDecl.name = samplerStateName;

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::SAMPLER_STATE_BODY_OPEN_BRACKET;

		return;
	}
	if (word == std::string(";"))
	{
		currentState = State::INSERT_SAMPLER_STATE;
		return;
	}
}

void HLSLConverter::insertSamplerState()
{
	if (statesStack.top() == State::SAMPLER_STATE_BODY_OPEN_BRACKET)
		statesStack.pop();
	if (statesStack.top() == State::SAMPLER_STATE)
		statesStack.pop();

	ShaderUnits::ShaderComponent* pSamplerState = new ShaderUnits::SAMPLER_STATE();
	if (samplerStateDecl.name != std::string(""))
		pSamplerState->setName(samplerStateDecl.name);
	if (samplerStateDecl.body)
		pSamplerState->add(samplerStateDecl.body);
	samplerStateDecl.clear();

	componentStack.pop();

	ShaderUnits::ShaderComponent* parent = componentStack.top();
	parent->add(pSamplerState);

	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::samplerStateBodyOpenBracket()
{
	ShaderUnits::ShaderComponent* body = new ShaderUnits::CURLY_BRACKETS();
	samplerStateDecl.body = body;
	componentStack.push(body);

	statesStack.push(State::SAMPLER_STATE_BODY_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void HLSLConverter::forState()
{
	ShaderUnits::ShaderComponent* pFor = new ShaderUnits::FOR();
	componentStack.push(pFor);

	if (pUnroll)
	{
		pFor->add(pUnroll);
		pUnroll = nullptr;
	}

	statesStack.push(State::FOR);

	std::string word = words.front();
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::FOR_EXPRESSION_OPEN_BRACKET;
		return;
	}
}

void HLSLConverter::forExpressionOpenBracket()
{
	ShaderUnits::ShaderComponent* pExpression = new ShaderUnits::ROUND_BRACKETS();
	componentStack.push(pExpression);

	currentState = State::FOR_INIT_EXPRESSION;
}

void HLSLConverter::forInitExpression()
{
	ShaderUnits::ShaderComponent* pInitExpression = new ShaderUnits::INIT_EXPRESSION();
	componentStack.push(pInitExpression);

	statesStack.push(State::FOR_INIT_EXPRESSION);

	currentState = State::UNKNOWN;
}

void HLSLConverter::forCondExpression()
{
	ShaderUnits::ShaderComponent* pCondExpression = new ShaderUnits::COND_EXPRESSION();
	componentStack.push(pCondExpression);

	statesStack.push(State::FOR_COND_EXPRESSION);

	currentState = State::UNKNOWN;
}

void HLSLConverter::forLoopExpression()
{
	ShaderUnits::ShaderComponent* pLoopExpression = new ShaderUnits::LOOP_EXPRESSION();
	componentStack.push(pLoopExpression);

	statesStack.push(State::FOR_LOOP_EXPRESSION);

	currentState = State::UNKNOWN;
}

void HLSLConverter::insertForExpression()
{
	ShaderUnits::ShaderComponent* pExpression = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pFor = componentStack.top();
	pFor->add(pExpression);

	std::string word = words.front();
	if (word == std::string("{"))
	{
		words.pop();
		currentState = State::FOR_BODY_OPEN_BRACKET;
		return;
	}
	currentState = State::UNKNOWN;
}

void HLSLConverter::forBodyOpenBracket()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pForBody = new ShaderUnits::CURLY_BRACKETS();
	componentStack.push(pForBody);

	statesStack.push(State::FOR_BODY_OPEN_BRACKET);

	currentState = State::UNKNOWN;
}

void HLSLConverter::insertForInitExpression()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pInitExpression = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pExpression = componentStack.top();
	pExpression->add(pInitExpression);

	currentState = State::FOR_COND_EXPRESSION;
}

void HLSLConverter::insertForCondExpression()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pCondExpression = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pExpression = componentStack.top();
	pExpression->add(pCondExpression);

	currentState = State::FOR_LOOP_EXPRESSION;
}

void HLSLConverter::insertForLoopExpression()
{
	statesStack.pop();

	ShaderUnits::ShaderComponent* pLoopExpression = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pExpression = componentStack.top();
	pExpression->add(pLoopExpression);

	currentState = State::INSERT_FOR_EXPRESSION;
}

void HLSLConverter::insertFor()
{
	statesStack.pop();
	
	ShaderUnits::ShaderComponent* pForBody = componentStack.top();
	componentStack.pop();

	ShaderUnits::ShaderComponent* pFor = componentStack.top();
	componentStack.pop();

	pFor->add(pForBody);

	ShaderUnits::ShaderComponent* pParent = componentStack.top();
	pParent->add(pFor);

	currentState = State::UNKNOWN;
}