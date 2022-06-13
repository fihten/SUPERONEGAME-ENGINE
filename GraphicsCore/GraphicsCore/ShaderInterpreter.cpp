#include "ShaderInterpreter.h"
#include "StringUtils.h"

::SHADER* ShaderInterpreter::build()
{
	words = std::queue<std::string>();
	statesStack = std::stack<State>();
	
	componentStack = std::stack<ShaderComponent*>();
	componentStack.push(new ::SHADER());
	
	currentState = State::UNKNOWN;
	currentIndex = 0;

	for (;;)
	{
		std::string word = readWord(
			shaderText,
			std::string(skipSymbols),
			std::string(stopSymbols),
			currentIndex
		);
		if (word != std::string(""))
			words.push(word);
		else
			break;
	}

	while (!words.empty())
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

		case State::VARIABLE:
			variable();
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

		case State::VOID:
			voidState();
			break;

		case State::FLOAT:
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

		case State::FLOAT4X4:
			float4x4State();
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

		case State::IN:
			inState();
			break;

		case State::OUT:
			outState();
			break;

		case State::INOUT:
			inoutState();
			break;

		case State::UNIFORM:
			uniformState();
			break;

		case State::MUL:
			mulState();
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
		}
	}

	return dynamic_cast<::SHADER*>(componentStack.top());
}

void ShaderInterpreter::unknown()
{
	std::string word = words.front();

	// open round brackets
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::BRACKETS_UNARY_OPERATOR_OPEN;

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
		if (word != std::string(")"))
			words.pop();
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
		currentState = State::VOID;

		return;
	}
	if (word == std::string("float"))
	{
		words.pop();
		currentState = State::FLOAT;

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
	if (word == std::string("float4x4"))
	{
		words.pop();
		currentState = State::FLOAT4X4;

		return;
	}
	if (word == std::string("in"))
	{
		words.pop();
		currentState = State::IN;

		return;
	}
	if (word == std::string("out"))
	{
		words.pop();
		currentState = State::OUT;

		return;
	}
	if (word == std::string("inout"))
	{
		words.pop();
		currentState = State::INOUT;

		return;
	}
	if (word == std::string("uniform"))
	{
		words.pop();
		currentState = State::UNIFORM;

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
		words.pop();
		currentState = State::INSERT_RETURN;

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
	if (word != std::string("("))
	{
		currentState = State::VARIABLE;
		return;
	}
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::FUNCTION_CALL;
		return;
	}
}

void ShaderInterpreter::bracketsUnaryOperatorOpen()
{
	statesStack.push(State::BRACKETS_UNARY_OPERATOR_OPEN);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::bracketsUnaryOperatorClose()
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
}

void ShaderInterpreter::finishExpression()
{
	std::string word = words.front();
	bool op = false;
	if (!statesStack.empty())
		op = isOperationState(statesStack.top());

	if (op)
	{
		ShaderComponent* operand = componentStack.top();
		componentStack.pop();

		ShaderComponent* operation = componentStack.top();
		operation->add(operand);
	}

	if (op && (word == std::string(";") || word == std::string(",") || word == std::string(")")))
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

	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	currentState = State::UNKNOWN;
	if ((word != std::string(")") || lastState != State::BRACKETS_UNARY_OPERATOR_OPEN) &&
		((word != std::string(")") && word != std::string(";") && word != std::string(",")) || lastState != State::VARIABLE_DECLARATION))
	{
		ShaderComponent* componentToAdd = componentStack.top();
		componentStack.pop();

		ShaderComponent* composite = componentStack.top();
		composite->add(componentToAdd);

		if (word != std::string(")") && word != std::string(";"))
			words.pop();
	}
	if ((word == std::string(")") || word == std::string(";") || word == std::string(",")) && lastState == State::VARIABLE_DECLARATION)
	{
		decls.top().value = componentStack.top();
		componentStack.pop();
	}
	
}

