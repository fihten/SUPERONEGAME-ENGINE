#pragma once

#include "Matrix4x4.h"
#include "Vec3d.h"
#include <Windows.h>
#include <map>
#include <string>
#include <vector>


class Camera
{
	flt3 position = flt3(0, 0, 0);
	flt3 fwd = flt3(0, 0, 1);

	float nearZ = 0.1;
	float farZ = 100;

	float aspectRatio = 4.0f / 3.0f;
	float fovY = 90;

	flt4x4 view;
	flt4x4 proj;

	bool changed = true;

	float rotateVelocity = 1;
	float walkVelocity = 1 / rotateVelocity;
	float runVelocity = 5 / rotateVelocity;

	float rotateCoeff = 1;

public:
	const flt4x4& getView();
	const flt4x4& getProj();

	enum Behaviour
	{
		CONTINUE,
		EXIT
	};
	Behaviour processMessage(UINT msg, WPARAM wparam, LPARAM lparam, float dt);
	bool loadCFG(const std::string& cfg);

private:
	void updateView();
	void updateProj();

	void walk(float dt);
	void run(float dt);

	void lookUp(float d);
	void lookDown(float d);

	void lookLeft(float d);
	void lookRight(float d);

	void rightSidestep(float dt);
	void leftSidestep(float dt);

	void rightSidefaststep(float dt);
	void leftSidefaststep(float dt);

	typedef void(Camera::* Operation)(float);
	std::map<std::string, Camera::Operation> operations =
	{
		{"walk", &Camera::walk},
		{"run", &Camera::run},
		{"lookUp", &Camera::lookUp},
		{"lookDown", &Camera::lookDown},
		{"lookLeft", &Camera::lookLeft},
		{"lookRight", &Camera::lookRight},
		{"rightSidestep", &Camera::rightSidestep},
		{"leftSidestep", &Camera::leftSidestep},
		{"rightSidefaststep", &Camera::rightSidefaststep},
		{"leftSidefaststep", &Camera::leftSidefaststep}
	};

	struct LinkOfKeyAndOperation
	{
		UINT key = 0;
		std::string operation = "";
	};
	std::vector<LinkOfKeyAndOperation> LinksOfKeysAndOperations;

	Operation getOperation(UINT vkKey) const;
};
