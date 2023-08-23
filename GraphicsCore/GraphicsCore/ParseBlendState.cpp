#include "ParseBlendState.h"
#include <algorithm>

bool extractBlendParamDefinition(const char* szBlendState, uint32_t start, uint32_t& end)
{
	uint32_t len = std::strlen(szBlendState);
	if (start >= len)
		return false;
	end = std::find(szBlendState + start, szBlendState + len, ';') - szBlendState;
	return true;
}

void extractBlendParamName(const char* szBlendParamDefinition, uint32_t& blendParamNameEnd)
{
	uint32_t len = std::strlen(szBlendParamDefinition);
	blendParamNameEnd = std::find(szBlendParamDefinition, szBlendParamDefinition + len, '=') - szBlendParamDefinition;
}

int extractBlendParamIndex(const char* szBlendParamDefinition)
{
	uint32_t len = std::strlen(szBlendParamDefinition);
	
	uint32_t p0 = std::find(szBlendParamDefinition, szBlendParamDefinition + len, '[') - szBlendParamDefinition;
	uint32_t p1 = std::find(szBlendParamDefinition, szBlendParamDefinition + len, ']') - szBlendParamDefinition;

	char buffer[2];
	std::copy(szBlendParamDefinition + p0 + 1, szBlendParamDefinition + p1, buffer);
	buffer[1] = 0;

	return std::atoi(buffer);
}

void extractBlendParamField(const char* szBlendParamDefinition, uint32_t& start)
{
	start = 0;
	uint32_t len = std::strlen(szBlendParamDefinition);
	start = std::find(szBlendParamDefinition + start, szBlendParamDefinition + len, '.') - szBlendParamDefinition;
	start++;
}

void setBooleanParam(
	const char* paramName,
	const char* szBlendParamDefinition,
	const char* szBlendParamNameEnd,
	const char* szBlendParamValueEnd,
	BOOL& param
)
{
	bool bEqual =
		std::equal(
			paramName, paramName + std::strlen(paramName),
			szBlendParamDefinition, szBlendParamNameEnd
		);
	if (!bEqual)
		return;

	const char* str = "TRUE";
	param =
		std::equal(
			str, str + std::strlen(str),
			szBlendParamNameEnd + 1, szBlendParamValueEnd
		);
}

template<class T>
void setRenderTarget(
	const char* szFieldName,
	T D3D11_RENDER_TARGET_BLEND_DESC::*field,
	const char* szValue,
	T value,
	const char* szBlendParamDefinition,
	const char* szBlendParamNameEnd,
	const char* szBlendParamValueEnd,
	D3D11_RENDER_TARGET_BLEND_DESC* renderTarget
)
{
	const char* szParamName = "RenderTarget";
	bool bEqual =
		std::equal(
			szParamName, szParamName + std::strlen(szParamName),
			szBlendParamDefinition, szBlendParamDefinition + std::strlen(szParamName)
		);
	if (!bEqual)
		return;
	
	int index = extractBlendParamIndex(szBlendParamDefinition);

	uint32_t fieldStart = 0;
	extractBlendParamField(szBlendParamDefinition, fieldStart);
	const char* szBlendParamField = szBlendParamDefinition + fieldStart;

	bEqual =
		std::equal(
			szFieldName, szFieldName + std::strlen(szFieldName),
			szBlendParamField, szBlendParamNameEnd
		);
	if (!bEqual)
		return;

	if (szValue)
	{
		bEqual =
			std::equal(
				szValue, szValue + std::strlen(szValue),
				szBlendParamNameEnd + 1, szBlendParamValueEnd
			);
		if (!bEqual)
			return;
	}

	renderTarget[index].*field = value;
}

