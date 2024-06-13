StructuredBuffer<float> Rc;
StructuredBuffer<float> Ac;
StructuredBuffer<float> Bc;

StructuredBuffer<float> Ad;
StructuredBuffer<float> Bd;
StructuredBuffer<float> Cd;

StructuredBuffer<float> R;
StructuredBuffer<float> A;
StructuredBuffer<float> B;

StructuredBuffer<float2> pointsOnPhotos;
StructuredBuffer<uint2> mapToVertexAndCamera;

RWStructuredBuffer<float3> I;
StructuredBuffer<float3> Iin;

RWStructuredBuffer<float3> J;
StructuredBuffer<float3> Jin;

RWStructuredBuffer<float3> K;
StructuredBuffer<float3> Kin;

RWStructuredBuffer<float3> xyzc;
StructuredBuffer<float3> xyzc_in;

RWStructuredBuffer<float3> xyz;
StructuredBuffer<float3> xyz_in;

RWStructuredBuffer<uint> error;

RWStructuredBuffer<float3> dIdA;
StructuredBuffer<float3> dIdAin;

RWStructuredBuffer<float3> dIdB;
StructuredBuffer<float3> dIdBin;

RWStructuredBuffer<float3> dIdC;
StructuredBuffer<float3> dIdCin;

RWStructuredBuffer<float3> dJdA;
StructuredBuffer<float3> dJdAin;

RWStructuredBuffer<float3> dJdB;
StructuredBuffer<float3> dJdBin;

RWStructuredBuffer<float3> dJdC;
StructuredBuffer<float3> dJdCin;

RWStructuredBuffer<float3> dKdA;
StructuredBuffer<float3> dKdAin;

RWStructuredBuffer<float3> dKdB;
StructuredBuffer<float3> dKdBin;

RWStructuredBuffer<float3> dKdC;
StructuredBuffer<float3> dKdCin;

RWStructuredBuffer<float3> dXYZCdR;
StructuredBuffer<float3> dXYZCdRin;

RWStructuredBuffer<float3> dXYZCdA;
StructuredBuffer<float3> dXYZCdAin;

RWStructuredBuffer<float3> dXYZCdB;
StructuredBuffer<float3> dXYZCdBin;

RWStructuredBuffer<float3> dXYZdR;
StructuredBuffer<float3> dXYZdRin;

RWStructuredBuffer<float3> dXYZdA;
StructuredBuffer<float3> dXYZdAin;

RWStructuredBuffer<float3> dXYZdB;
StructuredBuffer<float3> dXYZdBin;

RWStructuredBuffer<int> gradError_r;
StructuredBuffer<int> gradError_r_in;

RWStructuredBuffer<int> gradError_a;
StructuredBuffer<int> gradError_a_in;

RWStructuredBuffer<uint> minGradComponent_a;

int amountOfCameras;
int amountOfVertices;
int amountOfPointsOnPhotos;

float t_r;
float t_a;

void calculateGradient(
	float3 rv,
	float2 pt,
	float3 r, float3 rc,
	float3 i, float3 j, float3 k,
	int index,
	RWStructuredBuffer<int> grad
)
{
	float2 e0 = rv.xy - rv.z * pt;

	float3 rv_;
	rv_.x = dot(r - rc, i);
	rv_.y = dot(r - rc, j);
	rv_.z = dot(r - rc, k);

	float2 e1 = rv_.xy - rv_.z * pt;
	float e = dot(e0, e1);

	int d = 1000000;
	int iE = floor(e);
	int iEfraction = (e - iE) * d;

	int originalValue = 0;
	InterlockedAdd(grad[index + 0], iE, originalValue);
	InterlockedAdd(grad[index + 1], iEfraction, originalValue);
}

uint RcId(uint cameraId)
{
	return cameraId * 2;
}

uint Rid(uint vertexId)
{
	return amountOfCameras * 2 + vertexId * 2;
}

uint AcId(uint cameraId)
{
	return cameraId * 10;
}

uint BcId(uint cameraId)
{
	return cameraId * 10 + 2;
}

uint AdId(uint cameraId)
{
	return cameraId * 10 + 4;
}

uint BdId(uint cameraId)
{
	return cameraId * 10 + 6;
}

uint CdId(uint cameraId)
{
	return cameraId * 10 + 8;
}

uint Aid(uint vertexId)
{
	return amountOfCameras * 10 + vertexId * 4;
}

uint Bid(uint vertexId)
{
	return amountOfCameras * 10 + vertexId * 4 + 2;
}

float fetchGrad(uint index, StructuredBuffer<int> grad)
{
	int i = grad[index].x;
	int ifraction = grad[index + 1].x;
	
	float d = 1000000.0f;
	float f = (float)(i)+(float)(ifraction) / d;

	return f;
}

