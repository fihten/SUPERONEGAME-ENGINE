#include "ConeCreator.h"
#include "Selector.h"
#include "cameras.h"
#include "Utils.h"
#include "MainScene.h"
#include "MathUtils.h"
#include <algorithm>

void ConeCreator::init()
{
	Creator::init();

	coneFrameworkGeo.setTechnique(StringManager::toStringId("ConeFramework"));
	coneFrameworkGeo.setPass(p0_pass_id);

	const char* depth_stencil_state = "DepthEnable=false;DepthWriteMask=D3D11_DEPTH_WRITE_MASK_ZERO;DepthFunc=D3D11_COMPARISON_ALWAYS;StencilEnable=false;StencilReadMask=0;StencilWriteMask=0;FrontFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;FrontFace.StencilFunc=D3D11_COMPARISON_ALWAYS;BackFace.StencilFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilDepthFailOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilPassOp=D3D11_STENCIL_OP_KEEP;BackFace.StencilFunc=D3D11_COMPARISON_ALWAYS;";
	coneFrameworkGeo.setDepthStencilState(StringManager::toStringId(depth_stencil_state));

	coneFrameworkGeo.gpuReadyData = &coneFramework;
	coneFrameworkGeo.elementSize = sizeof ConeFramework;
	coneFrameworkGeo.verticesCount = 1;

	bInitialized = true;
}

