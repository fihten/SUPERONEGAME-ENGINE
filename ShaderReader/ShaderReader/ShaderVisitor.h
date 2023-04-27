#pragma once

#include "ShaderComponent.h"

class ShaderVisitor
{
public:
	virtual void startVisit(const ShaderUnits::CBUFFER* pCBUFFER) {};
	virtual void startVisit(const ShaderUnits::SAMPLER_STATE* pSAMPLER_STATE) {};
	virtual void startVisit(const ShaderUnits::FLOAT3X3* pFLOAT3X3) {};
	virtual void startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4) {};
	virtual void startVisit(const ShaderUnits::VOID_* pVOID) {};
	virtual void startVisit(const ShaderUnits::INT1* pINT) {};
	virtual void startVisit(const ShaderUnits::FLOAT1* pFLOAT) {};
	virtual void startVisit(const ShaderUnits::FLOAT2* pFLOAT2) {};
	virtual void startVisit(const ShaderUnits::FLOAT3* pFLOAT3) {};
	virtual void startVisit(const ShaderUnits::FLOAT4* pFLOAT4) {};
	virtual void startVisit(const ShaderUnits::STRUCT* pSTRUCT) {};
	virtual void startVisit(const ShaderUnits::TEXTURE2D* pTEXTURE2D) {};
	virtual void startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC) {};
	virtual void startVisit(const ShaderUnits::SV_POSITION* pSV_POSITION) {};
	virtual void startVisit(const ShaderUnits::SV_TARGET* pSV_TARGET) {};
	virtual void startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL) {};
	virtual void startVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL) {};
	virtual void startVisit(const ShaderUnits::MUL* pMUL) {};
	virtual void startVisit(const ShaderUnits::FLOAT3_CONSTRUCTOR* pFLOAT3_CONSTRUCTOR) {};
	virtual void startVisit(const ShaderUnits::FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	virtual void startVisit(const ShaderUnits::SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};
	virtual void startVisit(const ShaderUnits::SET_PIXEL_SHADER* pSET_PIXEL_SHADER) {};
	virtual void startVisit(const ShaderUnits::COMPILE_SHADER* pCOMPILE_SHADER) {};
	virtual void startVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	virtual void startVisit(const ShaderUnits::PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	virtual void startVisit(const ShaderUnits::IN_MODIFIER* pIN) {};
	virtual void startVisit(const ShaderUnits::OUT_MODIFIER* pOUT) {};
	virtual void startVisit(const ShaderUnits::INOUT_MODIFIER* pINOUT) {};
	virtual void startVisit(const ShaderUnits::UNIFORM* pUNIFORM) {};
	virtual void startVisit(const ShaderUnits::VARIABLE* pVARIABLE) {};
	virtual void startVisit(const ShaderUnits::NUMBER* pNUMBER) {};
	virtual void startVisit(const ShaderUnits::COMMENT* pCOMMENT) {};
	virtual void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL) {};
	virtual void startVisit(const ShaderUnits::BINARY_MINUS* pBINARY_MINUS) {};
	virtual void startVisit(const ShaderUnits::BINARY_PLUS* pBINARY_PLUS) {};
	virtual void startVisit(const ShaderUnits::BINARY_DIVIDE* pBINARY_DIVIDE) {};
	virtual void startVisit(const ShaderUnits::BINARY_MULTIPLY* pBINARY_MULTIPLY) {};
	virtual void startVisit(const ShaderUnits::GREATER_THAN* pGREATER_THAN) {};
	virtual void startVisit(const ShaderUnits::UNARY_MINUS* pUNARY_MINUS) {};
	virtual void startVisit(const ShaderUnits::UNARY_PLUS* pUNARY_PLUS) {};
	virtual void startVisit(const ShaderUnits::ASSIGNMENT* pASSIGNMENT) {};
	virtual void startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11) {};
	virtual void startVisit(const ShaderUnits::PASS* pPASS) {};
	virtual void startVisit(const ShaderUnits::ROUND_BRACKETS* pROUND_BRACKETS) {};
	virtual void startVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS) {};
	virtual void startVisit(const ShaderUnits::CURLY_BRACKETS* pCURLY_BRACKETS) {};
	virtual void startVisit(const ShaderUnits::RETURN* pRETURN) {};
	virtual void startVisit(const ShaderUnits::SHADER* pSHADER) {};
	virtual void startVisit(const ShaderUnits::ShaderComponent* pShaderComponent) {};
	virtual void startVisit(const ShaderUnits::IF_NODE* pIF_NODE) {};
	virtual void startVisit(const ShaderUnits::FOR* pFOR) {};
	virtual void startVisit(const ShaderUnits::INIT_EXPRESSION* pINIT_EXPRESSION) {};
	virtual void startVisit(const ShaderUnits::COND_EXPRESSION* pCOND_EXPRESSION) {};
	virtual void startVisit(const ShaderUnits::LOOP_EXPRESSION* pLOOP_EXPRESSION) {};
	virtual void startVisit(const ShaderUnits::FLATTEN* pFLATTEN) {};
	virtual void startVisit(const ShaderUnits::UNROLL* pUNROLL) {};
	virtual void startVisit(const ShaderUnits::ADDITION_ASSIGN* pADDITION_ASSIGN) {};
	virtual void startVisit(const ShaderUnits::DIVIDES_ASSIGN* pDIVIDES_ASSIGN) {};
	virtual void startVisit(const ShaderUnits::MULTIPLIES_ASSIGN* pMULTIPLIES_ASSIGN) {};
	virtual void startVisit(const ShaderUnits::SELECTED_FIELD* pSELECTED_FIELD) {};
	virtual void startVisit(const ShaderUnits::SELECTED_METHOD* pSELECTED_METHOD) {};
	virtual void startVisit(const ShaderUnits::CAST* pCAST) {};

	virtual void finishVisit(const ShaderUnits::CBUFFER* pCBUFFER) {};
	virtual void finishVisit(const ShaderUnits::SAMPLER_STATE* pSAMPLER_STATE) {};
	virtual void finishVisit(const ShaderUnits::FLOAT3X3* pFloat3X3) {};
	virtual void finishVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4) {};
	virtual void finishVisit(const ShaderUnits::VOID_* pVOID) {};
	virtual void finishVisit(const ShaderUnits::INT1* pINT) {};
	virtual void finishVisit(const ShaderUnits::FLOAT1* pFLOAT) {};
	virtual void finishVisit(const ShaderUnits::FLOAT2* pFLOAT2) {};
	virtual void finishVisit(const ShaderUnits::FLOAT3* pFLOAT3) {};
	virtual void finishVisit(const ShaderUnits::FLOAT4* pFLOAT4) {};
	virtual void finishVisit(const ShaderUnits::STRUCT* pSTRUCT) {};
	virtual void finishVisit(const ShaderUnits::TEXTURE2D* pTEXTURE2D) {};
	virtual void finishVisit(const ShaderUnits::SEMANTIC* pSEMANTIC) {};
	virtual void finishVisit(const ShaderUnits::SV_POSITION* pSV_POSITION) {};
	virtual void finishVisit(const ShaderUnits::SV_TARGET* pSV_TARGET) {};
	virtual void finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL) {};
	virtual void finishVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL) {};
	virtual void finishVisit(const ShaderUnits::MUL* pMUL) {};
	virtual void finishVisit(const ShaderUnits::FLOAT3_CONSTRUCTOR* pFLOAT3_CONSTRUCTOR) {};
	virtual void finishVisit(const ShaderUnits::FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	virtual void finishVisit(const ShaderUnits::SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};
	virtual void finishVisit(const ShaderUnits::SET_PIXEL_SHADER* pSET_PIXEL_SHADER) {};
	virtual void finishVisit(const ShaderUnits::COMPILE_SHADER* pCOMPILE_SHADER) {};
	virtual void finishVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	virtual void finishVisit(const ShaderUnits::PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	virtual void finishVisit(const ShaderUnits::IN_MODIFIER* pIN) {};
	virtual void finishVisit(const ShaderUnits::OUT_MODIFIER* pOUT) {};
	virtual void finishVisit(const ShaderUnits::INOUT_MODIFIER* pINOUT) {};
	virtual void finishVisit(const ShaderUnits::UNIFORM* pUNIFORM) {};
	virtual void finishVisit(const ShaderUnits::VARIABLE* pVARIABLE) {};
	virtual void finishVisit(const ShaderUnits::NUMBER* pNUMBER) {};
	virtual void finishVisit(const ShaderUnits::COMMENT* pCOMMENT) {};
	virtual void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL) {};
	virtual void finishVisit(const ShaderUnits::BINARY_MINUS* pBINARY_MINUS) {};
	virtual void finishVisit(const ShaderUnits::BINARY_PLUS* pBINARY_PLUS) {};
	virtual void finishVisit(const ShaderUnits::BINARY_DIVIDE* pBINARY_DIVIDE) {};
	virtual void finishVisit(const ShaderUnits::BINARY_MULTIPLY* pBINARY_MULTIPLY) {};
	virtual void finishVisit(const ShaderUnits::GREATER_THAN* pGREATER_THAN) {};
	virtual void finishVisit(const ShaderUnits::UNARY_MINUS* pUNARY_MINUS) {};
	virtual void finishVisit(const ShaderUnits::UNARY_PLUS* pUNARY_PLUS) {};
	virtual void finishVisit(const ShaderUnits::ASSIGNMENT* pASSIGNMENT) {};
	virtual void finishVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11) {};
	virtual void finishVisit(const ShaderUnits::PASS* pPASS) {};
	virtual void finishVisit(const ShaderUnits::ROUND_BRACKETS* pROUND_BRACKETS) {};
	virtual void finishVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS) {};
	virtual void finishVisit(const ShaderUnits::CURLY_BRACKETS* pCURLY_BRACKETS) {};
	virtual void finishVisit(const ShaderUnits::RETURN* pRETURN) {};
	virtual void finishVisit(const ShaderUnits::SHADER* pSHADER) {};
	virtual void finishVisit(const ShaderUnits::ShaderComponent* pShaderComponent) {};
	virtual void finishVisit(const ShaderUnits::IF_NODE* pIF_NODE) {};
	virtual void finishVisit(const ShaderUnits::FOR* pFOR) {};
	virtual void finishVisit(const ShaderUnits::INIT_EXPRESSION* pINIT_EXPRESSION) {};
	virtual void finishVisit(const ShaderUnits::COND_EXPRESSION* pCOND_EXPRESSION) {};
	virtual void finishVisit(const ShaderUnits::LOOP_EXPRESSION* pLOOP_EXPRESSION) {};
	virtual void finishVisit(const ShaderUnits::FLATTEN* pFLATTEN) {};
	virtual void finishVisit(const ShaderUnits::UNROLL* pUNROLL) {};
	virtual void finishVisit(const ShaderUnits::ADDITION_ASSIGN* pADDITION_ASSIGN) {};
	virtual void finishVisit(const ShaderUnits::DIVIDES_ASSIGN* pDIVIDES_ASSIGN) {};
	virtual void finishVisit(const ShaderUnits::MULTIPLIES_ASSIGN* pMULTIPLIES_ASSIGN) {};
	virtual void finishVisit(const ShaderUnits::SELECTED_FIELD* pSELECTED_FIELD) {};
	virtual void finishVisit(const ShaderUnits::SELECTED_METHOD* pSELECTED_METHOD) {};
	virtual void finishVisit(const ShaderUnits::CAST* pCAST) {};
};