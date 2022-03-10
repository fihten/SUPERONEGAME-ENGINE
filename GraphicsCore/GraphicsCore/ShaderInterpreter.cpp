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

	if (word == ";" || word == "," || word == ")" || word == ":")
	{
		statesStack.push(State::BRACKETS_UNARY_OPERATOR_CLOSE);
		currentState = State::FINISH_EXPRESSION;

		return;
	}
	if (word == "+")
	{
		words.pop();
		statesStack.push(State::BRACKETS_UNARY_OPERATOR_CLOSE);
		currentState = State::BINARY_PLUS;

		return;
	}
	if (word == "-")
	{
		words.pop();
		statesStack.push(State::BRACKETS_UNARY_OPERATOR_CLOSE);
		currentState = State::BINARY_MINUS;

		return;
	}
	if (word == "*")
	{
		words.pop();
		statesStack.push(State::BRACKETS_UNARY_OPERATOR_CLOSE);
		currentState = State::BINARY_MULTIPLY;

		return;
	}
	if (word == "/")
	{
		words.pop();
		statesStack.push(State::BRACKETS_UNARY_OPERATOR_CLOSE);
		currentState = State::BINARY_DIVIDE;

		return;
	}
}

void ShaderBuilder::finishExpression()
{
	if (statesStack.top() == State::BRACKETS_UNARY_OPERATOR_CLOSE)
	{
		statesStack.pop();
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