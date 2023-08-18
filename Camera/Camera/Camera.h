#pragma once

#include "Matrix4x4.h"
#include "Vec3d.h"
#include <Windows.h>
#include <map>
#include <string>
#include <vector>


class Camera
{
	flt3 position = flt3(0, 0, -2);
	flt4 position4 = flt4(0, 0, -2, 1);
	
	flt3 fwd = flt3(0, 0, 1);
	flt4 fwd4 = flt4(0, 0, 1, 1);

	float nearZ = 0.1;
	float farZ = 100;

	float aspectRatio = 4.0f / 3.0f;
	float fovY = 90;

	flt4x4 view;
	flt4x4 proj;
	flt4x4 view_x_proj;

	bool changed = true;

	float rotateVelocity = 10;
	float walkVelocity = 1 / rotateVelocity;
	float runVelocity = 5 / rotateVelocity;

	float rotateCoeff = 1;

public:
	const flt3& getEyePos();
	const flt4& getEyePos4();
	const flt3& getFwd();
	const flt4& getFwd4();
	const flt4x4& getView();
	const flt4x4& getProj();
	const flt4x4& getViewProj();
	float getNear();
	float getFar();
	float getFov();
	float getAspectRatio();

	void setAspectRatio(float aspectRatio);

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
	void walkBackward(float dt);
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
		{"walkBackward", &Camera::walkBackward},
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
