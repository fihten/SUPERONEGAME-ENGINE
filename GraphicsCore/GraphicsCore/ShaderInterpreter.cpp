#include "ShaderInterpreter.h"
#include "StringUtils.h"

void ShaderBuilder::build()
{
	words = std::queue<std::string>();
	statesStack = std::stack<State>();
	componentStack = std::stack<Component*>();
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
		if (word != "")
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
		}
	}
}

void ShaderBuilder::unknown()
{
	std::string word = words.front();

	// open round brackets
	if (word == "(")
	{
		words.pop();
		currentState = State::BRACKETS_UNARY_OPERATOR_OPEN;

		return;
	}
	if (word == ")" && !statesStack.empty() && statesStack.top() == State::BRACKETS_UNARY_OPERATOR_OPEN)
	{
		words.pop();
		currentState = State::BRACKETS_UNARY_OPERATOR_CLOSE;

		return;
	}
	if (word == ")" && !statesStack.empty() && statesStack.top() == State::ARGUMENTS_LIST_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::ARGUMENTS_LIST_CLOSE_BRACKET;

		return;
	}
	if (word == ")" && !statesStack.empty() && statesStack.top() == State::SIGNATURE_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::SIGNATURE_CLOSE_BRACKET;

		return;
	}
	if (word == "}" && !statesStack.empty() && statesStack.top() == State::FUNCTION_BODY_OPEN_BRACKET)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_CLOSE_BRACKET;

		return;
	}
	if ((word == ";" || word == "," || word == ")") 
		&& !statesStack.empty() && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		if (word != ")")
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
	if (word == "-")
	{
		words.pop();
		currentState = State::UNARY_MINUS;

		return;
	}
	if (word == "+")
	{
		words.pop();
		currentState = State::UNARY_PLUS;

		return;
	}
	if (word == "void")
	{
		words.pop();
		currentState = State::VOID;

		return;
	}
	if (word == "float")
	{
		words.pop();
		currentState = State::FLOAT;

		return;
	}
	if (word == "float2")
	{
		words.pop();
		currentState = State::FLOAT2;

		return;
	}
	if (word == "float3")
	{
		words.pop();
		currentState = State::FLOAT3;

		return;
	}
	if (word == "float4")
	{
		words.pop();
		currentState = State::FLOAT4;

		return;
	}
	if (word == "float4x4")
	{
		words.pop();
		currentState = State::FLOAT4X4;

		return;
	}
	if (word == "in")
	{
		words.pop();
		currentState = State::IN;

		return;
	}
	if (word == "out")
	{
		words.pop();
		currentState = State::OUT;

		return;
	}
	if (word == "inout")
	{
		words.pop();
		currentState = State::INOUT;

		return;
	}
	if (word == "uniform")
	{
		words.pop();
		currentState = State::UNIFORM;

		return;
	}
	if (word == "mul")
	{
		words.pop();
		currentState = State::MUL;

		return;
	}

	userName = word;
	words.pop();
	if (words.front() != "(");
	{
		currentState = State::VARIABLE;
		return;
	}
	if (words.front() == "(");
	{
		words.pop();
		currentState = State::FUNCTION_CALL;
		return;
	}
}

