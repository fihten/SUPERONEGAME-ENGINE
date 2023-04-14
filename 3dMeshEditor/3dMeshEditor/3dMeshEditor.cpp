#include "GraphicsCore.h"
#include "Cameras.h"
#include "Timer.h"
#include "Mesh.h"
#include "Scene.h"
#include "DrawVisitor.h"
#include <Windows.h>

GraphicsCore graphicsCore;
Mesh cube;
Scene scene;
DrawVisitor drawVisitor;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lparam);
		UINT height = HIWORD(lparam);

		graphicsCore.resize(width, height);

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}

	float dt = Timer::Instance()->elapsedTime();
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
		cameras()[i].processMessage(msg,wparam,lparam, dt);

	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void drawFunc(GraphicsCore* graphicsCore)
{
	graphicsCore->startFrame();
	scene.accept(&drawVisitor);
	graphicsCore->endFrame();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	cube = createCube();

	NodeID mesh = scene.addMeshNode(&cube);
	
	scene.setNodeParam(mesh, "gMaterial.Ambient", "{0.137f, 0.42f, 0.556f, 1.0f}");
	scene.setNodeParam(mesh, "gMaterial.Diffuse", "{0.137f, 0.42f, 0.556f, 1.0f}");
	scene.setNodeParam(mesh, "gMaterial.Specular", "{0.8f, 0.8f, 0.8f, 96.0f}");
	scene.setNodeParam(mesh, "gMaterial.Reflect", "{0,0,0,0}");

	NodeID dirLight = scene.addTransformNode(flt4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, -1, 1
	));
	
	scene.setNodeParam(dirLight, "gDirLight.Ambient", "{0.2f, 0.2f, 0.2f, 1.0f}");
	scene.setNodeParam(dirLight, "gDirLight.Diffuse", "{0.5f, 0.5f, 0.5f, 1.0f}");
	scene.setNodeParam(dirLight, "gDirLight.Specular", "{0.5f, 0.5f, 0.5f, 1.0f}");
	scene.setNodeParam(dirLight, "gDirLight.Direction", "node_axis_z");
	
	scene.createNodeParamReference(mesh, dirLight, "gDirLight.Ambient");
	scene.createNodeParamReference(mesh, dirLight, "gDirLight.Diffuse");
	scene.createNodeParamReference(mesh, dirLight, "gDirLight.Specular");
	scene.createNodeParamReference(mesh, dirLight, "gDirLight.Direction");

	NodeID pointLight = scene.addTransformNode(flt4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		-1, 0, -1, 1
	));

	scene.setNodeParam(pointLight, "gPointLight.Ambient", "{0.3f, 0.3f, 0.3f, 1.0f}");
	scene.setNodeParam(pointLight, "gPointLight.Diffuse", "{0.7f, 0.7f, 0.7f, 1.0f}");
	scene.setNodeParam(pointLight, "gPointLight.Specular", "{0.7f, 0.7f, 0.7f, 1.0f}");
	scene.setNodeParam(pointLight, "gPointLight.Position", "node_position");
	scene.setNodeParam(pointLight, "gPointLight.Range", "100");
	scene.setNodeParam(pointLight, "gPointLight.Att", "{0.0f, 0.1f, 0.0f}");

	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Ambient");
	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Diffuse");
	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Specular");
	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Position");
	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Range");
	scene.createNodeParamReference(mesh, pointLight, "gPointLight.Att");

	NodeID spotLight = scene.addTransformNode(flt4x4(
		0, 0, 1, 0,
		0, 1, 0, 0,
		1, 0, 0, 0,
		-1, 0, 0, 1
	));

	scene.setNodeParam(spotLight, "gSpotLight.Ambient", "{0.0f, 0.0f, 0.0f, 1.0f}");
	scene.setNodeParam(spotLight, "gSpotLight.Diffuse", "{1.0f, 1.0f, 0.0f, 1.0f}");
	scene.setNodeParam(spotLight, "gSpotLight.Specular", "{1.0f, 1.0f, 1.0f, 1.0f}");
	scene.setNodeParam(spotLight, "gSpotLight.Position", "node_position");
	scene.setNodeParam(spotLight, "gSpotLight.Range", "100");
	scene.setNodeParam(spotLight, "gSpotLight.Direction", "node_axis_z");
	scene.setNodeParam(spotLight, "gSpotLight.Spot", "10");
	scene.setNodeParam(spotLight, "gSpotLight.Att", "{1.0f, 0.0f, 0.0f}");

	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Ambient");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Diffuse");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Specular");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Position");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Range");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Direction");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Spot");
	scene.createNodeParamReference(mesh, spotLight, "gSpotLight.Att");

	drawVisitor.graphicsCore = &graphicsCore;

	graphicsCore.init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	return graphicsCore.run();
}