[numthreads(64, 1, 1)]
void cs_calculate_axis_I(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	float gradA = fetchGrad(AdId(i), gradError_a_in);
	float gradB = fetchGrad(BdId(i), gradError_a_in);
	float gradC = fetchGrad(CdId(i), gradError_a_in);

	float a = Ad[i] + t_a * gradA;
	float b = Bd[i] + t_a * gradB;
	float c = Cd[i] + t_a * gradC;

	I[i].x = cos(b) * cos(c) - cos(a) * sin(b) * sin(c);
	I[i].y = sin(b) * cos(c) + cos(a) * cos(b) * sin(c);
	I[i].z = sin(a) * sin(c);
}

[numthreads(64, 1, 1)]
void cs_calculate_axis_J(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	float gradA = fetchGrad(AdId(i), gradError_a_in);
	float gradB = fetchGrad(BdId(i), gradError_a_in);
	float gradC = fetchGrad(CdId(i), gradError_a_in);

	float a = Ad[i] + t_a * gradA;
	float b = Bd[i] + t_a * gradB;
	float c = Cd[i] + t_a * gradC;

	J[i].x = -cos(b) * sin(c) - cos(a) * sin(b) * cos(c);
	J[i].y = -sin(b) * sin(c) + cos(a) * cos(b) * cos(c);
	J[i].z = sin(a) * cos(c);
}

[numthreads(64, 1, 1)]
void cs_calculate_axis_K(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	float gradA = fetchGrad(AdId(i), gradError_a_in);
	float gradB = fetchGrad(BdId(i), gradError_a_in);
	float gradC = fetchGrad(CdId(i), gradError_a_in);

	float a = Ad[i] + t_a * gradA;
	float b = Bd[i] + t_a * gradB;
	float c = Cd[i] + t_a * gradC;

	K[i].x = sin(a) * sin(b);
	K[i].y = -sin(a) * cos(b);
	K[i].z = cos(a);
}

[numthreads(64, 1, 1)]
void cs_calculate_xyzc(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	float gradR = fetchGrad(RcId(i), gradError_r_in);
	float gradA = fetchGrad(AcId(i), gradError_a_in);
	float gradB = fetchGrad(BcId(i), gradError_a_in);

	float r = Rc[i] + t_r * gradR;
	float a = Ac[i] + t_a * gradA;
	float b = Bc[i] + t_a * gradB;

	xyzc[i].x = r * sin(b) * cos(a);
	xyzc[i].y = r * sin(b) * sin(a);
	xyzc[i].z = r * cos(b);
}

[numthreads(64, 1, 1)]
void cs_calculate_xyz(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfVertices)
		return;

	float gradR = fetchGrad(Rid(i), gradError_r_in);
	float gradA = fetchGrad(Aid(i), gradError_a_in);
	float gradB = fetchGrad(Bid(i), gradError_a_in);

	float r = R[i] + t_r * gradR;
	float a = A[i] + t_a * gradA;
	float b = B[i] + t_a * gradB;

	xyz[i].x = r * sin(b) * cos(a);
	xyz[i].y = r * sin(b) * sin(a);
	xyz[i].z = r * cos(b);
}

