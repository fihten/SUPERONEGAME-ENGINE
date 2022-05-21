#include "ShaderComponent.h"

class ShaderVisitor
{
public:
	virtual void startVisit(const CBUFFER* pCBUFFER) {};
	virtual void startVisit(const FLOAT4X4* pFLOAT4X4) {};
	virtual void startVisit(const VOID* pVOID) {};
	virtual void startVisit(const FLOAT* pFLOAT) {};
	virtual void startVisit(const FLOAT2* pFLOAT2) {};
	virtual void startVisit(const FLOAT3* pFLOAT3) {};
	virtual void startVisit(const FLOAT4* pFLOAT4) {};
	virtual void startVisit(const STRUCT* pSTRUCT) {};
	virtual void startVisit(const SEMANTIC* pSEMANTIC) {};
	virtual void startVisit(const SV_POSITION* pSV_POSITION) {};
	virtual void startVisit(const SV_TARGET* pSV_TARGET) {};
	virtual void startVisit(const FUNCTION_DECL* pFUNCTION_DECL) {};
	virtual void startVisit(const FUNCTION_CALL* pFUNCTION_CALL) {};
	virtual void startVisit(const MUL* pMUL) {};
	virtual void startVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	virtual void startVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};

	virtual void finishVisit(const CBUFFER* pCBUFFER) {};
	virtual void finishVisit(const FLOAT4X4* pFLOAT4X4) {};
	virtual void finishVisit(const VOID* pVOID) {};
	virtual void finishVisit(const FLOAT* pFLOAT) {};
	virtual void finishVisit(const FLOAT2* pFLOAT2) {};
	virtual void finishVisit(const FLOAT3* pFLOAT3) {};
	virtual void finishVisit(const FLOAT4* pFLOAT4) {};
	virtual void finishVisit(const STRUCT* pSTRUCT) {};
	virtual void finishVisit(const SEMANTIC* pSEMANTIC) {};
	virtual void finishVisit(const SV_POSITION* pSV_POSITION) {};
	virtual void finishVisit(const SV_TARGET* pSV_TARGET) {};
	virtual void finishVisit(const FUNCTION_DECL* pFUNCTION_DECL) {};
	virtual void finishVisit(const FUNCTION_CALL* pFUNCTION_CALL) {};
	virtual void finishVisit(const MUL* pMUL) {};
	virtual void finishVisit(const FLOAT4_CONSTRUCTOR* pFLOAT4_CONSTRUCTOR) {};
	virtual void finishVisit(const SET_VERTEX_SHADER* pSET_VERTEX_SHADER) {};
};