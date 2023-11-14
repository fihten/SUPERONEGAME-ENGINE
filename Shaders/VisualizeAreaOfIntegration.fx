Texture2D arrow;

struct VertexIn
{
	float2 pos : Position;
};

int2 centerOfArrow;
int radiusOfArrow;

float angle0;
float scale0;

float angle1;
float scale1;

int width;
int height;
float ndcOffsetY;

struct VertexOut
{
	float2 ndc : SV_Position;
	float2 uv : TexCoord;
};

VertexOut vs(VertexIn vin)
{
	float3x3 m;
	m[0][0] = radiusOfArrow * scale0 * cos(angle0);
	m[0][1] = radiusOfArrow * scale0 * sin(angle0);
	m[0][2] = 0;
	m[1][0] = -radiusOfArrow * scale1 * sin(angle1);
	m[1][1] = radiusOfArrow * scale1 * cos(angle1);
	m[1][2] = 0;
	m[2][0] = centerOfArrow.x; m[2][1] = centerOfArrow.y; m[2][2] = 1;

	float3x3 toNDC;

}