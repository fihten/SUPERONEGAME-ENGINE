#include "Cameras.h"
#include <tchar.h>
#include <string>

Camera cameras[CAMERAS_NUMBER];

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

		cameras[i].loadCFG(szCameraCFG);
	}
}