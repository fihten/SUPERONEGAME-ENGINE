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
RWStructuredBuffer<float3> J;
RWStructuredBuffer<float3> K;

RWStructuredBuffer<float3> xyzc;
RWStructuredBuffer<float3> xyz;

RWStructuredBuffer<uint> error;

int amountOfCameras;
int amountOfVertices;
int amountOfPointsOnPhotos;

[numthreads(64, 1, 1)]
void cs_calculate_axis_I(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	I[i].x = cos(Bd[i]) * cos(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * sin(Cd[i]);
	I[i].y = sin(Bd[i]) * cos(Cd[i]) + cos(Ad[i]) * cos(Bd[i]) * sin(Cd[i]);
	I[i].z = sin(Ad[i]) * sin(Cd[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_axis_J(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	J[i].x = -cos(Bd[i]) * sin(Cd[i]) - cos(Ad[i]) * sin(Bd[i]) * cos(Cd[i]);
	J[i].y = -sin(Bd[i]) * sin(Cd[i]) + cos(Ad[i]) * cos(Bd[i]) * cos(Cd[i]);
	J[i].z = sin(Ad[i]) * cos(Cd[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_axis_K(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	K[i].x = sin(Ad[i]) * sin(Bd[i]);
	K[i].y = -sin(Ad[i]) * cos(Bd[i]);
	K[i].z = cos(Ad[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_xyzc(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfCameras)
		return;

	xyzc[i].x = Rc[i] * sin(Bc[i]) * cos(Ac[i]);
	xyzc[i].y = Rc[i] * sin(Bc[i]) * sin(Ac[i]);
	xyzc[i].z = Rc[i] * cos(Bc[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_xyz(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfVertices)
		return;

	xyz[i].x = R[i] * sin(B[i]) * cos(A[i]);
	xyz[i].y = R[i] * sin(B[i]) * sin(A[i]);
	xyz[i].z = R[i] * cos(B[i]);
}

[numthreads(64, 1, 1)]
void cs_calculate_error(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfPointsOnPhotos)
		return;

	int vertexId = mapToVertexAndCamera[i].x;
	float3 xyz_ = xyz[vertexId];
	
	int cameraId = mapToVertexAndCamera[i].y;
	float3 xyzc_ = xyzc[cameraId];
	float3 I_ = I[cameraId];
	float3 J_ = J[cameraId];
	float3 K_ = K[cameraId];

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
}