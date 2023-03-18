#include "pch.h"
#include "Cameras.h"
#include <tchar.h>
#include <string>

Camera* cameras()
{
	static Camera c[CAMERAS_NUMBER];
	return c;
}
void loadCamerasCFGs()
{
	char cfgsFolder[200];
	int sz = sizeof cfgsFolder / sizeof * cfgsFolder;
	GetEnvironmentVariableA("CONFIGS", cfgsFolder, sz);

	std::string sCameraCFGtemplate = std::string(cfgsFolder) + "\\Camera%d.cfg";
	for (int i = 0; i < CAMERAS_NUMBER; ++i)
	{
		char szCameraCFG[200];
		sprintf(szCameraCFG, sCameraCFGtemplate.c_str(), i);

		cameras()[i].loadCFG(szCameraCFG);
	}
}