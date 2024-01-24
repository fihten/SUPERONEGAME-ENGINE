#include "Transition.h"

void Transition::processWindowMessage(UINT msg, WPARAM wparam, LPARAM lparam)
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

		float param = 0;
		switch (grabbedAxis)
		{
		case IntersectedAxis::AXIS_X:
			param = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->moveAlongAxisX(param - paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnXaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Y:
			param = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->moveAlongAxisY(param - paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnYaxis(mousePosX, mousePosY);
			break;
		case IntersectedAxis::AXIS_Z:
			param = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			FrameOfReferenceState::instance()->moveAlongAxisZ(param - paramOnAxis);
			paramOnAxis = FrameOfReferenceState::instance()->projectOnZaxis(mousePosX, mousePosY);
			break;
		}

		break;
	}
}