[numthreads(64, 1, 1)]
void cs_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfPointsOnPhotos)
		return;

	int vertexId = mapToVertexAndCamera[i].x;
	float3 xyz_ = xyz_in[vertexId];
	
	int cameraId = mapToVertexAndCamera[i].y;
	float3 xyzc_ = xyzc_in[cameraId];
	float3 I_ = Iin[cameraId];
	float3 J_ = Jin[cameraId];
	float3 K_ = Kin[cameraId];

	float3 xyzv;
	xyzv.x = dot(xyz_ - xyzc_, I_);
	xyzv.y = dot(xyz_ - xyzc_, J_);
	xyzv.z = dot(xyz_ - xyzc_, K_);

	float2 pointOnPhoto = pointsOnPhotos[i];

	float2 e2 = xyzv.xy - xyzv.z * pointOnPhoto;
	float e = dot(e2, e2);

	float d = 1000000;
	uint uiE = floor(e);
	uint uiEfraction = (e - uiE) * d;

	uint originalValue = 0;
	InterlockedAdd(error[0], uiE, originalValue);
	InterlockedAdd(error[1], uiEfraction, originalValue);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_axis_I(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	//I[i].x = cos(Bd[i]) * cos(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * sin(Cd[i]);
	//I[i].y = sin(Bd[i]) * cos(Cd[i]) + cos(Ad[i]) * cos(Bd[i]) * sin(Cd[i]);
	//I[i].z = sin(Ad[i]) * sin(Cd[i]);

	dIdA[i].x = sin(Ad[i]) * sin(Bd[i]) * sin(Cd[i]);
	dIdA[i].y = -sin(Ad[i]) * cos(Bd[i]) * sin(Cd[i]);
	dIdA[i].z = cos(Ad[i]) * sin(Cd[i]);

	dIdB[i].x = -sin(Bd[i]) * cos(Cd[i]) - cos(Ad[i]) * cos(Bd[i]) * sin(Cd[i]);
	dIdB[i].y = cos(Bd[i]) * cos(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * sin(Cd[i]);
	dIdB[i].z = 0;

	dIdC[i].x = -cos(Bd[i]) * sin(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * cos(Cd[i]);
	dIdC[i].y = -sin(Bd[i]) * sin(Cd[i]) + cos(Ad[i]) * cos(Bd[i]) * cos(Cd[i]);
	dIdC[i].z = sin(Ad[i]) * cos(Cd[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_axis_J(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	//J[i].x = -cos(Bd[i]) * sin(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * cos(Cd[i]);
	//J[i].y = -sin(Bd[i]) * sin(Cd[i]) + cos(Ad[i]) * cos(Bd[i]) * cos(Cd[i]);
	//J[i].z = sin(Ad[i]) * cos(Cd[i]);

	dJdA[i].x = sin(Ad[i]) * sin(Bd[i]) * cos(Cd[i]);
	dJdA[i].y = -sin(Ad[i]) * cos(Bd[i]) * cos(Cd[i]);
	dJdA[i].z = cos(Ad[i]) * cos(Cd[i]);

	dJdB[i].x = sin(Bd[i]) * sin(Cd[i]) - cos(Ad[i]) * cos(Bd[i]) * cos(Cd[i]);
	dJdB[i].y = -cos(Bd[i]) * sin(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * cos(Cd[i]);
	dJdB[i].z = 0;

	dJdC[i].x = -cos(Bd[i]) * cos(Cd[i]) + cos(Ad[i]) * sin(Bd[i]) * sin(Cd[i]);
	dJdC[i].y = -sin(Bd[i]) * cos(Cd[i]) - cos(Ad[i]) * cos(Bd[i]) * sin(Cd[i]);
	dJdC[i].z = -sin(Ad[i]) * sin(Cd[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_axis_K(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	//K[i].x = sin(Ad[i]) * sin(Bd[i]);
	//K[i].y = -sin(Ad[i]) * cos(Bd[i]);
	//K[i].z = cos(Ad[i]);

	dKdA[i].x = cos(Ad[i]) * sin(Bd[i]);
	dKdA[i].y = -cos(Ad[i]) * cos(Bd[i]);
	dKdA[i].z = -sin(Ad[i]);

	dKdB[i].x = sin(Ad[i]) * cos(Bd[i]);
	dKdB[i].y = sin(Ad[i]) * sin(Bd[i]);
	dKdB[i].z = 0;

	dKdC[i].x = 0;
	dKdC[i].y = 0;
	dKdC[i].z = 0;
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_xyzc_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	dXYZCdR[i].x = sin(Bc[i]) * cos(Ac[i]);
	dXYZCdR[i].y = sin(Bc[i]) * sin(Ac[i]);
	dXYZCdR[i].z = cos(Bc[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_xyzc_a(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	//xyzc[i].x = Rc[i] * sin(Bc[i]) * cos(Ac[i]);
	//xyzc[i].y = Rc[i] * sin(Bc[i]) * sin(Ac[i]);
	//xyzc[i].z = Rc[i] * cos(Bc[i]);

	dXYZCdA[i].x = -Rc[i] * sin(Bc[i]) * sin(Ac[i]);
	dXYZCdA[i].y = Rc[i] * sin(Bc[i]) * cos(Ac[i]);
	dXYZCdA[i].z = 0;

	dXYZCdB[i].x = Rc[i] * cos(Bc[i]) * cos(Ac[i]);
	dXYZCdB[i].y = Rc[i] * cos(Bc[i]) * sin(Ac[i]);
	dXYZCdB[i].z = -Rc[i] * sin(Bc[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_xyz_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfVertices)
		return;

	dXYZdR[i].x = sin(B[i]) * cos(A[i]);
	dXYZdR[i].y = sin(B[i]) * sin(A[i]);
	dXYZdR[i].z = cos(B[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_xyz_a(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfVertices)
		return;

	//xyz[i].x = R[i] * sin(B[i]) * cos(A[i]);
	//xyz[i].y = R[i] * sin(B[i]) * sin(A[i]);
	//xyz[i].z = R[i] * cos(B[i]);

	dXYZdA[i].x = -R[i] * sin(B[i]) * sin(A[i]);
	dXYZdA[i].y = R[i] * sin(B[i]) * cos(A[i]);
	dXYZdA[i].z = 0;

	dXYZdB[i].x = R[i] * cos(B[i]) * cos(A[i]);
	dXYZdB[i].y = R[i] * cos(B[i]) * sin(A[i]);
	dXYZdB[i].z = -R[i] * sin(B[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_error_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfPointsOnPhotos)
		return;

	uint vertexId = mapToVertexAndCamera[i].x;
	uint cameraId = mapToVertexAndCamera[i].y;
	
	float3 I_ = Iin[cameraId];
	float3 J_ = Jin[cameraId];
	float3 K_ = Kin[cameraId];
	
	float3 xyz_ = xyz_in[vertexId].xyz;
	float3 xyzc_ = xyzc_in[cameraId].xyz;

	float2 pt = pointsOnPhotos[i];

	float3 rv;
	rv.x = dot(xyz_ - xyzc_, I_);
	rv.y = dot(xyz_ - xyzc_, J_);
	rv.z = dot(xyz_ - xyzc_, K_);

	calculateGradient(
		rv,
		pt,
		0, dXYZCdRin[cameraId],
		I_, J_, K_,
		RcId(cameraId),
		gradError_r
	);
	calculateGradient(
		rv,
		pt,
		dXYZdRin[vertexId], 0,
		I_, J_, K_,
		Rid(vertexId),
		gradError_r
	);
}

[numthreads(64, 1, 1)]
void cs_calculate_grad_of_error_a(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfPointsOnPhotos)
		return;

	uint vertexId = mapToVertexAndCamera[i].x;
	uint cameraId = mapToVertexAndCamera[i].y;

	float3 I_ = Iin[cameraId].xyz;
	float3 J_ = Jin[cameraId].xyz;
	float3 K_ = Kin[cameraId].xyz;

	float3 xyz_ = xyz_in[vertexId].xyz;
	float3 xyzc_ = xyzc_in[cameraId].xyz;

	float3 rv;
	rv.x = dot(xyz_ - xyzc_, I_);
	rv.y = dot(xyz_ - xyzc_, J_);
	rv.z = dot(xyz_ - xyzc_, K_);

	float2 pt = pointsOnPhotos[i];
	
	// Cameras
	calculateGradient(
		rv,
		pt,
		0, dXYZCdAin[cameraId],
		I_, J_, K_,
		AcId(cameraId),
		gradError_a
	);
	calculateGradient(
		rv,
		pt,
		0, dXYZCdBin[cameraId],
		I_, J_, K_,
		BcId(cameraId),
		gradError_a
	);
	calculateGradient(
		rv,
		pt,
		xyz_, xyzc_,
		dIdAin[cameraId], dJdAin[cameraId], dKdAin[cameraId],
		AdId(cameraId),
		gradError_a
	);
	calculateGradient(
		rv,
		pt,
		xyz_, xyzc_,
		dIdBin[cameraId], dJdBin[cameraId], dKdBin[cameraId],
		BdId(cameraId),
		gradError_a
	);
	calculateGradient(
		rv,
		pt,
		xyz_, xyzc_,
		dIdCin[cameraId], dJdCin[cameraId], dKdCin[cameraId],
		CdId(cameraId),
		gradError_a
	);
	
	// Vertices
	calculateGradient(
		v,
		pt,
		dXYZdAin[vertexId], 0,
		I_, J_, K_,
		Aid(vertexId),
		gradError_a
	);
	calculateGradient(
		v,
		pt,
		dXYZdBin[vertexId], 0,
		I_, J_, K_,
		Bid(vertexId),
		gradError_a
	);
}

[numthreads(64, 1, 1)]
void cs_calculate_min_grad_component_a(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint i = dispatchThreadID.x;
	if (i >= amountOfCameras + amountOfVertices)
		return;

	float minGrad = 0;
	if (i < amountOfCameras)
	{
		float gradAc = abs(fetchGrad(AcId(i), gradError_a_in));
		float gradBc = abs(fetchGrad(BcId(i), gradError_a_in));

		float gradAd = abs(fetchGrad(AdId(i), gradError_a_in));
		float gradBd = abs(fetchGrad(BdId(i), gradError_a_in));
		float gradCd = abs(fetchGrad(CdId(i), gradError_a_in));

		minGrad = min(gradAc, gradBc);
		minGrad = min(minGrad, gradAd);
		minGrad = min(minGrad, gradBd);
		minGrad = min(minGrad, gradCd);
	}
	else
	{
		i -= amountOfCameras;

		float gradA = abs(fetchGrad(Aid(i), gradError_a_in));
		float gradB = abs(fetchGrad(Bid(i), gradError_a_in));

		minGrad = min(gradA, gradB);
	}

	float d = 1000000.0f;
	uint ui = minGrad * d;

	uint originalValue = 0;
	InterlockedMin(minGradComponent_a[0], ui, originalValue);
}