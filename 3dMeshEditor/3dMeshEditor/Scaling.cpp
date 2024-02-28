#include "Scaling.h"

Modifier::Behaviour Scaling::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
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
			FrameOfReferenceState::instance()->checkHandleIntersection(
				mousePosX, mousePosY);

		switch (grabbedAxis)
		{
		case IntersectedAxis::AXIS_X:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			return Behaviour::FINISH;
		case IntersectedAxis::AXIS_Y:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			return Behaviour::FINISH;
		case IntersectedAxis::AXIS_Z:
			paramOnAxis = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			return Behaviour::FINISH;
		}
	}
	break;

	case WM_LBUTTONUP:
		FrameOfReferenceState::instance()->release();
		if (grabbedAxis != IntersectedAxis::NONE)
		{
			grabbedAxis = IntersectedAxis::NONE;
			return Behaviour::FINISH;
		}
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

		float param = 0;
		switch (grabbedAxis)
		{
		case IntersectedAxis::AXIS_X:
			param = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->scaleAlongAxisX(param / paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Y:
			param = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->scaleAlongAxisY(param / paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Z:
			param = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->scaleAlongAxisZ(param / paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			break;
		}
		return Behaviour::FINISH;
	}
	return Behaviour::CONTINUE;
}

void Scaling::draw()
{
	FrameOfReferenceState::instance()->drawScale();;
}