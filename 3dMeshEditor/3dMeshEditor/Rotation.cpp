#include "Rotation.h"

void Rotation::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_LBUTTONDOWN:
	{
		mousePosX = LOWORD(lparam) + 0.5f;
		mousePosY = HIWORD(lparam) + 0.5f;

		RECT rect;
		GetClientRect(hWnd, &rect);

		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		mousePosX /= width;
		mousePosY /= height;

		mousePosX = 2 * (mousePosX - 0.5);
		mousePosY = 2 * (0.5 - mousePosY);

		grabbedAxis =
			FrameOfReferenceState::instance()->checkIntersection(
				mousePosX, mousePosY);

		switch (grabbedAxis)
		{
		case IntersectedAxis::AXIS_X:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Y:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Z:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			break;
		}
	}
	break;

	case WM_LBUTTONUP:
		grabbedAxis = IntersectedAxis::NONE;
		FrameOfReferenceState::instance()->release();
		break;

	case WM_MOUSEMOVE:
		if (grabbedAxis == IntersectedAxis::NONE)
			break;

		mousePosX = LOWORD(lparam) + 0.5f;
		mousePosY = HIWORD(lparam) + 0.5f;

		RECT rect;
		GetClientRect(hWnd, &rect);

		float width = rect.right - rect.left;
		float height = rect.bottom - rect.top;

		mousePosX /= width;
		mousePosY /= height;

		mousePosX = 2 * (mousePosX - 0.5);
		mousePosY = 2 * (0.5 - mousePosY);

		switch (grabbedAxis)
		{
		case IntersectedAxis::AXIS_X:
			flt2 p_xy = FrameOfReferenceState::instance()->intersectWithOxy(mousePosX, mousePosY);
			float angleZ = atan2(p_xy.y(), p_xy.x());
			angleZ *= 180.0f / M_PI;

			p_xy.x() -= paramOnAxis;
			if (p_xy.x() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisZ(angleZ);
				break;
			}
			float tanXY = p_xy.y() / p_xy.x();

			flt2 p_xz = FrameOfReferenceState::instance()->intersectWithOxz(mousePosX, mousePosY);
			float angleY = atan2(p_xz.x(), p_xz.y());
			angleY *= 180.0f / M_PI;

			p_xz.x() -= paramOnAxis;
			if (p_xz.x() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisY(angleY);
				break;
			}
			float tanXZ = p_xz.y() / p_xz.x();
			
			if (tanXY > tanXZ)
				FrameOfReferenceState::instance()->rotateAlongAxisZ(angleZ);
			else
				FrameOfReferenceState::instance()->rotateAlongAxisY(angleY);

			break;
		case IntersectedAxis::AXIS_Y:
			flt2 p_xy = FrameOfReferenceState::instance()->intersectWithOxy(mousePosX, mousePosY);
			float angleZ = atan2(p_xy.y(), p_xy.x());
			angleZ *= 180.0f / M_PI;

			p_xy.y() -= paramOnAxis;
			if (p_xy.y() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisZ(angleZ);
				break;
			}
			float tanXY = p_xy.x() / p_xy.y();

			flt2 p_yz = FrameOfReferenceState::instance()->intersectWithOyz(mousePosX, mousePosY);
			float angleX = atan2(p_yz.y(), p_yz.x());
			angleX *= 180.0f / M_PI;

			p_yz.x() -= paramOnAxis;
			if (p_yz.x() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisX(angleX);
				break;
			}
			float tanYZ = p_yz.y() / p_yz.x();

			if (tanXY > tanYZ)
				FrameOfReferenceState::instance()->rotateAlongAxisZ(angleZ);
			else
				FrameOfReferenceState::instance()->rotateAlongAxisX(angleX);

			break;
		case IntersectedAxis::AXIS_Z:
			flt2 p_xz = FrameOfReferenceState::instance()->intersectWithOxz(mousePosX, mousePosY);
			float angleY = atan2(p_xz.x(), p_xz.y());
			angleY *= 180.0f / M_PI;

			p_xz.y() -= paramOnAxis;
			if (p_xz.y() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisY(angleY);
				break;
			}
			float tanXZ = p_xz.x() / p_xz.y();

			flt2 p_yz = FrameOfReferenceState::instance()->intersectWithOyz(mousePosX, mousePosY);
			float angleX = atan2(p_yz.y(), p_yz.x());
			angleX *= 180.0f / M_PI;

			p_yz.y() -= paramOnAxis;
			if (p_yz.y() == 0)
			{
				FrameOfReferenceState::instance()->rotateAlongAxisX(angleX);
				break;
			}
			float tanYZ = p_yz.x() / p_yz.y();

			if (tanXZ > tanYZ)
				FrameOfReferenceState::instance()->rotateAlongAxisY(angleY);
			else
				FrameOfReferenceState::instance()->rotateAlongAxisX(angleX);

			break;
		}

		break;
	}
}