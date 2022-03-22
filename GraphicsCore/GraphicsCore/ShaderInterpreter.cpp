#include "ShaderInterpreter.h"
#include "StringUtils.h"

void ShaderBuilder::build()
{
	words = std::queue<std::string>();
	statesStack = std::stack<State>();
	componentStack = std::stack<Component*>();
	currentState = State::UNKNOWN;
	currentIndex = 0;

	do
	{
		std::string word = readWord(
			shaderText,
			std::string(skipSymbols),
			std::string(stopSymbols),
			currentIndex
		);
		words.push(word);

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

		}
	} while (!words.empty());
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

	currentState = State::UNKNOWN;
}

bool ShaderBuilder::isOperationState(State state) const
{
	if (state == State::BINARY_DIVIDE ||
		state == State::BINARY_MINUS ||
		state == State::BINARY_MULTIPLY ||
		state == State::BINARY_PLUS
		)
		return true;

	return false;
}

void ShaderBuilder::binaryMinus()
{
	State lastState = State::UNKNOWN;
	if(!statesStack.empty())
		lastState = statesStack.top();

	if (lastState == State::BINARY_DIVIDE || lastState == State::BINARY_MULTIPLY)
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

	if (lastState == State::BINARY_DIVIDE || lastState == State::BINARY_MULTIPLY)
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

	if (lastState == State::BINARY_DIVIDE)
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