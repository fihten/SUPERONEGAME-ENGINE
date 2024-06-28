#include "GenerateVerticesAndMapping.h"
#include "Vec4d.h"
#include "Matrix4x4.h"
#include <cmath>
#include <algorithm>

void generateVerticesAndMapping(
	std::vector<flt2>& points,
	std::vector<Vec2d<uint32_t>>& mapping
)
{
	int N = 50;
	uint32_t verticesCount = (N + 1)*(N + 1);
	uint32_t camerasCount = 3;

	float A = 20;
	float B = 4;
	float C = 1;

	float alpha0 = M_PI / 3;
	float alpha1 = 2 * M_PI / 3;

	float beta0 = -M_PI / 6;
	float beta1 = M_PI / 6;

	std::vector<flt3> vertices;
	for (int i = 0; i < N + 1; i++)
	{
		float alpha = alpha0 * (1.0f - (float)(i) / N) + alpha1 * (float)(i) / N;
		for (int j = 0; j < N + 1; j++)
		{
			float beta = beta0 * (1.0f - (float)(j) / N) + beta1 * (float)(j) / N;
			vertices.emplace_back(
				A * sin(alpha) * cos(beta),
				B * sin(alpha) * sin(beta),
				C * cos(alpha)
			);
		}
	}
	std::vector<Matrix4x4<float>> views;
	for (int i = 0; i < camerasCount; i++)
	{
		float alpha = 0.5 * M_PI;
		float beta = beta0 * (1.0f - (float)(i) / (camerasCount - 1)) + beta1 * (float)(i) / (camerasCount - 1);
		float r = 4 * std::max<float>(A, std::max<float>(B, C));

		flt3 p(
			r * sin(alpha) * cos(beta),
			r * sin(alpha) * sin(beta),
			r * cos(alpha)
		);

		flt3 K(
			-sin(alpha) * cos(beta),
			-sin(alpha) * sin(beta),
			-cos(alpha)
		);
		flt3 J(0, 0, 1.0f);
		flt3 I = cross(K, J);

		views.emplace_back(
			I.x(), J.x(), K.x(), 0.0f,
			I.y(), J.y(), K.y(), 0.0f,
			I.z(), J.z(), K.z(), 0.0f,
			-dot(I, p), -dot(J, p), -dot(K, p), 1.0f
		);
	}

	for (int vi = 0; vi < verticesCount; vi++)
	{
		for (int ci = 0; ci < camerasCount; ci++)
		{
			auto& v = vertices[vi];
			auto& c = views[ci];

			flt4 v4(v.x(), v.y(), v.z(), 1.0f);
			v4 = v4 * c;
			v4 = v4 / v4.z();

			points.emplace_back(v4.x(), v4.y());
			mapping.emplace_back(vi, ci);
		}
	}
}