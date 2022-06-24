// ShaderInterpreterTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "ShaderVisitor.h"
#include "ShaderInterpreter.h"

class PrintVisitor : public ShaderVisitor
{
	int numberOfSpaces = 0;
	void printSpaces()
	{
		for (int i = 0; i < numberOfSpaces; ++i)
			std::cout << ' ';
	};
public:
	void startVisit(const CBUFFER* pCBUFFER) 
	{
		printSpaces();
		std::cout << "CBUFFER " << pCBUFFER->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const FLOAT4X4* pFLOAT4X4) 
	{
		printSpaces();
		std::cout << "float4x4" << std::endl;
	};
	void startVisit(const VOID* pVOID) 
	{
		printSpaces();
		std::cout << "void" << std::endl;
	};
	void startVisit(const FLOAT1* pFLOAT)
	{
		printSpaces();
		std::cout << "float" << std::endl;
	};
	void startVisit(const FLOAT2* pFLOAT2) 
	{
		printSpaces();
		std::cout << "float2" << std::endl;
	};
	void startVisit(const FLOAT3* pFLOAT3) 
	{
		printSpaces();
		std::cout << "float3" << std::endl;
	};
	void startVisit(const FLOAT4* pFLOAT4)
	{
		printSpaces();
		std::cout << "float4" << std::endl;
	};
	void startVisit(const STRUCT* pSTRUCT) 
	{
		printSpaces();
		std::cout << "struct " << pSTRUCT->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const SEMANTIC* pSEMANTIC) 
	{
		printSpaces();
		std::cout << "user semantic " << pSEMANTIC->getName() << std::endl;
	};
	void startVisit(const SV_POSITION* pSV_POSITION) 
	{
		printSpaces();
		std::cout << "SV_POSITION" << std::endl;
	};
	void startVisit(const SV_TARGET* pSV_TARGET) 
	{
		printSpaces();
		std::cout << "SV_TARGET" << std::endl;
	};
	void startVisit(const FUNCTION_DECL* pFUNCTION_DECL) 
	{
		printSpaces();
		std::cout << "function declaration " << pFUNCTION_DECL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const FUNCTION_CALL* pFUNCTION_CALL) 
	{
		printSpaces();
		std::cout << "function call " << pFUNCTION_CALL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const MUL* pMUL) 
	{
		printSpaces();
		std::cout << "mul" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) 
	{
		printSpaces();
		std::cout << "constructor of float4" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) 
	{
		printSpaces();
		std::cout << "set_vertex_shader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const SET_PIXEL_SHADER* pSET_PIXEL_SHADER) 
	{
		printSpaces();
		std::cout << "set_pixel_shader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const COMPILE_SHADER* pCOMPILE_SHADER) 
	{
		printSpaces();
		std::cout << "CompileShader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) 
	{
		printSpaces();
		std::cout << pVERTEX_SHADER_VERSION->getName() << std::endl;
	};
	void startVisit(const PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) 
	{
		printSpaces();
		std::cout << pPIXEL_SHADER_VERSION->getName() << std::endl;
	};
	void startVisit(const IN* pIN) 
	{
		printSpaces();
		std::cout << "in" << std::endl;
	};
	void startVisit(const OUT* pOUT) 
	{
		printSpaces();
		std::cout << "out" << std::endl;
	};
	void startVisit(const INOUT* pINOUT) 
	{
		printSpaces();
		std::cout << "inout" << std::endl;
	};
	void startVisit(const UNIFORM* pUNIFORM) 
	{
		printSpaces();
		std::cout << "uniform" << std::endl;
	};
	void startVisit(const VARIABLE* pVARIABLE) 
	{
		printSpaces();
		std::cout << pVARIABLE->getName() << std::endl;
	};
	void startVisit(const VARIABLE_DECL* pVARIABLE_DECL) 
	{
		printSpaces();
		std::cout << "Decl of variable " << pVARIABLE_DECL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const BINARY_MINUS* pBINARY_MINUS)
	{
		printSpaces();
		std::cout << "-" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const BINARY_PLUS* pBINARY_PLUS) 
	{
		printSpaces();
		std::cout << "+" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const BINARY_DIVIDE* pBINARY_DIVIDE) 
	{
		printSpaces();
		std::cout << "/" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const BINARY_MULTIPLY* pBINARY_MULTIPLY) 
	{
		printSpaces();
		std::cout << "*" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const UNARY_MINUS* pUNARY_MINUS) 
	{
		printSpaces();
		std::cout << "-" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const UNARY_PLUS* pUNARY_PLUS) 
	{
		printSpaces();
		std::cout << "+" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ASSIGNMENT* pASSIGNMENT) 
	{
		printSpaces();
		std::cout << "=" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const TECHNIQUE11* pTECHNIQUE11) 
	{
		printSpaces();
		std::cout << "technique11 " << pTECHNIQUE11->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const PASS* pPASS) 
	{
		printSpaces();
		std::cout << "pass " << pPASS->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ROUND_BRACKETS* pROUND_BRACKETS) 
	{
		printSpaces();
		std::cout << "()" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const SQUARE_BRACKETS* pSQUARE_BRACKETS) 
	{
		printSpaces();
		std::cout << "[]" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const CURLY_BRACKETS* pCURLY_BRACKETS) 
	{
		printSpaces();
		std::cout << "{}" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const RETURN* pRETURN) 
	{
		printSpaces();
		std::cout << "return" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const SHADER* pSHADER) 
	{
		printSpaces();
		std::cout << "SHADER:" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderComponent* pShaderComponent) {};

	void finishVisit(const CBUFFER* pCBUFFER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const FLOAT4X4* pFLOAT4X4) {};
	void finishVisit(const VOID* pVOID) {};
	void finishVisit(const FLOAT1* pFLOAT) {};
	void finishVisit(const FLOAT2* pFLOAT2) {};
	void finishVisit(const FLOAT3* pFLOAT3) {};
	void finishVisit(const FLOAT4* pFLOAT4) {};
	void finishVisit(const STRUCT* pSTRUCT)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const SEMANTIC* pSEMANTIC) {};
	void finishVisit(const SV_POSITION* pSV_POSITION) {};
	void finishVisit(const SV_TARGET* pSV_TARGET) {};
	void finishVisit(const FUNCTION_DECL* pFUNCTION_DECL) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const FUNCTION_CALL* pFUNCTION_CALL) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const MUL* pMUL)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const SET_PIXEL_SHADER* pSET_PIXEL_SHADER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const COMPILE_SHADER* pCOMPILE_SHADER) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	void finishVisit(const PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	void finishVisit(const IN* pIN) {};
	void finishVisit(const OUT* pOUT) {};
	void finishVisit(const INOUT* pINOUT) {};
	void finishVisit(const UNIFORM* pUNIFORM) {};
	void finishVisit(const VARIABLE* pVARIABLE) {};
	void finishVisit(const VARIABLE_DECL* pVARIABLE_DECL) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const BINARY_MINUS* pBINARY_MINUS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const BINARY_PLUS* pBINARY_PLUS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const BINARY_DIVIDE* pBINARY_DIVIDE) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const BINARY_MULTIPLY* pBINARY_MULTIPLY) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const UNARY_MINUS* pUNARY_MINUS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const UNARY_PLUS* pUNARY_PLUS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ASSIGNMENT* pASSIGNMENT) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const TECHNIQUE11* pTECHNIQUE11) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const PASS* pPASS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ROUND_BRACKETS* pROUND_BRACKETS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const SQUARE_BRACKETS* pSQUARE_BRACKETS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const CURLY_BRACKETS* pCURLY_BRACKETS) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const RETURN* pRETURN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const SHADER* pSHADER) 
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderComponent* pShaderComponent) {};
};

int main()
{
	std::ifstream file("C:\\3dEngine\\Shaders\\Demo.fx");
	if (file)
	{
		file.seekg(0, file.end);
		int length = file.tellg();
		file.seekg(0, file.beg);

		char* buff = new char[length + 1];
		file.read(buff, length);
		buff[length] = 0;

		std::string shaderText(buff);

		ShaderInterpreter interpreter;
		interpreter.setShaderText(shaderText);
		SHADER* shader = interpreter.build();

		PrintVisitor visitor;
		shader->query(&visitor);

		delete shader;
		file.close();
		delete []buff;
	}

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
