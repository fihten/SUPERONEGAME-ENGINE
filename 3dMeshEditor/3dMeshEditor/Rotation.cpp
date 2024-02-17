#include "Rotation.h"

Modifier::Behaviour Rotation::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
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
			FrameOfReferenceState::instance()->checkCircleIntersection(
				mousePosX, mousePosY);

		switch (grabbedAxis)
		{
		case IntersectedCircleAxis::PLAIN_XY:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXYcircle(mousePosX, mousePosY);
			return Behaviour::FINISH;
		case IntersectedCircleAxis::PLAIN_XZ:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXZcircle(mousePosX, mousePosY);
			return Behaviour::FINISH;
		case IntersectedCircleAxis::PLAIN_YZ:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYZcircle(mousePosX, mousePosY);
			return Behaviour::FINISH;
		}
	}
	break;

	case WM_LBUTTONUP:
		FrameOfReferenceState::instance()->release();
		if (grabbedAxis != IntersectedCircleAxis::NONE)
		{
			grabbedAxis = IntersectedCircleAxis::NONE;
			return Behaviour::FINISH;
		}
		break;

	case WM_MOUSEMOVE:
		if (grabbedAxis == IntersectedCircleAxis::NONE)
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

		float param = 0;
		switch (grabbedAxis)
		{
		case IntersectedCircleAxis::PLAIN_XY:
			param = FrameOfReferenceState::instance()->projectOnXYcircle(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->rotateAlongAxisZ(param - paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXYcircle(mousePosX, mousePosY);
			break;
		case IntersectedCircleAxis::PLAIN_YZ:
			param = FrameOfReferenceState::instance()->projectOnYZcircle(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->rotateAlongAxisX(param - paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYZcircle(mousePosX, mousePosY);
			break;
		case IntersectedCircleAxis::PLAIN_XZ:
			param = FrameOfReferenceState::instance()->projectOnXZcircle(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->rotateAlongAxisY(paramOnAxis - param);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXZcircle(mousePosX, mousePosY);
			break;
		}
		return Behaviour::FINISH;
	}
	return Behaviour::CONTINUE;
}

void Rotation::draw()
{
	FrameOfReferenceState::instance()->drawSpheric();
}