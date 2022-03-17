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

		case State::BINARY_PLUS:

			break;

		case State::BINARY_DIVIDE:

			break;

		case State::BINARY_MULTIPLY:

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
	if (word == ")" && statesStack.top() == State::BRACKETS_UNARY_OPERATOR_OPEN)
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

	if (isOperationState(statesStack.top()))
	{
		Component* operand = componentStack.top();
		componentStack.pop();

		Component* operation = componentStack.top();
		operation->add(operand);
	}

	if (isOperationState(statesStack.top()) && (word == ";" || word == "," || word == ")" || word == ";"))
	{
		statesStack.pop();
		return;
	}

	if (isOperationState(statesStack.top()) && word == "+")
	{
		statesStack.pop();
		currentState = State::BINARY_PLUS;
		return;
	}
	if (isOperationState(statesStack.top()) && word == "-")
	{
		statesStack.pop();
		currentState = State::BINARY_MINUS;
		return;
	}
	if (isOperationState(statesStack.top()) && word == "*")
	{
		statesStack.pop();
		currentState = State::BINARY_MULTIPLY;
		return;
	}
	if (isOperationState(statesStack.top()) && word == "/")
	{
		statesStack.pop();
		currentState = State::BINARY_DIVIDE;
		return;
	}

	words.pop();

	Component* component = componentStack.top();
	componentStack.pop();

	componentStack.top()->add(component);

	currentState = State::UNKNOWN;
	if (!statesStack.empty())
	{
		currentState = statesStack.top();
		statesStack.pop();
	}
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
	State lastState = statesStack.top();
	if (lastState == State::BINARY_DIVIDE || lastState == State::BINARY_MULTIPLY)
		currentState = State::FINISH_EXPRESSION;
	else
		currentState = State::CREATING_BINARY_MINUS;
}