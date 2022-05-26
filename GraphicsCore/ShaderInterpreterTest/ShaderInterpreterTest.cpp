// ShaderInterpreterTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ShaderVisitor.h"

class PrintVisitor : public ShaderVisitor
{
public:
	void startVisit(const CBUFFER* pCBUFFER) {};
	void startVisit(const FLOAT4X4* pFLOAT4X4) {};
	void startVisit(const VOID* pVOID) {};
	void startVisit(const FLOAT* pFLOAT) {};
	void startVisit(const FLOAT2* pFLOAT2) {};
	void startVisit(const FLOAT3* pFLOAT3) {};
	void startVisit(const FLOAT4* pFLOAT4) {};
	void startVisit(const STRUCT* pSTRUCT) {};
	void startVisit(const SEMANTIC* pSEMANTIC) {};
	void startVisit(const SV_POSITION* pSV_POSITION) {};
	void startVisit(const SV_TARGET* pSV_TARGET) {};
	void startVisit(const FUNCTION_DECL* pFUNCTION_DECL) {};
	void startVisit(const FUNCTION_CALL* pFUNCTION_CALL) {};
	void startVisit(const MUL* pMUL) {};
	void startVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	void startVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};
	void startVisit(const SET_PIXEL_SHADER* pSET_PIXEL_SHADER) {};
	void startVisit(const COMPILE_SHADER* pCOMPILE_SHADER) {};
	void startVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	void startVisit(const PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	void startVisit(const IN* pIN) {};
	void startVisit(const OUT* pOUT) {};
	void startVisit(const INOUT* pINOUT) {};
	void startVisit(const UNIFORM* pUNIFORM) {};
	void startVisit(const VARIABLE* pVARIABLE) {};
	void startVisit(const VARIABLE_DECL* pVARIABLE_DECL) {};
	void startVisit(const BINARY_MINUS* pBINARY_MINUS) {};
	void startVisit(const BINARY_PLUS* pBINARY_PLUS) {};
	void startVisit(const BINARY_DIVIDE* pBINARY_DIVIDE) {};
	void startVisit(const BINARY_MULTIPLY* pBINARY_MULTIPLY) {};
	void startVisit(const UNARY_MINUS* pUNARY_MINUS) {};
	void startVisit(const UNARY_PLUS* pUNARY_PLUS) {};
	void startVisit(const ASSIGNMENT* pASSIGNMENT) {};
	void startVisit(const TECHNIQUE11* pTECHNIQUE11) {};
	void startVisit(const PASS* pPASS) {};
	void startVisit(const ROUND_BRACKETS* pROUND_BRACKETS) {};
	void startVisit(const SQUARE_BRACKETS* pSQUARE_BRACKETS) {};
	void startVisit(const CURLY_BRACKETS* pCURLY_BRACKETS) {};
	void startVisit(const RETURN* pRETURN) {};
	void startVisit(const SHADER* pSHADER) {};
	void startVisit(const ShaderComponent* pShaderComponent) {};

	void finishVisit(const CBUFFER* pCBUFFER) {};
	void finishVisit(const FLOAT4X4* pFLOAT4X4) {};
	void finishVisit(const VOID* pVOID) {};
	void finishVisit(const FLOAT* pFLOAT) {};
	void finishVisit(const FLOAT2* pFLOAT2) {};
	void finishVisit(const FLOAT3* pFLOAT3) {};
	void finishVisit(const FLOAT4* pFLOAT4) {};
	void finishVisit(const STRUCT* pSTRUCT) {};
	void finishVisit(const SEMANTIC* pSEMANTIC) {};
	void finishVisit(const SV_POSITION* pSV_POSITION) {};
	void finishVisit(const SV_TARGET* pSV_TARGET) {};
	void finishVisit(const FUNCTION_DECL* pFUNCTION_DECL) {};
	void finishVisit(const FUNCTION_CALL* pFUNCTION_CALL) {};
	void finishVisit(const MUL* pMUL) {};
	void finishVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	void finishVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};
	void finishVisit(const SET_PIXEL_SHADER* pSET_PIXEL_SHADER) {};
	void finishVisit(const COMPILE_SHADER* pCOMPILE_SHADER) {};
	void finishVisit(const VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	void finishVisit(const PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	void finishVisit(const IN* pIN) {};
	void finishVisit(const OUT* pOUT) {};
	void finishVisit(const INOUT* pINOUT) {};
	void finishVisit(const UNIFORM* pUNIFORM) {};
	void finishVisit(const VARIABLE* pVARIABLE) {};
	void finishVisit(const VARIABLE_DECL* pVARIABLE_DECL) {};
	void finishVisit(const BINARY_MINUS* pBINARY_MINUS) {};
	void finishVisit(const BINARY_PLUS* pBINARY_PLUS) {};
	void finishVisit(const BINARY_DIVIDE* pBINARY_DIVIDE) {};
	void finishVisit(const BINARY_MULTIPLY* pBINARY_MULTIPLY) {};
	void finishVisit(const UNARY_MINUS* pUNARY_MINUS) {};
	void finishVisit(const UNARY_PLUS* pUNARY_PLUS) {};
	void finishVisit(const ASSIGNMENT* pASSIGNMENT) {};
	void finishVisit(const TECHNIQUE11* pTECHNIQUE11) {};
	void finishVisit(const PASS* pPASS) {};
	void finishVisit(const ROUND_BRACKETS* pROUND_BRACKETS) {};
	void finishVisit(const SQUARE_BRACKETS* pSQUARE_BRACKETS) {};
	void finishVisit(const CURLY_BRACKETS* pCURLY_BRACKETS) {};
	void finishVisit(const RETURN* pRETURN) {};
	void finishVisit(const SHADER* pSHADER) {};
	void finishVisit(const ShaderComponent* pShaderComponent) {};
};

int main()
{
    std::cout << "Hello World!\n";
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
