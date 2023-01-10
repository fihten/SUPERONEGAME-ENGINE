cbuffer cbPerObject
{
	float4x4 WVP;
}

struct VertexOut
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
};

void f(float a);
 
void VSdemo(float3 iPosL:POSITION, float4 iColor:COLOR, out float4 oPosH:SV_POSITION, out float4 oColor:COLOR)
{
	// Normalize the light vector.
	lightVec /= d;

	diffuse *= att;

	if (a > b)
	{
		a = b;
		c = ((100 * ((a * b - t) * 10 - 20)) + 50) * andrusha;
	}
	else
	{
		if (1)
		{
			float a = 10;
		}
	}
	if (a > b)
		if (1)
			w;
		else
			if (5 > 5)
				t = 5;
			else
				if (t > t)
					t = 6;
				else
					r = i;
	else
		if (u)
			e = o + t;
		else
			w = w - r;
	
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}

	float a = (1 + ((5 + a) * l + 10)) * 100;
	oPosH = mul(float4(iPosL,1), WVP);
	oColor = iColor;	
}

float4 PSdemo(float4 posH:SV_POSITION, float4 color:COLOR):SV_Target
{
	return color;
}

technique11 Demo
{
	pass P0
	{
		SetVertexShader(CompileShader(vs_5_0, VSdemo()));
		SetPixelShader(CompileShader(ps_5_0, PSdemo()));
	}
}