void ShaderBuilder::bracketsUnaryOperatorOpen()
{
	statesStack.push(State::BRACKETS_UNARY_OPERATOR_OPEN);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::bracketsUnaryOperatorClose()
{
	std::string word = words.front();

	statesStack.pop();
	if (word == ";" || word == "," || word == ")" || word == ":")
		currentState = State::FINISH_EXPRESSION;
	if (word == "+")
		currentState = State::BINARY_PLUS;
	if (word == "-")
		currentState = State::BINARY_MINUS;
	if (word == "*")
		currentState = State::BINARY_MULTIPLY;
	if (word == "/")
		currentState = State::BINARY_DIVIDE;
}

void ShaderBuilder::finishExpression()
{
	std::string word = words.front();
	bool op = false;
	if (!statesStack.empty())
		op = isOperationState(statesStack.top());

	if (op)
	{
		Component* operand = componentStack.top();
		componentStack.pop();

		Component* operation = componentStack.top();
		operation->add(operand);
	}

	if (op && (word == ";" || word == "," || word == ")"))
	{
		statesStack.pop();
		return;
	}

	if (op && word == "+")
	{
		statesStack.pop();
		currentState = State::BINARY_PLUS;
		return;
	}
	if (op && word == "-")
	{
		statesStack.pop();
		currentState = State::BINARY_MINUS;
		return;
	}
	if (op && word == "*")
	{
		statesStack.pop();
		currentState = State::BINARY_MULTIPLY;
		return;
	}
	if (op && word == "/")
	{
		statesStack.pop();
		currentState = State::BINARY_DIVIDE;
		return;
	}

	State lastState = State::UNKNOWN;
	if (!statesStack.empty())
		lastState = statesStack.top();

	currentState = State::UNKNOWN;
	if ((word != ")" || lastState != State::BRACKETS_UNARY_OPERATOR_OPEN) &&
		((word != ")" && word != ";" && word != ",") || lastState != State::VARIABLE_DECLARATION))
	{
		Component* componentToAdd = componentStack.top();
		componentStack.pop();

		Component* composite = componentStack.top();
		composite->add(componentToAdd);

		if (word != ")")
			words.pop();
	}
	if ((word == ")" || word == ";" || word == ",") && lastState == State::VARIABLE_DECLARATION)
	{
		decls.top().value = componentStack.top();
		componentStack.pop();
	}
	
}

bool ShaderBuilder::isOperationState(State state) const
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

void ShaderBuilder::binaryMinus()
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

void ShaderBuilder::creatingBinaryMinus()
{
	words.pop();

	Component* leftOperand = componentStack.top();
	componentStack.pop();

	Component* binaryMinusOp = new ::BINARY_MINUS();
	binaryMinusOp->add(leftOperand);

	componentStack.push(binaryMinusOp);

	statesStack.push(State::BINARY_MINUS);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::binaryPlus()
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

void ShaderBuilder::creatingBinaryPlus()
{
	words.pop();

	Component* leftOperand = componentStack.top();
	componentStack.pop();

	Component* binaryPlusOp = new ::BINARY_PLUS();
	binaryPlusOp->add(leftOperand);

	componentStack.push(binaryPlusOp);

	statesStack.push(State::BINARY_PLUS);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::binaryDivide()
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

void ShaderBuilder::creatingBinaryDivide()
{
	words.pop();

	Component* leftOperand = componentStack.top();
	componentStack.pop();

	Component* binaryDivideOp = new ::BINARY_DIVIDE();
	binaryDivideOp->add(leftOperand);

	componentStack.push(binaryDivideOp);

	statesStack.push(State::BINARY_DIVIDE);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::binaryMultiply()
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

void ShaderBuilder::creatingBinaryMultiply()
{
	words.pop();

	Component* leftOperand = componentStack.top();
	componentStack.pop();

	Component* binaryMultiplyOp = new ::BINARY_MULTIPLY();
	binaryMultiplyOp->add(leftOperand);

	componentStack.push(binaryMultiplyOp);

	statesStack.push(State::BINARY_MULTIPLY);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::variable()
{
	Component* var = new ::VARIABLE();
	var->setName(userName);
	userName = "";

	componentStack.push(var);

	std::string word = words.front();
	if (word == "," || word == ";" || word == ")" || word == ":" || word == "}")
		currentState = State::FINISH_EXPRESSION;
	if (word == "-")
		currentState = State::BINARY_MINUS;
	if (word == "+")
		currentState = State::BINARY_PLUS;
	if (word == "*")
		currentState = State::BINARY_MULTIPLY;
	if (word == "/")
		currentState = State::BINARY_DIVIDE;
	if (word == "=")
		currentState = State::ASSIGNMENT;
}

void ShaderBuilder::unaryMinus()
{
	statesStack.push(State::UNARY_MINUS);
	currentState = State::UNKNOWN;

	Component* unaryMinusOp = new ::UNARY_MINUS();
	componentStack.push(unaryMinusOp);
}

void ShaderBuilder::unaryPlus()
{
	statesStack.push(State::UNARY_PLUS);
	currentState = State::UNKNOWN;

	Component* unaryPlusOp = new ::UNARY_PLUS();
	componentStack.push(unaryPlusOp);
}

void ShaderBuilder::functionCall()
{
	Component* functionCallOp = new ::FUNCTION_CALL();
	functionCallOp->setName(userName);
	userName = "";

	componentStack.push(functionCallOp);

	statesStack.push(State::FUNCTION_CALL);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;
}

void ShaderBuilder::argumentsListOpenBracket()
{
	Component* arguments = new ::ROUND_BRACKETS();
	componentStack.push(arguments);

	statesStack.push(State::ARGUMENTS_LIST_OPEN_BRACKET);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::argumentsListCloseBracket()
{
	statesStack.pop();
	statesStack.pop();

	Component* arguments = componentStack.top();
	componentStack.pop();

	Component* func = componentStack.top();
	func->add(arguments);

	std::string word = words.front();
	if (word == "," || word == ";" || word == ")")
		currentState = State::FINISH_EXPRESSION;
	if (word == "+")
		currentState = State::BINARY_PLUS;
	if (word == "-")
		currentState = State::BINARY_MINUS;
	if (word == "*")
		currentState = State::BINARY_MULTIPLY;
	if (word == "/")
		currentState = State::BINARY_DIVIDE;
}

void ShaderBuilder::assignment()
{
	words.pop();

	Component* leftValue = componentStack.top();
	componentStack.pop();

	Component* assignmentOp = new ::ASSIGNMENT();
	assignmentOp->add(leftValue);
	componentStack.push(assignmentOp);

	statesStack.push(State::ASSIGNMENT);
	currentState = State::UNKNOWN;
}

void ShaderBuilder::voidState()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::VOID();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderBuilder::floatState()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderBuilder::float2State()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT2();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderBuilder::float3State()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT3();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		return;
	}
}

void ShaderBuilder::float4State()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT4();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;
		
		return;
	}
	if (word == "(")
	{
		words.pop();
		currentState = State::FLOAT4_CONSTRUCTOR;

		return;
	}
}

void ShaderBuilder::float4x4State()
{
	std::string word = words.front();
	if (word != "(")
	{
		words.pop();

		ShaderBuilder::DeclarationFunctionOrVariable decl;

		decl.type = new ::FLOAT4X4();
		decl.name = word;

		decls.push(decl);

		currentState = State::CUSTOM_NAME;

		return;
	}
}

void ShaderBuilder::customName()
{
	std::string word = words.front();
	if (word == "(")
	{
		currentState = State::FUNCTION_DECLARATION;
		return;
	}

	currentState = State::VARIABLE_DECLARATION;
	return;
}

void ShaderBuilder::functionDeclaration()
{
	std::string word = words.front();
	if (word == "(")
	{
		words.pop();

		statesStack.push(State::FUNCTION_DECLARATION);
		currentState = State::SIGNATURE_OPEN_BRACKET;

		return;
	}
}

void ShaderBuilder::signatureOpenBracket()
{
	Component* signature = new ::ROUND_BRACKETS();
	decls.top().signature = signature;
	componentStack.push(signature);

	statesStack.push(State::SIGNATURE_OPEN_BRACKET);
	currentState = State::UNKNOWN;

	return;
}

void ShaderBuilder::signatureCloseBracket()
{
	std::string word = words.front();
	componentStack.pop();

	if (word == ":")
	{
		words.pop();
		currentState = State::SEMANTIC;
		return;
	}
	if (word == ";")
	{
		words.pop();
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == "{")
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
}

void ShaderBuilder::semantic()
{
	std::string word = words.front();
	if (word == "SV_POSITION")
	{
		words.pop();
		currentState = State::SV_POSITION_;
		return;
	}
	if (word == "SV_TARGET")
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

void ShaderBuilder::svPosition()
{
	std::string word = words.front();

	Component* semanticElement = new ::SV_POSITION();
	decls.top().semantic = semanticElement;

	if (word == ";" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == "{" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == ";" || word == "," || word == ")") && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		if (word != ")")
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderBuilder::svTarget()
{
	std::string word = words.front();

	Component* semanticElement = new ::SV_TARGET();
	decls.top().semantic = semanticElement;

	if (word == ";" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == "{" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == ";" || word == "," || word == ")") && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		if (word != ")")
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderBuilder::customSemantic()
{
	std::string word = words.front();

	Component* semanticElement = new ::SEMANTIC();
	semanticElement->setName(userName);
	userName = "";
	decls.top().semantic = semanticElement;

	if (word == ";" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::INSERT_FUNCTION_DECLARATION;
		return;
	}
	if (word == "{" && statesStack.top() == State::FUNCTION_DECLARATION)
	{
		words.pop();
		currentState = State::FUNCTION_BODY_OPEN_BRACKET;
		return;
	}
	if ((word == ";" || word == "," || word == ")") && statesStack.top() == State::VARIABLE_DECLARATION)
	{
		if (word != ")")
			words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderBuilder::functionBodyOpenBracket()
{
	Component* body = new ::CURLY_BRACKETS();
	decls.top().body = body;
	componentStack.push(body);

	currentState = State::UNKNOWN;
	statesStack.push(State::FUNCTION_BODY_OPEN_BRACKET);
}

void ShaderBuilder::functionBodyCloseBracket()
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

void ShaderBuilder::insertFunctionDeclaration()
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

void ShaderBuilder::variableDeclaration()
{
	if (modifier)
	{
		decls.top().modifier = modifier;
		modifier = nullptr;
	}

	std::string word = words.front();
	statesStack.push(State::VARIABLE_DECLARATION);
	if (word == "=")
	{
		words.pop();
		currentState = State::UNKNOWN;
		return;
	}
	if (word == ":")
	{
		words.pop();
		currentState = State::SEMANTIC;
		return;
	}
	if (word == ";")
	{
		words.pop();
		currentState = State::INSERT_VARIABLE_DECLARATION;
		return;
	}
}

void ShaderBuilder::insertVariableDeclaration()
{
	statesStack.pop();

	Component* pVariableDeclaration = new ::VARIABLE_DECL();
	
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

void ShaderBuilder::inState()
{
	modifier = new ::IN();
	currentState = State::UNKNOWN;

	return;
}

void ShaderBuilder::outState()
{
	modifier = new ::OUT();
	currentState = State::UNKNOWN;

	return;
}

void ShaderBuilder::inoutState()
{
	modifier = new ::INOUT();
	currentState = State::UNKNOWN;

	return;
}

void ShaderBuilder::uniformState()
{
	modifier = new ::UNIFORM();
	currentState = State::UNKNOWN;

	return;
}

void ShaderBuilder::mulState()
{
	statesStack.push(State::MUL);

	Component* pMul = new ::MUL();
	componentStack.push(pMul);

	std::string word = words.front();
	if (word == "(")
	{
		words.pop();
		currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

		return;
	}
}

void ShaderBuilder::float4constructor()
{
	Component* pFloat4Constructor = new ::FLOAT4_CONSTRUCTOR();
	componentStack.push(pFloat4Constructor);

	statesStack.push(State::FLOAT4_CONSTRUCTOR);
	currentState = State::ARGUMENTS_LIST_OPEN_BRACKET;

	return;
}