Modifier::Behaviour ConeCreator::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		flt3 segV0;
		flt3 segV1;
		calculateRay(hWnd, lparam, segV0, segV1);

		switch (currentState)
		{
		case State::Initial:
		{
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

				coneFramework.posW =
					barycentricCoords.x() * v0 +
					barycentricCoords.y() * v1 +
					(1 - barycentricCoords.x() - barycentricCoords.y()) * v2;

				flt3 s0 = v1 - v0;
				flt3 s1 = v2 - v0;

				axis2 = cross(s1, s0);
				axis0 = cross(flt3(0.0f, 1.0f, 0.0f), axis2);
				if (axis0.length() == 0) {
					axis0 = cross(flt3(1.0f, 0.0f, 0.0f), axis2);
				}
				axis1 = cross(axis0, axis2);

				axis0.normalize();
				axis1.normalize();
				axis2.normalize();

				coneFramework.axis0 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.axis1 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.axis2 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.scaleFromBottomToTop = 1;

				currentState = State::DeterminingOfOxyProjectionOfBottom;

				return Behaviour::FINISH;
			}
			else if (linePlaneIntersection(
				segV0, segV1 - segV0,
				flt3(0.0f, 0.0f, 0.0f), flt3(1.0f, 0.0f, 0.0f), flt3(0.0f, 0.0f, 1.0f),
				coneFramework.posW
			))
			{
				axis0 = flt3(1.0f, 0.0f, 0.0f);
				axis1 = flt3(0.0f, 0.0f, 1.0f);
				axis2 = flt3(0.0f, 1.0f, 0.0f);

				coneFramework.posW =
					coneFramework.posW.x() * axis0 +
					coneFramework.posW.y() * axis1;

				coneFramework.axis0 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.axis1 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.axis2 = flt3(0.0f, 0.0f, 0.0f);
				coneFramework.scaleFromBottomToTop = 1;

				currentState = State::DeterminingOfOxyProjectionOfBottom;

				return Behaviour::FINISH;
			}

			return Behaviour::CONTINUE;
		}
		case State::DeterminingOfHeight:
		{
			currentState = State::DeterminingOfOxyProjectionOfTop;
			return Behaviour::CONTINUE;
		}
		}
	}
	case WM_MOUSEMOVE:
	{
		flt3 segV0;
		flt3 segV1;
		calculateRay(hWnd, lparam, segV0, segV1);

		switch (currentState)
		{
		case State::DeterminingOfOxyProjectionOfBottom:
		{
			flt3 pointOfIntersection;
			linePlaneIntersection(
				segV0, segV1 - segV0,
				coneFramework.posW, axis0, axis1,
				pointOfIntersection
			);

			coneFramework.axis0 = axis0;
			coneFramework.axis0 *= pointOfIntersection.x();

			coneFramework.axis1 = axis1;
			coneFramework.axis1 *= pointOfIntersection.y();

			return Behaviour::FINISH;
		}
		case State::DeterminingOfHeight:
		{
			float t = 0;
			distanceBetweenLines(
				coneFramework.posW +
				coneFramework.axis0 +
				coneFramework.axis1, axis2,
				segV0, segV1 - segV0,
				&t
			);

			coneFramework.axis2 = axis2;
			coneFramework.axis2 *= t;

			return Behaviour::FINISH;
		}
		case State::DeterminingOfOxyProjectionOfTop:
		{
			flt3 pointOfIntersection;
			linePlaneIntersection(
				segV0, segV1 - segV0,
				coneFramework.posW +
				coneFramework.axis2 +
				0.5f * coneFramework.axis0 +
				0.5f * coneFramework.axis1,
				axis0, axis1,
				pointOfIntersection
			);

			pointOfIntersection.x() = std::abs(pointOfIntersection.x());
			pointOfIntersection.y() = std::abs(pointOfIntersection.y());

			coneFramework.scaleFromBottomToTop =
				pointOfIntersection.x() / coneFramework.axis0.length();
			if (pointOfIntersection.y() > pointOfIntersection.x())
			{
				coneFramework.scaleFromBottomToTop =
					pointOfIntersection.y() / coneFramework.axis1.length();
			}
			coneFramework.scaleFromBottomToTop *= 2;

			return Behaviour::FINISH;
		}
		case State::Initial:
		{
			if(!bInitialized)
				return Behaviour::FINISH;

			bDrawBase = false;
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

				baseOfFramework.posW =
					barycentricCoords.x() * v0 +
					barycentricCoords.y() * v1 +
					(1 - barycentricCoords.x() - barycentricCoords.y()) * v2;

				flt3 s0 = v1 - v0;
				flt3 s1 = v2 - v0;

				baseOfFramework.axis2 = cross(s1, s0);
				baseOfFramework.axis0 =
					cross(flt3(0.0f, 1.0f, 0.0f), baseOfFramework.axis2);
				if (baseOfFramework.axis0.length() == 0) {
					baseOfFramework.axis0 =
						cross(flt3(1.0f, 0.0f, 0.0f), baseOfFramework.axis2);
				}
				baseOfFramework.axis1 =
					cross(baseOfFramework.axis2, baseOfFramework.axis0);

				baseOfFramework.axis0.normalize();
				baseOfFramework.axis1.normalize();
				baseOfFramework.axis2.normalize();

				bDrawBase = true;

				return Behaviour::FINISH;
			}
			else if (linePlaneIntersection(
				segV0, segV1 - segV0,
				flt3(0.0f, 0.0f, 0.0f), flt3(1.0f, 0.0f, 0.0f), flt3(0.0f, 0.0f, 1.0f),
				baseOfFramework.posW
			))
			{
				baseOfFramework.axis0 = flt3(1.0f, 0.0f, 0.0f);
				baseOfFramework.axis1 = flt3(0.0f, 0.0f, 1.0f);
				baseOfFramework.axis2 = flt3(0.0f, 1.0f, 0.0f);

				baseOfFramework.posW =
					baseOfFramework.posW.x() * baseOfFramework.axis0 +
					baseOfFramework.posW.y() * baseOfFramework.axis1;

				bDrawBase = true;

				return Behaviour::FINISH;
			}
			return Behaviour::CONTINUE;
		}
		}
		return Behaviour::CONTINUE;
	}
	case WM_LBUTTONUP:
	{
		switch (currentState)
		{
		case State::DeterminingOfOxyProjectionOfBottom:
		{
			currentState = State::DeterminingOfHeight;
			return Behaviour::FINISH;
		}
		case State::DeterminingOfOxyProjectionOfTop:
		{
			currentState = State::Insertion;

			auto posW = coneFramework.posW;
			posW += 0.5f * coneFramework.axis0;
			posW += 0.5f * coneFramework.axis1;
			posW += 0.5f * coneFramework.axis2;

			flt4x4 transform(axis0.x(), axis0.y(), axis0.z(), 0.0f,
				axis2.x(), axis2.y(), axis2.z(), 0.0f,
				axis1.x(), axis1.y(), axis1.z(), 0.0f,
				posW.x(), posW.y(), posW.z(), 1.0f);
			auto meshNode = MainScene::instance()->addTransformNode(transform);

			auto bottomRadius = flt2(
				0.5f * coneFramework.axis0.length(),
				0.5f * coneFramework.axis1.length()
			);
			float height = coneFramework.axis2.length();
			flt2 topRadius = coneFramework.scaleFromBottomToTop * bottomRadius;
			meshes.push_back(createCone(bottomRadius, topRadius, height, 24));

			MainScene::instance()->addMeshNode(&meshes.back(), meshNode);
			MainScene::instance()->updateGpu();

			currentState = State::Initial;
			return Behaviour::FINISH;
		}
		}
		return Behaviour::CONTINUE;
	}
	}
	return Behaviour::CONTINUE;
}

void ConeCreator::draw()
{
	switch (currentState)
	{
	case State::Initial:
		Creator::draw();
		break;
	default:
		GraphicsCore::instance()->draw(coneFrameworkGeo);
	}
}