#include "SphereCreator.h"
#include "Selector.h"
#include "mathUtils.h"
#include "MainScene.h"
#include "cameras.h"
#include "Utils.h"

void SphereCreator::init()
{
	sphereFrameworkGeo.setTechnique(StringManager::toStringId("SphereFramework"));
	sphereFrameworkGeo.setPass(p0_pass_id);

	const char* depth_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	sphereFrameworkGeo.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	sphereFrameworkGeo.gpuReadyData = &sphereFramework;
	sphereFrameworkGeo.elementSize = sizeof SphereFramework;
	sphereFrameworkGeo.verticesCount = 1;

	bInitialized = true;
}

Modifier::Behaviour SphereCreator::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		switch (currentState)
		{
		case State::Initial:
		{
			flt3 segV0;
			flt3 segV1;
			calculateRay(hWnd, lparam, segV0, segV1);

			Selector::instance()->selectObject(segV0, segV1, true);
			if (Selector::instance()->selectedObjectsCount > 0)
			{
				uint32_t objectIndex = Selector::instance()->selectedObject;
				uint32_t triangleIndex = Selector::instance()->selectedTriangle;

				flt3 v0;
				flt3 v1;
				flt3 v2;
				MainScene::instance()->getTriangle(
					objectIndex, triangleIndex, v0, v1, v2);

				float t = 0;
				flt2 barycentricCoords;
				checkIntersection(v0, v1, v2, segV0, segV1, t, barycentricCoords);

				sphereFramework.posW =
					barycentricCoords.x() * v0 +
					barycentricCoords.y() * v1 +
					(1 - barycentricCoords.x() - barycentricCoords.y()) * v2;

				flt3 s0 = v1 - v0;
				flt3 s1 = v2 - v0;

				axis2 = cross(s1, s0);
				axis0 = cross(flt3(0.0f, 1.0f, 0.0f), axis2);
				axis1 = cross(axis2, axis0);

				axis0.normalize();
				axis1.normalize();
				axis2.normalize();

				sphereFramework.axis0 = flt3(0.0f, 0.0f, 0.0f);
				sphereFramework.axis1 = flt3(0.0f, 0.0f, 0.0f);
				sphereFramework.axis2 = flt3(0.0f, 0.0f, 0.0f);

				currentState = State::DeterminingOfOxyProjection;

				return Behaviour::FINISH;
			}
			else if (linePlaneIntersection(
				segV0, segV1 - segV0,
				flt3(0.0f, 0.0f, 0.0f), flt3(1.0f, 0.0f, 0.0f), flt3(0.0f, 0.0f, 1.0f),
				sphereFramework.posW
			))
			{
				axis0 = flt3(1.0f, 0.0f, 0.0f);
				axis1 = flt3(0.0f, 0.0f, 1.0f);
				axis2 = flt3(0.0f, 1.0f, 0.0f);

				sphereFramework.posW =
					sphereFramework.posW.x() * axis0 +
					sphereFramework.posW.y() * axis1;

				sphereFramework.axis0 = flt3(0.0f, 0.0f, 0.0f);
				sphereFramework.axis1 = flt3(0.0f, 0.0f, 0.0f);
				sphereFramework.axis2 = flt3(0.0f, 0.0f, 0.0f);

				currentState = State::DeterminingOfOxyProjection;

				return Behaviour::FINISH;
			}
			
			return Behaviour::CONTINUE;
		}
		case State::DeterminingOfHeight:
		{
			currentState = State::Insertion;

			currentState = State::Initial;

			return Behaviour::FINISH;
		}
		}
		return Behaviour::CONTINUE;
	}
	case WM_MOUSEMOVE:
	{
		flt3 segV0;
		flt3 segV1;
		calculateRay(hWnd, lparam, segV0, segV1);

		switch (currentState)
		{
		case State::DeterminingOfOxyProjection:
		{
			flt3 pointOfIntersection;
			linePlaneIntersection(
				segV0, segV1 - segV0,
				sphereFramework.posW, axis0, axis1,
				pointOfIntersection
			);

			sphereFramework.axis0 = axis0;
			sphereFramework.axis0 *= pointOfIntersection.x();

			sphereFramework.axis1 = axis1;
			sphereFramework.axis1 *= pointOfIntersection.y();

			return Behaviour::FINISH;
		}
		case State::DeterminingOfHeight:
		{
			float t = 0;
			distanceBetweenLines(
				sphereFramework.posW +
				sphereFramework.axis0 +
				sphereFramework.axis1, axis2,
				segV0, segV1 - segV0,
				&t
			);

			sphereFramework.axis2 = axis2;
			sphereFramework.axis2 *= t;

			return Behaviour::FINISH;
		}
		default:
			return Behaviour::CONTINUE;
		}
		return Behaviour::CONTINUE;
	}
	case WM_LBUTTONUP:
	{
		switch (currentState)
		{
		case State::DeterminingOfOxyProjection:
		{
			currentState = State::DeterminingOfHeight;
			return Behaviour::FINISH;
		}
		// case...
		}

		return Behaviour::CONTINUE;;
	}
	default:
		return Behaviour::CONTINUE;
	}
	return Behaviour::FINISH;
}

void SphereCreator::draw()
{
	if (currentState != State::Initial)
	{
		GraphicsCore::instance()->draw(sphereFrameworkGeo);
	}
}