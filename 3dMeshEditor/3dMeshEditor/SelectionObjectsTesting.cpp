#include "SelectionObjectsTesting.h"
#include "MainScene.h"

void fillSceneForObjectsSelectionTesting()
{
	static Mesh cube = createCube();
	static Mesh sphere = createSphere(10, 10);
	static Mesh cone = createCone(1, 2, 2, 10);
//	static Mesh plane = createPlane(2, 2, 100, 100);

	Mesh* meshes[] = { &cube,&sphere,&cone };

	int L = 10;
	int M = 10;
	int N = 10;

	float dX = 5;
	float dY = 5;
	float dZ = 5;

	float x0 = -L * dX / 2;
	float y0 = -M * dY / 2;
	float z0 = -N * dZ / 2;

	std::srand(543);
	for (int i = 0; i < L; i++)
	{
		for (int j = 0; j < M; j++)
		{
			for (int k = 0; k < N; k++)
			{
				flt4x4 pos;
				pos.m30() = x0 + i * dX;
				pos.m31() = y0 + j * dY;
				pos.m32() = z0 + k * dZ;

				float angleX = (float)std::rand() / (float)RAND_MAX * 360;
				float angleY = (float)std::rand() / (float)RAND_MAX * 360;
				float angleZ = (float)std::rand() / (float)RAND_MAX * 360;

				flt4x4 rotX = makeRotate(flt3(1, 0, 0), angleX);
				flt4x4 rotY = makeRotate(flt3(0, 1, 0), angleY);
				flt4x4 rotZ = makeRotate(flt3(0, 0, 1), angleZ);

				flt4x4 rot = rotX * rotY * rotZ;

				float scaleX = 0.5 * (float)std::rand() / (float)RAND_MAX * dX / sqrt(3);
				float scaleY = 0.5 * (float)std::rand() / (float)RAND_MAX * dY / sqrt(3);
				float scaleZ = 0.5 * (float)std::rand() / (float)RAND_MAX * dZ / sqrt(3);

				flt4x4 scale(
					scaleX, 0, 0, 0,
					0, scaleY, 0, 0,
					0, 0, scaleZ, 0,
					0, 0, 0, 1
				);

				pos = scale * rot * pos;

				NodeID transformID = MainScene::instance()->addTransformNode(pos);

				int meshIndex = std::rand() % (sizeof(meshes) / sizeof(Mesh*));

				MainScene::instance()->addMeshNode(meshes[meshIndex], transformID);
			}
		}
	}
	MainScene::instance()->updateGpu();
}