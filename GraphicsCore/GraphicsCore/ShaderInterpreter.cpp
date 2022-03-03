#include "ShaderInterpreter.h"
#include "StringUtils.h"

void ShaderBuilder::build()
{
	words = std::queue<std::string>();
	statesStack = std::stack<State>();
	currentState = State::UNKNOWN;
	leftOperand = nullptr;
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
	words.pop();
}