#include "stdafx.h"
#include "detours.h"
#include "BL2-SDK.h"
#include "Util.h"
#include "Settings.h"
#include "Logging.h"

wchar_t exeBaseFolder[FILENAME_MAX];
int DLLcount = 0;

// Sets exeBaseFolder to hold current executable's path, including "\"
void SetExecutableFolder()
{
	GetModuleFileName(NULL, exeBaseFolder, FILENAME_MAX);
	int x = sizeof(exeBaseFolder) - 1;
	while (exeBaseFolder[x] != '\\')
		x--;
	exeBaseFolder[x + 1] = '\0';
}

DWORD WINAPI Start(LPVOID lpParam)
{
	if (Util::WaitForModules(10000, { L"d3d9.dll", L"PhysXLoader.dll" }) == WAIT_TIMEOUT) {
		return 0; // One or more modules were not loaded in time
	}

	SetExecutableFolder();
	Settings::Initialize(exeBaseFolder);
	Logging::InitializeFile(Settings::GetLogFilePath());

	Logging::LogF("======= Borderlands Python Loader =======\n");
	BL2SDK::initialize(exeBaseFolder);

	return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hInst);

		DetourRestoreAfterWith();
		DetourUpdateThread(GetCurrentThread());

		DWORD dwThreadId, dwThrdParam = 1;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, Start, &dwThrdParam, 0, &dwThreadId);
	}
	if (reason == DLL_PROCESS_DETACH)
	{
		BL2SDK::cleanup();
	}

	return 1;
}