void parseBlendState(const char* szBlendState, D3D11_BLEND_DESC& blendDesc)
{
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x00;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;

	for (int i = 0; i < 8; i++)
		blendDesc.RenderTarget[i] = blendDesc.RenderTarget[0];

	uint32_t start = 0;
	uint32_t end = 0;
	while (extractBlendParamDefinition(szBlendState, start, end))
	{
		const char* szBlendParamDefinition = szBlendState + start;

		uint32_t paramNameEnd = 0;
		extractBlendParamName(szBlendParamDefinition, paramNameEnd);

		const char* szBlendParamNameEnd = szBlendParamDefinition + paramNameEnd;
		const char* szBlendParamValueEnd = szBlendState + end;
		//--------------------------------------------------------------
		setBooleanParam(
			"AlphaToCoverageEnable",
			szBlendParamDefinition,
			szBlendParamDefinition + paramNameEnd,
			szBlendState + end,
			blendDesc.AlphaToCoverageEnable
		);
		//--------------------------------------------------------------
		setBooleanParam(
			"IndependentBlendEnable",
			szBlendParamDefinition,
			szBlendParamDefinition + paramNameEnd,
			szBlendState + end,
			blendDesc.IndependentBlendEnable
		);
		//--------------------------------------------------------------
		setRenderTarget(
			"BlendEnable",
			&D3D11_RENDER_TARGET_BLEND_DESC::BlendEnable,
			"TRUE",
			TRUE,
			szBlendParamDefinition,
			szBlendParamNameEnd,
			szBlendParamValueEnd,
			blendDesc.RenderTarget
		);
		//--------------------------------------------------------------
		setRenderTarget(
			"BlendEnable",
			&D3D11_RENDER_TARGET_BLEND_DESC::BlendEnable,
			"FALSE",
			FALSE,
			szBlendParamDefinition,
			szBlendParamNameEnd,
			szBlendParamValueEnd,
			blendDesc.RenderTarget
		);
		//--------------------------------------------------------------
		typedef D3D11_BLEND D3D11_RENDER_TARGET_BLEND_DESC::* PTR_BLEND;
		std::pair<const char*, PTR_BLEND> blendFields[] =
		{
			{"SrcBlend",&D3D11_RENDER_TARGET_BLEND_DESC::SrcBlend},
			{"DestBlend",&D3D11_RENDER_TARGET_BLEND_DESC::DestBlend},
			{"SrcBlendAlpha",&D3D11_RENDER_TARGET_BLEND_DESC::SrcBlendAlpha},
			{"DestBlendAlpha",&D3D11_RENDER_TARGET_BLEND_DESC::DestBlendAlpha}
		};
		std::pair<const char*, D3D11_BLEND> blendValues[] =
		{
			{"D3D11_BLEND_ZERO", D3D11_BLEND_ZERO},
			{"D3D11_BLEND_ONE",D3D11_BLEND_ONE},
			{"D3D11_BLEND_SRC_COLOR",D3D11_BLEND_SRC_COLOR},
			{"D3D11_BLEND_INV_SRC_COLOR",D3D11_BLEND_INV_SRC_COLOR},
			{"D3D11_BLEND_SRC_ALPHA", D3D11_BLEND_SRC_ALPHA},
			{"D3D11_BLEND_INV_SRC_ALPHA",D3D11_BLEND_INV_SRC_ALPHA},
			{"D3D11_BLEND_DEST_ALPHA",D3D11_BLEND_DEST_ALPHA},
			{"D3D11_BLEND_INV_DEST_ALPHA",D3D11_BLEND_INV_DEST_ALPHA},
			{"D3D11_BLEND_DEST_COLOR",D3D11_BLEND_DEST_COLOR},
			{"D3D11_BLEND_INV_DEST_COLOR",D3D11_BLEND_INV_DEST_COLOR},
			{"D3D11_BLEND_SRC_ALPHA_SAT",D3D11_BLEND_SRC_ALPHA_SAT},
			{"D3D11_BLEND_BLEND_FACTOR",D3D11_BLEND_BLEND_FACTOR},
			{"D3D11_BLEND_INV_BLEND_FACTOR",D3D11_BLEND_INV_BLEND_FACTOR},
			{"D3D11_BLEND_SRC1_COLOR",D3D11_BLEND_SRC1_COLOR},
			{"D3D11_BLEND_INV_SRC1_COLOR",D3D11_BLEND_INV_SRC1_COLOR},
			{"D3D11_BLEND_SRC1_ALPHA",D3D11_BLEND_SRC1_ALPHA},
			{"D3D11_BLEND_INV_SRC1_ALPHA",D3D11_BLEND_INV_SRC1_ALPHA}
		};
		for (int i = 0; i < sizeof(blendFields) / sizeof(*blendFields); i++)
		{
			for (int j = 0; j < sizeof(blendValues) / sizeof(*blendValues); j++)
			{
				setRenderTarget(
					blendFields[i].first,
					blendFields[i].second,
					blendValues[j].first,
					blendValues[j].second,
					szBlendParamDefinition,
					szBlendParamNameEnd,
					szBlendParamValueEnd,
					blendDesc.RenderTarget
				);
			}
		}
		//--------------------------------------------------------------
		typedef D3D11_BLEND_OP D3D11_RENDER_TARGET_BLEND_DESC::* PTR_BLEND_OP;
		std::pair<const char*, PTR_BLEND_OP> blendOpFields[] =
		{
			{"BlendOp",&D3D11_RENDER_TARGET_BLEND_DESC::BlendOp},
			{"BlendOpAlpha",&D3D11_RENDER_TARGET_BLEND_DESC::BlendOpAlpha}
		};
		std::pair<const char*, D3D11_BLEND_OP> blendOpValues[] =
		{
			{"D3D11_BLEND_OP_ADD",D3D11_BLEND_OP_ADD},
			{"D3D11_BLEND_OP_SUBTRACT",D3D11_BLEND_OP_SUBTRACT},
			{"D3D11_BLEND_OP_REV_SUBTRACT",D3D11_BLEND_OP_REV_SUBTRACT},
			{"D3D11_BLEND_OP_MIN",D3D11_BLEND_OP_MIN},
			{"D3D11_BLEND_OP_MAX",D3D11_BLEND_OP_MAX}
		};
		for (int i = 0; i < sizeof(blendOpFields) / sizeof(*blendOpFields); i++)
		{
			for (int j = 0; j < sizeof(blendOpValues) / sizeof(*blendOpValues); j++)
			{
				setRenderTarget(
					blendOpFields[i].first,
					blendOpFields[i].second,
					blendOpValues[j].first,
					blendOpValues[j].second,
					szBlendParamDefinition,
					szBlendParamNameEnd,
					szBlendParamValueEnd,
					blendDesc.RenderTarget
				);
			}
		}
		//--------------------------------------------------------------
		char str[256] = {0};
		std::memcpy(str, szBlendParamNameEnd + 1, szBlendParamValueEnd - szBlendParamNameEnd - 1);
		setRenderTarget<UINT8>(
			"RenderTargetWriteMask",
			&D3D11_RENDER_TARGET_BLEND_DESC::RenderTargetWriteMask,
			0,
			std::atoi(str),
			szBlendParamDefinition,
			szBlendParamNameEnd,
			szBlendParamValueEnd,
			blendDesc.RenderTarget
		);

		start = end + 1;
	}
}