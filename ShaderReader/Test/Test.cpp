#include <iostream>
#include "ShaderVisitor.h"
#include "ShaderReader.h"
#include "HLSLConverter.h"

class PrintVisitor : public ShaderVisitor
{
	int numberOfSpaces = 0;
	void printSpaces()
	{
		for (int i = 0; i < numberOfSpaces; ++i)
			std::cout << ' ';
	};
public:
	void startVisit(const ShaderUnits::CBUFFER* pCBUFFER)
	{
		printSpaces();
		std::cout << "CBUFFER " << pCBUFFER->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SAMPLER_STATE* pSAMPLER_STATE)
	{
		printSpaces();
		std::cout << "SamplerState " << pSAMPLER_STATE->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FLOAT3X3* pFLOAT3X3)
	{
		printSpaces();
		std::cout << "float3x3" << std::endl;
	};
	void startVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4)
	{
		printSpaces();
		std::cout << "float4x4" << std::endl;
	};
	void startVisit(const ShaderUnits::VOID_* pVOID)
	{
		printSpaces();
		std::cout << "void" << std::endl;
	};
	void startVisit(const ShaderUnits::FLOAT1* pFLOAT)
	{
		printSpaces();
		std::cout << "float" << std::endl;
	};
	void startVisit(const ShaderUnits::FLOAT2* pFLOAT2)
	{
		printSpaces();
		std::cout << "float2" << std::endl;
	};
	void startVisit(const ShaderUnits::FLOAT3* pFLOAT3)
	{
		printSpaces();
		std::cout << "float3" << std::endl;
	};
	void startVisit(const ShaderUnits::FLOAT4* pFLOAT4)
	{
		printSpaces();
		std::cout << "float4" << std::endl;
	};
	void startVisit(const ShaderUnits::STRUCT* pSTRUCT)
	{
		printSpaces();
		std::cout << "struct " << pSTRUCT->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::TEXTURE2D* pTexture2D)
	{
		printSpaces();
		std::cout << "Texture2D" << std::endl;
	}
	void startVisit(const ShaderUnits::SEMANTIC* pSEMANTIC)
	{
		printSpaces();
		std::cout << "user semantic " << pSEMANTIC->getName() << std::endl;
	};
	void startVisit(const ShaderUnits::SV_POSITION* pSV_POSITION)
	{
		printSpaces();
		std::cout << "SV_POSITION" << std::endl;
	};
	void startVisit(const ShaderUnits::SV_TARGET* pSV_TARGET)
	{
		printSpaces();
		std::cout << "SV_TARGET" << std::endl;
	};
	void startVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
	{
		printSpaces();
		std::cout << "function declaration " << pFUNCTION_DECL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL)
	{
		printSpaces();
		std::cout << "function call " << pFUNCTION_CALL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::MUL* pMUL)
	{
		printSpaces();
		std::cout << "mul" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FLOAT3_CONSTRUCTOR* pFLOAT3_CONSTRUCTOR)
	{
		printSpaces();
		std::cout << "constructor of float3" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR)
	{
		printSpaces();
		std::cout << "constructor of float4" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SET_VERTEX_SHADER* pSET_VERTEX_SHADER)
	{
		printSpaces();
		std::cout << "set_vertex_shader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SET_PIXEL_SHADER* pSET_PIXEL_SHADER)
	{
		printSpaces();
		std::cout << "set_pixel_shader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::COMPILE_SHADER* pCOMPILE_SHADER)
	{
		printSpaces();
		std::cout << "CompileShader" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION)
	{
		printSpaces();
		std::cout << pVERTEX_SHADER_VERSION->getName() << std::endl;
	};
	void startVisit(const ShaderUnits::PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION)
	{
		printSpaces();
		std::cout << pPIXEL_SHADER_VERSION->getName() << std::endl;
	};
	void startVisit(const ShaderUnits::IN_MODIFIER* pIN)
	{
		printSpaces();
		std::cout << "in" << std::endl;
	};
	void startVisit(const ShaderUnits::OUT_MODIFIER* pOUT)
	{
		printSpaces();
		std::cout << "out" << std::endl;
	};
	void startVisit(const ShaderUnits::INOUT_MODIFIER* pINOUT)
	{
		printSpaces();
		std::cout << "inout" << std::endl;
	};
	void startVisit(const ShaderUnits::UNIFORM* pUNIFORM)
	{
		printSpaces();
		std::cout << "uniform" << std::endl;
	};
	void startVisit(const ShaderUnits::VARIABLE* pVARIABLE)
	{
		printSpaces();
		std::cout << "Variable " << pVARIABLE->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::NUMBER* pNUMBER)
	{
		printSpaces();
		std::cout << "Number " << pNUMBER->getName() << std::endl;
	};
	void startVisit(const ShaderUnits::COMMENT* pCOMMENT)
	{
		printSpaces();
		std::cout << "Comment " << pCOMMENT->getName() << std::endl;
	};
	void startVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
	{
		printSpaces();
		std::cout << "Decl of variable " << pVARIABLE_DECL->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::BINARY_MINUS* pBINARY_MINUS)
	{
		printSpaces();
		std::cout << "-" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::BINARY_PLUS* pBINARY_PLUS)
	{
		printSpaces();
		std::cout << "+" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::BINARY_DIVIDE* pBINARY_DIVIDE)
	{
		printSpaces();
		std::cout << "/" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::BINARY_MULTIPLY* pBINARY_MULTIPLY)
	{
		printSpaces();
		std::cout << "*" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::GREATER_THAN* pGREATER_THAN)
	{
		printSpaces();
		std::cout << ">" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::UNARY_MINUS* pUNARY_MINUS)
	{
		printSpaces();
		std::cout << "-" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::UNARY_PLUS* pUNARY_PLUS)
	{
		printSpaces();
		std::cout << "+" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::ASSIGNMENT* pASSIGNMENT)
	{
		printSpaces();
		std::cout << "=" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::ADDITION_ASSIGN* pADDITION_ASSIGN)
	{
		printSpaces();
		std::cout << "+=" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::DIVIDES_ASSIGN* pDIVIDES_ASSIGN)
	{
		printSpaces();
		std::cout << "/=" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::MULTIPLIES_ASSIGN* pMULTIPLIES_ASSIGN)
	{
		printSpaces();
		std::cout << "*=" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11)
	{
		printSpaces();
		std::cout << "technique11 " << pTECHNIQUE11->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::PASS* pPASS)
	{
		printSpaces();
		std::cout << "pass " << pPASS->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::ROUND_BRACKETS* pROUND_BRACKETS)
	{
		printSpaces();
		std::cout << "(" << pROUND_BRACKETS->getName() << ")" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS)
	{
		printSpaces();
		std::cout << "[]" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::CURLY_BRACKETS* pCURLY_BRACKETS)
	{
		printSpaces();
		std::cout << "{" << pCURLY_BRACKETS->getName() << "}" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::RETURN* pRETURN)
	{
		printSpaces();
		std::cout << "return" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SHADER* pSHADER)
	{
		printSpaces();
		std::cout << "SHADER:" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::ShaderComponent* pShaderComponent) {};
	void startVisit(const ShaderUnits::IF_NODE* pIF_NODE)
	{
		printSpaces();
		std::cout << "if" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FOR* pFOR)
	{
		printSpaces();
		std::cout << "for" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::INIT_EXPRESSION* pINIT_EXPRESSION)
	{
		printSpaces();
		std::cout << "init-expression" << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::FLATTEN* pFLATTEN)
	{
		printSpaces();
		std::cout << "[flatten]" << std::endl;
	}
	void startVisit(const ShaderUnits::UNROLL* pUNROLL)
	{
		printSpaces();
		std::cout << "[unroll]" << std::endl;
	}
	void startVisit(const ShaderUnits::SELECTED_FIELD* pSELECTED_FIELD)
	{
		printSpaces();
		std::cout << "selected field: " << pSELECTED_FIELD->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::SELECTED_METHOD* pSELECTED_METHOD)
	{
		printSpaces();
		std::cout << "selected method: " << pSELECTED_METHOD->getName() << std::endl;
		numberOfSpaces += 4;
	};
	void startVisit(const ShaderUnits::CAST* pCAST)
	{
		printSpaces();
		std::cout << "CASTING" << std::endl;
		numberOfSpaces += 4;
	};

	void finishVisit(const ShaderUnits::CBUFFER* pCBUFFER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SAMPLER_STATE* pSAMPLER_STATE)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FLOAT4X4* pFLOAT4X4) {};
	void finishVisit(const ShaderUnits::VOID_* pVOID) {};
	void finishVisit(const ShaderUnits::FLOAT1* pFLOAT) {};
	void finishVisit(const ShaderUnits::FLOAT2* pFLOAT2) {};
	void finishVisit(const ShaderUnits::FLOAT3* pFLOAT3) {};
	void finishVisit(const ShaderUnits::FLOAT4* pFLOAT4) {};
	void finishVisit(const ShaderUnits::TEXTURE2D* pTEXTURE2D) {};
	void finishVisit(const ShaderUnits::STRUCT* pSTRUCT)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SEMANTIC* pSEMANTIC) {};
	void finishVisit(const ShaderUnits::SV_POSITION* pSV_POSITION) {};
	void finishVisit(const ShaderUnits::SV_TARGET* pSV_TARGET) {};
	void finishVisit(const ShaderUnits::FUNCTION_DECL* pFUNCTION_DECL)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FUNCTION_CALL* pFUNCTION_CALL)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::MUL* pMUL)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FLOAT3_CONSTRUCTOR* pFLOAT3_CONSTRUCTOR)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SET_VERTEX_SHADER* pSET_VERTEX_SHADER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SET_PIXEL_SHADER* pSET_PIXEL_SHADER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::COMPILE_SHADER* pCOMPILE_SHADER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::VERTEX_SHADER_VERSION* pVERTEX_SHADER_VERSION) {};
	void finishVisit(const ShaderUnits::PIXEL_SHADER_VERSION* pPIXEL_SHADER_VERSION) {};
	void finishVisit(const ShaderUnits::IN_MODIFIER* pIN) {};
	void finishVisit(const ShaderUnits::OUT_MODIFIER* pOUT) {};
	void finishVisit(const ShaderUnits::INOUT_MODIFIER* pINOUT) {};
	void finishVisit(const ShaderUnits::UNIFORM* pUNIFORM) {};
	void finishVisit(const ShaderUnits::VARIABLE* pVARIABLE)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::NUMBER* pNUMBER) {};
	void finishVisit(const ShaderUnits::COMMENT* pCOMMENT) {};
	void finishVisit(const ShaderUnits::VARIABLE_DECL* pVARIABLE_DECL)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::BINARY_MINUS* pBINARY_MINUS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::BINARY_PLUS* pBINARY_PLUS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::BINARY_DIVIDE* pBINARY_DIVIDE)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::BINARY_MULTIPLY* pBINARY_MULTIPLY)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::GREATER_THAN* pGREATER_THAN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::UNARY_MINUS* pUNARY_MINUS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::UNARY_PLUS* pUNARY_PLUS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::ASSIGNMENT* pASSIGNMENT)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::ADDITION_ASSIGN* pADDITION_ASSIGN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::DIVIDES_ASSIGN* pDIVIDES_ASSIGN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::MULTIPLIES_ASSIGN* pMULTIPLIES_ASSIGN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::TECHNIQUE11* pTECHNIQUE11)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::PASS* pPASS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::ROUND_BRACKETS* pROUND_BRACKETS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SQUARE_BRACKETS* pSQUARE_BRACKETS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::CURLY_BRACKETS* pCURLY_BRACKETS)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::RETURN* pRETURN)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SHADER* pSHADER)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::ShaderComponent* pShaderComponent) {};
	void finishVisit(const ShaderUnits::IF_NODE* pIF_NODE)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FOR* pFOR)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::INIT_EXPRESSION* pINIT_EXPRESSION)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::FLATTEN* pFLATTEN) {};
	void finishVisit(const ShaderUnits::UNROLL* pUNROLL) {};
	void finishVisit(const ShaderUnits::SELECTED_FIELD* pSELECTED_FIELD)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::SELECTED_METHOD* pSELECTED_METHOD)
	{
		numberOfSpaces -= 4;
	};
	void finishVisit(const ShaderUnits::CAST* pCAST)
	{
		numberOfSpaces -= 4;
	};
};

int main()
{
	ShaderReader reader;
	reader.openShader(L"C:\\3dEngine\\Shaders\\LightingDemo.fx");

	HLSLConverter converter;
	reader.parseShader(converter);

	HLSLShader hlslShader;
	converter.getShader(hlslShader);

	PrintVisitor visitor;
	hlslShader.query(&visitor);
}
