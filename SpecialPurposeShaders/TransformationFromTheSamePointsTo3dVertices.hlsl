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

RWStructuredBuffer<float3> dIdA;
RWStructuredBuffer<float3> dIdB;
RWStructuredBuffer<float3> dIdC;

RWStructuredBuffer<float3> dJdA;
RWStructuredBuffer<float3> dJdB;
RWStructuredBuffer<float3> dJdC;

RWStructuredBuffer<float3> dKdA;
RWStructuredBuffer<float3> dKdB;
RWStructuredBuffer<float3> dKdC;

RWStructuredBuffer<float3> dXYZCdR;
RWStructuredBuffer<float3> dXYZCdA;
RWStructuredBuffer<float3> dXYZCdB;

RWStructuredBuffer<float3> dXYZdR;
RWStructuredBuffer<float3> dXYZdA;
RWStructuredBuffer<float3> dXYZdB;

RWStructuredBuffer<uint> gradError_r;
RWStructuredBuffer<uint> gradError_a;

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

[numthreads(32, 32, 1)]
void cs_calculate_grad_of_error_r(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	int i = dispatchThreadID.x;
	if (i >= amountOfPointsOnPhotos)
		return;
	
	int j = dispatchThreadID.y;
	if (j >= amountOfCameras + amountOfVertices)
		return;

	uint vertexId = mapToVertexAndCamera[i].x;
	uint cameraId = mapToVertexAndCamera[i].y;
	
	float3 I_ = I[cameraId];
	float3 J_ = J[cameraId];
	float3 K_ = K[cameraId];
	
	float3 xyz_ = xyz[vertexId].xyz;
	float3 xyzc_ = xyzc[cameraId].xyz;

	float2 pt = pointsOnPhotos[i];

	float3 rv;
	rv.x = dot(xyz_ - xyzc_, I_);
	rv.y = dot(xyz_ - xyzc_, J_);
	rv.z = dot(xyz_ - xyzc_, K_);

	if (j < amountOfCameras)
	{
		if (j != cameraId)
			return;

		float3 dXYZCdR_ = dXYZCdR[cameraId].xyz;
		
		float3 rv_;
		rv_.x = -dot(dXYZCdR_, I_);
		rv_.y = -dot(dXYZCdR_, J_);
		rv_.z = -dot(dXYZCdR_, K_);

		float2 e0 = rv.xy - pt * rv.z;
		float2 e1 = rv_.xy - pt * rv_.z;
		float e = dot(e0, e1);

		d = 1000000;
		uint uiE = floor(e);
		uint uiEfraction = (e - uiE) * d;

		uint originalValue = 0;
		InterlockedAdd(gradError_r[0], uiE, originalValue);
		InterlockedAdd(gradError_r[1], uiEfraction, originalValue);
	}
	else
	{
		j -= amountOfCameras;
		if (j != vertexId)
			return;

	}

}