bool ShaderInterpreter::isOperationState(State state) const
{
	if (state == State::BINARY_DIVIDE ||
		state == State::BINARY_MINUS ||
		state == State::BINARY_MULTIPLY ||
		state == State::BINARY_PLUS ||
		state == State::UNARY_MINUS ||
		state == State::UNARY_PLUS ||
		state == State::ASSIGNMENT
		)
		return true;

	return false;
}

void ShaderInterpreter::binaryMinus()
{
	State lastState = State::UNKNOWN;
	if(!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE || 
		lastState == State::BINARY_MULTIPLY || 
		lastState == State::UNARY_MINUS ||
		lastState == State::UNARY_PLUS)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_BINARY_MINUS;
}

void ShaderInterpreter::creatingBinaryMinus()
{
	words.pop();

	ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderComponent* binaryMinusOp = new ::BINARY_MINUS();
	binaryMinusOp->add(leftOperand);

	componentStack.push(binaryMinusOp);

	statesStack.push(State::BINARY_MINUS);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::binaryPlus()
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

void ShaderInterpreter::creatingBinaryPlus()
{
	words.pop();

	ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderComponent* binaryPlusOp = new ::BINARY_PLUS();
	binaryPlusOp->add(leftOperand);

	componentStack.push(binaryPlusOp);

	statesStack.push(State::BINARY_PLUS);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::binaryDivide()
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

void ShaderInterpreter::creatingBinaryDivide()
{
	words.pop();

	ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderComponent* binaryDivideOp = new ::BINARY_DIVIDE();
	binaryDivideOp->add(leftOperand);

	componentStack.push(binaryDivideOp);

	statesStack.push(State::BINARY_DIVIDE);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::binaryMultiply()
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

void ShaderInterpreter::creatingBinaryMultiply()
{
	words.pop();

	ShaderComponent* leftOperand = componentStack.top();
	componentStack.pop();

	ShaderComponent* binaryMultiplyOp = new ::BINARY_MULTIPLY();
	binaryMultiplyOp->add(leftOperand);

	componentStack.push(binaryMultiplyOp);

	statesStack.push(State::BINARY_MULTIPLY);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::variable()
{
	ShaderComponent* var = new ::VARIABLE();
	var->setName(userName);
	userName = std::string("");

	componentStack.push(var);

	std::string word = words.front();
	if (word == std::string(",") || word == std::string(";") || word == std::string(")") || word == std::string(":") || word == std::string("}"))
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
}

void ShaderInterpreter::unaryMinus()
{
	statesStack.push(State::UNARY_MINUS);
	currentState = State::UNKNOWN;

	ShaderComponent* unaryMinusOp = new ::UNARY_MINUS();
	componentStack.push(unaryMinusOp);
}

void ShaderInterpreter::unaryPlus()
{
	statesStack.push(State::UNARY_PLUS);
	currentState = State::UNKNOWN;

	ShaderComponent* unaryPlusOp = new ::UNARY_PLUS();
	componentStack.push(unaryPlusOp);
}

void ShaderInterpreter::functionCall()
{
	ShaderComponent* functionCallOp = new ::FUNCTION_CALL();
	functionCallOp->setName(userName);
	userName = std::string("");

	componentStack.push(functionCallOp);

	statesStack.push(State::FUNCTION_CALL);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;
}

void ShaderInterpreter::argumentsListOpenBracket()
{
	ShaderComponent* arguments = new ::ROUND_BRACKETS();
	componentStack.push(arguments);

	statesStack.push(State::ARGUMENTS_LIST_OPEN_BRACKET);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::argumentsListCloseBracket()
{
	statesStack.pop();
	statesStack.pop();

	ShaderComponent* arguments = componentStack.top();
	componentStack.pop();

	ShaderComponent* func = componentStack.top();
	func->add(arguments);

	std::string word = words.front();
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
}

void ShaderInterpreter::assignment()
{
	words.pop();

	ShaderComponent* leftValue = componentStack.top();
	componentStack.pop();

	ShaderComponent* assignmentOp = new ::ASSIGNMENT();
	assignmentOp->add(leftValue);
	componentStack.push(assignmentOp);

	statesStack.push(State::ASSIGNMENT);
	currentState = State::UNKNOWN;
}

void ShaderInterpreter::voidState()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::VOID();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderInterpreter::floatState()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderInterpreter::float2State()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT2();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderInterpreter::float3State()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT3();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderInterpreter::float4State()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT4();
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

void ShaderInterpreter::float4x4State()
{
	std::string word = words.front();
	if (word != std::string("("))
	{
		words.pop();

		ShaderInterpreter::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT4X4();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		return;
	}
}

void ShaderInterpreter::customName()
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

void ShaderInterpreter::functionDeclaration()
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

void ShaderInterpreter::signatureOpenBracket()
{
	ShaderComponent* signature = new ::ROUND_BRACKETS();
	decls.top().signature = signature;
	componentStack.push(signature);

	statesStack.push(State::SIGNATURE_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::signatureCloseBracket()
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
		words.pop();
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

void ShaderInterpreter::semantic()
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

void ShaderInterpreter::svPosition()
{
	std::string word = words.front();

	ShaderComponent* semanticElement = new ::SV_POSITION();
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
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
		if (word != std::string(")"))
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderInterpreter::svTarget()
{
	std::string word = words.front();

	ShaderComponent* semanticElement = new ::SV_TARGET();
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
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
		if (word != std::string(")"))
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderInterpreter::customSemantic()
{
	std::string word = words.front();

	ShaderComponent* semanticElement = new ::SEMANTIC();
	semanticElement->setName(userName);
	userName = std::string("");
	decls.top().semantic = semanticElement;

	if (word == std::string(";") && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
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
		if (word != std::string(")"))
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderInterpreter::functionBodyOpenBracket()
{
	ShaderComponent* body = new ::CURLY_BRACKETS();
	decls.top().body = body;
	componentStack.push(body);

	currentState = State::UNKNOWN;
	statesStack.push(State::FUNCTION_BODY_OPEN_BRACKET);
}

void ShaderInterpreter::functionBodyCloseBracket()
{
	componentStack.pop();

	statesStack.pop();
	statesStack.pop();

	DeclarationFunctionOrVariable& funcDeclStruct = decls.top();
	
	FUNCTION_DECL* funcDecl = new ::FUNCTION_DECL();
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

void ShaderInterpreter::insertFunctionDeclaration()
{
	statesStack.pop();

	DeclarationFunctionOrVariable& funcDeclStruct = decls.top();

	FUNCTION_DECL* funcDecl = new ::FUNCTION_DECL();
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

void ShaderInterpreter::variableDeclaration()
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
	if (word == std::string(";"))
	{
		words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderInterpreter::insertVariableDeclaration()
{
	statesStack.pop();

	ShaderComponent* pVariableDeclaration = new ::VARIABLE_DECL();
	
	if (decls.top().modifier)
		pVariableDeclaration->add(decls.top().modifier);
	pVariableDeclaration->add(decls.top().type);
	pVariableDeclaration->setName(decls.top().name);
	if (decls.top().semantic)
		pVariableDeclaration->add(decls.top().semantic);
	if (decls.top().value)
		pVariableDeclaration->add(decls.top().value);

	decls.pop();

	componentStack.top()->add(pVariableDeclaration);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::inState()
{
	modifier = new ::IN();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::outState()
{
	modifier = new ::OUT();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::inoutState()
{
	modifier = new ::INOUT();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::uniformState()
{
	modifier = new ::UNIFORM();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::mulState()
{
	statesStack.push(State::MUL);

	ShaderComponent* pMul = new ::MUL();
	componentStack.push(pMul);

	std::string word = words.front();
	if (word == std::string("("))
	{
		words.pop();
		currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

		return;
	}
}

void ShaderInterpreter::float4constructor()
{
	ShaderComponent* pFloat4Constructor = new ::FLOAT4_CONSTRUCTOR();
	componentStack.push(pFloat4Constructor);

	statesStack.push(State::FLOAT4_CONSTRUCTOR);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	return;
}

void ShaderInterpreter::cbufferState()
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

void ShaderInterpreter::cbufferName()
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
		words.pop();
		currentState = State::INSERT_CBUFFER;

		return;
	}
}

void ShaderInterpreter::insertCbuffer()
{
	if (statesStack.top() == State::CBUFFER_BODY_OPEN_BRACKET)
		statesStack.pop();
	if (statesStack.top() == State::CBUFFER)
		statesStack.pop();

	ShaderComponent* pCbuffer = new ::CBUFFER();
	if (cbufferDecl.name != std::string(""))
		pCbuffer->setName(cbufferDecl.name);
	if (cbufferDecl.body)
		pCbuffer->add(cbufferDecl.body);
	cbufferDecl.clear();

	componentStack.pop();

	ShaderComponent* parent = componentStack.top();
	parent->add(pCbuffer);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::cbufferBodyOpenBracket()
{
	ShaderComponent* body = new ::CURLY_BRACKETS();
	cbufferDecl.body = body;
	componentStack.push(body);

	statesStack.push(State::CBUFFER_BODY_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::technique11state()
{
	std::string techName = words.front();
	words.pop();
	words.pop();

	tech = new ::TECHNIQUE11();
	tech->setName(techName);

	ShaderComponent* body = new ::CURLY_BRACKETS();
	tech->add(body);

	componentStack.push(body);
	statesStack.push(State::TECHNIQUE11);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::insertTechnique11()
{
	componentStack.pop();

	ShaderComponent* parent = componentStack.top();
	parent->add(tech);
	tech = nullptr;

	statesStack.pop();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::passState()
{
	std::string passName = words.front();
	words.pop();
	words.pop();

	pass = new ::PASS();
	pass->setName(passName);

	ShaderComponent* body = new ::CURLY_BRACKETS();
	pass->add(body);

	componentStack.push(body);
	statesStack.push(State::PASS);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::insertPass()
{
	componentStack.pop();

	ShaderComponent* parent = componentStack.top();
	parent->add(pass);
	pass = nullptr;

	statesStack.pop();
	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::setVertexShaderState()
{
	ShaderComponent* pSetVertexShader = new ::SET_VERTEX_SHADER();
	componentStack.push(pSetVertexShader);

	statesStack.push(State::SET_VERTEX_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	return;
}

void ShaderInterpreter::setPixelShaderState()
{
	ShaderComponent* pSetPixelShader = new ::SET_PIXEL_SHADER();
	componentStack.push(pSetPixelShader);

	statesStack.push(State::SET_PIXEL_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	return;
}

void ShaderInterpreter::compileShader()
{
	ShaderComponent* pCompileShader = new ::COMPILE_SHADER();
	componentStack.push(pCompileShader);

	statesStack.push(State::COMPILE_SHADER);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	words.pop();

	return;
}

void ShaderInterpreter::vs_5_0_state()
{
	ShaderComponent* pVS_5_0 = new ::VERTEX_SHADER_VERSION();
	pVS_5_0->setName(std::string("vs_5_0"));

	ShaderComponent* parent = componentStack.top();
	parent->add(pVS_5_0);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::ps_5_0_state()
{
	ShaderComponent* pPS_5_0 = new ::PIXEL_SHADER_VERSION();
	pPS_5_0->setName(std::string("ps_5_0"));

	ShaderComponent* parent = componentStack.top();
	parent->add(pPS_5_0);

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::returnState()
{
	std::string word = words.front();
	
	ShaderComponent* pReturn = new ::RETURN();
	componentStack.push(pReturn);
	
	statesStack.push(State::RETURN);

	if (word == std::string(";"))
	{
		words.pop();
		currentState = State::INSERT_RETURN;

		return;
	}

	currentState = State::UNKNOWN;

	return;
}

void ShaderInterpreter::insertReturn()
{
	statesStack.pop();

	ShaderComponent* pReturn = componentStack.top();
	componentStack.pop();

	ShaderComponent* parent = componentStack.top();
	parent->add(pReturn);

	currentState = State::UNKNOWN;

	return;
}