#include "GraphicsCore.h"
#include "Cameras.h"
#include "Timer.h"
#include "Mesh.h"
#include "Scene.h"
#include "DrawVisitor.h"
#include <Windows.h>

GraphicsCore graphicsCore;
Mesh cube;
Mesh trees;
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
	trees = createTrees();

	NodeID mesh = scene.addMeshNode(&trees);

	scene.setNodeParam(mesh, "gMaterial.Ambient", "{0.137f, 0.42f, 0.556f, 1.0f}");
	scene.setNodeParam(mesh, "gMaterial.Diffuse", "{0.137f, 0.42f, 0.556f, 1.0f}");
	scene.setNodeParam(mesh, "gMaterial.Specular", "{0.8f, 0.8f, 0.8f, 96.0f}");
	scene.setNodeParam(mesh, "gMaterial.Reflect", "{0,0,0,0}");

	NodeID dirLight0 = scene.addTransformNode(flt4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	));
	
	scene.setNodeParam(dirLight0, "gDirLights[0].Ambient", "{0.2f, 0.2f, 0.2f, 1.0f}");
	scene.setNodeParam(dirLight0, "gDirLights[0].Diffuse", "{0.5f, 0.5f, 0.5f, 1.0f}");
	scene.setNodeParam(dirLight0, "gDirLights[0].Specular", "{0.5f, 0.5f, 0.5f, 1.0f}");
	scene.setNodeParam(dirLight0, "gDirLights[0].Direction", "node_axis_z");
	
	scene.createNodeParamReference(mesh, dirLight0, "gDirLights[0].Ambient");
	scene.createNodeParamReference(mesh, dirLight0, "gDirLights[0].Diffuse");
	scene.createNodeParamReference(mesh, dirLight0, "gDirLights[0].Specular");
	scene.createNodeParamReference(mesh, dirLight0, "gDirLights[0].Direction");

	NodeID dirLight1 = scene.addTransformNode(flt4x4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1
	));

	scene.setNodeParam(dirLight1, "gDirLights[1].Ambient", "{0.2f, 0.1f, 0.1f, 1.0f}");
	scene.setNodeParam(dirLight1, "gDirLights[1].Diffuse", "{0.5f, 0.25f, 0.25f, 1.0f}");
	scene.setNodeParam(dirLight1, "gDirLights[1].Specular", "{0.5f, 0.25f, 0.25f, 1.0f}");
	scene.setNodeParam(dirLight1, "gDirLights[1].Direction", "node_axis_z");

	scene.createNodeParamReference(mesh, dirLight1, "gDirLights[1].Ambient");
	scene.createNodeParamReference(mesh, dirLight1, "gDirLights[1].Diffuse");
	scene.createNodeParamReference(mesh, dirLight1, "gDirLights[1].Specular");
	scene.createNodeParamReference(mesh, dirLight1, "gDirLights[1].Direction");

	NodeID dirLight2 = scene.addTransformNode(flt4x4(
		1, 0, 0, 0,
		0, 0, 1, 0,
		0, -1, 0, 0,
		0, 0, 0, 1
	));

	scene.setNodeParam(dirLight2, "gDirLights[2].Ambient", "{0.1f, 0.2f, 0.1f, 1.0f}");
	scene.setNodeParam(dirLight2, "gDirLights[2].Diffuse", "{0.25f, 0.5f, 0.25f, 1.0f}");
	scene.setNodeParam(dirLight2, "gDirLights[2].Specular", "{0.25f, 0.5f, 0.25f, 1.0f}");
	scene.setNodeParam(dirLight2, "gDirLights[2].Direction", "node_axis_z");

	scene.createNodeParamReference(mesh, dirLight2, "gDirLights[2].Ambient");
	scene.createNodeParamReference(mesh, dirLight2, "gDirLights[2].Diffuse");
	scene.createNodeParamReference(mesh, dirLight2, "gDirLights[2].Specular");
	scene.createNodeParamReference(mesh, dirLight2, "gDirLights[2].Direction");

	drawVisitor.graphicsCore = &graphicsCore;

	graphicsCore.init(hInstance, iCmdShow, WndProc, drawFunc, 640, 480, true, false);

	return graphicsCore.run();
}