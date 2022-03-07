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

			break;
		
		case State::BRACKETS_UNARY_OPERATOR_OPEN:

			break;

		case State::BRACKETS_UNARY_OPERATOR_CLOSE:

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
		statesStack.push(State::UNKNOWN);
		currentState = State::BRACKETS_UNARY_OPERATOR_OPEN;
		componentStack.push(new ROUND_BRACKETS());

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
}