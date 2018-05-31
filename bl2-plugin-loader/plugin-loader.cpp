#include "stdafx.h"
#include "Logging.h"
#include <Shlwapi.h>
#pragma comment (lib, "Shlwapi.lib")

static HINSTANCE hOriginalBink = NULL;
FARPROC p[77] = { 0 };
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

// --- Load Plugins ---
void loadPlugins(const wchar_t *folder)
{
	WIN32_FIND_DATA fd;
	wchar_t targetfilter[FILENAME_MAX];
	wchar_t currfile[FILENAME_MAX];
	wcscpy_s(targetfilter, exeBaseFolder);
	wcscat_s(targetfilter, folder);
	PathCombine(targetfilter, targetfilter, nullptr);
	if (CreateDirectory(targetfilter, nullptr) || ERROR_ALREADY_EXISTS == GetLastError()) {
		//success
	}
	else {
		Logging::LogF("Failed to create plugins folder");
	}
	wcscat_s(targetfilter, L"\\*.dll");
	PathCombine(targetfilter, targetfilter, nullptr);
	HANDLE dllFile = FindFirstFile(targetfilter, &fd);
	if (dllFile == INVALID_HANDLE_VALUE) {
		return;
	}
	do
	{
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			wcscpy_s(currfile, exeBaseFolder);
			wcscat_s(currfile, folder);
			wcscat_s(currfile, L"\\");
			wcscat_s(currfile, fd.cFileName);
			PathCombine(currfile, currfile, nullptr);

			if (PathMatchSpec(_wcslwr(currfile), L"*.dll"))
			{
				if (LoadLibrary(currfile))
				{
					Logging::LogF("Plugin loaded: %s\n", currfile);
					DLLcount++;
				}
				else {
					Logging::LogF("Plugin error: %s\n", currfile);
					MessageBox(nullptr, currfile, TEXT("Error"), MB_OK);
				}
			}
			else {
				MessageBox(nullptr, TEXT("Failed to load plugin"), currfile, MB_OK);
			}
		}
	} while (FindNextFile(dllFile, &fd));
	FindClose(dllFile);
}

void GetAddresses()
{
	hOriginalBink = LoadLibrary(L"binkw23.dll");
	if (hOriginalBink)
	{
		p[0] = GetProcAddress(hOriginalBink, "BinkLogoAddress");
		p[1] = GetProcAddress(hOriginalBink, "BinkBufferSetDirectDraw");
		p[2] = GetProcAddress(hOriginalBink, "BinkIsSoftwareCursor");
		p[3] = GetProcAddress(hOriginalBink, "BinkCheckCursor");
		p[4] = GetProcAddress(hOriginalBink, "BinkRestoreCursor");
		p[5] = GetProcAddress(hOriginalBink, "BinkBufferSetResolution");
		p[6] = GetProcAddress(hOriginalBink, "BinkBufferCheckWinPos");
		p[7] = GetProcAddress(hOriginalBink, "BinkBufferSetOffset");
		p[8] = GetProcAddress(hOriginalBink, "BinkBufferOpen");
		p[9] = GetProcAddress(hOriginalBink, "BinkBufferClose");
		p[10] = GetProcAddress(hOriginalBink, "BinkBufferLock");
		p[11] = GetProcAddress(hOriginalBink, "BinkBufferUnlock");
		p[12] = GetProcAddress(hOriginalBink, "BinkBufferBlit");
		p[13] = GetProcAddress(hOriginalBink, "BinkBufferSetScale");
		p[14] = GetProcAddress(hOriginalBink, "BinkBufferSetHWND");
		p[15] = GetProcAddress(hOriginalBink, "BinkBufferGetDescription");
		p[16] = GetProcAddress(hOriginalBink, "BinkBufferGetError");
		p[17] = GetProcAddress(hOriginalBink, "BinkBufferClear");
		p[18] = GetProcAddress(hOriginalBink, "BinkDDSurfaceType");
		p[19] = GetProcAddress(hOriginalBink, "BinkDX8SurfaceType");
		p[20] = GetProcAddress(hOriginalBink, "BinkDX9SurfaceType");
		p[21] = GetProcAddress(hOriginalBink, "BinkSetError");
		p[22] = GetProcAddress(hOriginalBink, "BinkGetError");
		p[23] = GetProcAddress(hOriginalBink, "BinkSetSoundSystem");
		p[24] = GetProcAddress(hOriginalBink, "BinkSetFrameRate");
		p[25] = GetProcAddress(hOriginalBink, "BinkSetFileOffset");
		p[26] = GetProcAddress(hOriginalBink, "BinkSetIOSize");
		p[27] = GetProcAddress(hOriginalBink, "BinkSetIO");
		p[28] = GetProcAddress(hOriginalBink, "BinkSetSimulate");
		p[29] = GetProcAddress(hOriginalBink, "BinkSetSoundTrack");
		p[30] = GetProcAddress(hOriginalBink, "BinkGetFrameBuffersInfo");
		p[31] = GetProcAddress(hOriginalBink, "BinkRegisterFrameBuffers");
		p[32] = GetProcAddress(hOriginalBink, "BinkOpen");
		p[33] = GetProcAddress(hOriginalBink, "BinkOpenWithOptions");
		p[34] = GetProcAddress(hOriginalBink, "BinkCopyToBuffer");
		p[35] = GetProcAddress(hOriginalBink, "BinkCopyToBufferRect");
		p[36] = GetProcAddress(hOriginalBink, "BinkDoFramePlane");
		p[37] = GetProcAddress(hOriginalBink, "BinkDoFrame");
		p[38] = GetProcAddress(hOriginalBink, "BinkShouldSkip");
		p[39] = GetProcAddress(hOriginalBink, "BinkNextFrame");
		p[40] = GetProcAddress(hOriginalBink, "BinkGetKeyFrame");
		p[41] = GetProcAddress(hOriginalBink, "BinkGoto");
		p[42] = GetProcAddress(hOriginalBink, "BinkFreeGlobals");
		p[43] = GetProcAddress(hOriginalBink, "BinkClose");
		p[44] = GetProcAddress(hOriginalBink, "BinkGetPlatformInfo");
		p[45] = GetProcAddress(hOriginalBink, "BinkWait");
		p[46] = GetProcAddress(hOriginalBink, "BinkPause");
		p[47] = GetProcAddress(hOriginalBink, "BinkGetSummary");
		p[48] = GetProcAddress(hOriginalBink, "BinkGetRealtime");
		p[49] = GetProcAddress(hOriginalBink, "BinkGetRects");
		p[50] = GetProcAddress(hOriginalBink, "BinkService");
		p[51] = GetProcAddress(hOriginalBink, "BinkSetVolume");
		p[52] = GetProcAddress(hOriginalBink, "BinkSetSpeakerVolumes");
		p[53] = GetProcAddress(hOriginalBink, "BinkSetPan");
		p[54] = GetProcAddress(hOriginalBink, "BinkGetTrackType");
		p[55] = GetProcAddress(hOriginalBink, "BinkGetTrackMaxSize");
		p[56] = GetProcAddress(hOriginalBink, "BinkGetTrackID");
		p[57] = GetProcAddress(hOriginalBink, "BinkOpenTrack");
		p[58] = GetProcAddress(hOriginalBink, "BinkCloseTrack");
		p[59] = GetProcAddress(hOriginalBink, "BinkGetTrackData");
		p[60] = GetProcAddress(hOriginalBink, "BinkSetVideoOnOff");
		p[61] = GetProcAddress(hOriginalBink, "BinkSetSoundOnOff");
		p[62] = GetProcAddress(hOriginalBink, "BinkSetMemory");
		p[63] = GetProcAddress(hOriginalBink, "BinkSetWillLoop");
		p[64] = GetProcAddress(hOriginalBink, "BinkControlBackgroundIO");
		p[65] = GetProcAddress(hOriginalBink, "BinkOpenWaveOut");
		p[66] = GetProcAddress(hOriginalBink, "BinkOpenDirectSound");
		p[67] = GetProcAddress(hOriginalBink, "BinkOpenMiles");
		p[68] = GetProcAddress(hOriginalBink, "RADTimerRead");
		p[69] = GetProcAddress(hOriginalBink, "BinkStartAsyncThread");
		p[70] = GetProcAddress(hOriginalBink, "BinkRequestStopAsyncThread");
		p[71] = GetProcAddress(hOriginalBink, "BinkWaitStopAsyncThread");
		p[72] = GetProcAddress(hOriginalBink, "BinkDoFrameAsync");
		p[73] = GetProcAddress(hOriginalBink, "BinkDoFrameAsyncWait");
		p[74] = GetProcAddress(hOriginalBink, "BinkOpenXAudio2");
		p[75] = GetProcAddress(hOriginalBink, "BinkControlPlatformFeatures");
		p[76] = GetProcAddress(hOriginalBink, "BinkGetPalette");
	}
}

DWORD WINAPI Start(LPVOID lpParam)
{
	Logging::LogF("Borderlands 2 DLL Loader by Rob (Robeth) Chiocchio. Based on ME1 ASI Loader by Erik JS\n");
	GetAddresses();
	if (hOriginalBink)
	{
		Logging::LogF("Addresses loaded from binkw23.dll - OK\n");
	}
	else
	{
		Logging::LogF("Error loading binkw23.dll!\n");
		return 0;
	}
	SetExecutableFolder();
	loadPlugins(L"..\\plugins");
	return 0;
}

extern "C" BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DWORD dwThreadId, dwThrdParam = 1;
		HANDLE hThread;
		hThread = CreateThread(NULL, 0, Start, &dwThrdParam, 0, &dwThreadId);
	}
	if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(hOriginalBink);
	}

	return 1;
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkLogoAddress()
{
	__asm
	{
		jmp p[0 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferSetDirectDraw(int a1, int a2)
{
	__asm
	{
		jmp p[1 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkIsSoftwareCursor(int a1, int a2)
{
	__asm
	{
		jmp p[2 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkCheckCursor(int a1, int a2, int a3, int a4, int a5)
{
	__asm
	{
		jmp p[3 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkRestoreCursor(int a1)
{
	__asm
	{
		jmp p[4 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferSetResolution(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[5 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferCheckWinPos(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[6 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferSetOffset(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[7 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferOpen(int a1, int a2, int a3, int a4)
{
	__asm
	{
		jmp p[8 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferClose(int a1)
{
	__asm
	{
		jmp p[9 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferLock(int a1)
{
	__asm
	{
		jmp p[10 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferUnlock(int a1)
{
	__asm
	{
		jmp p[11 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferBlit(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[12 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferSetScale(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[13 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferSetHWND(int a1, int a2)
{
	__asm
	{
		jmp p[14 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferGetDescription(int a1)
{
	__asm
	{
		jmp p[15 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferGetError()
{
	__asm
	{
		jmp p[16 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkBufferClear(int a1, int a2)
{
	__asm
	{
		jmp p[17 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDDSurfaceType(int a1)
{
	__asm
	{
		jmp p[18 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDX8SurfaceType(int a1)
{
	__asm
	{
		jmp p[19 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDX9SurfaceType(int a1)
{
	__asm
	{
		jmp p[20 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetError(int a1)
{
	__asm
	{
		jmp p[21 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetError()
{
	__asm
	{
		jmp p[22 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundSystem(int a1, int a2)
{
	__asm
	{
		jmp p[23 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetFrameRate(int a1, int a2)
{
	__asm
	{
		jmp p[24 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetFileOffset()
{
	__asm
	{
		jmp p[25 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetIOSize(int a1)
{
	__asm
	{
		jmp p[26 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetIO(int a1)
{
	__asm
	{
		jmp p[27 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSimulate(int a1)
{
	__asm
	{
		jmp p[28 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundTrack(int a1, int a2)
{
	__asm
	{
		jmp p[29 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetFrameBuffersInfo(int a1, int a2)
{
	__asm
	{
		jmp p[30 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkRegisterFrameBuffers(int a1, int a2)
{
	__asm
	{
		jmp p[31 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpen(int a1, int a2)
{
	__asm
	{
		jmp p[32 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenWithOptions()
{
	__asm
	{
		jmp p[33 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkCopyToBuffer(int a1, int a2, int a3, int a4, int a5, int a6, int a7)
{
	__asm
	{
		jmp p[34 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkCopyToBufferRect(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
	__asm
	{
		jmp p[35 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDoFramePlane()
{
	__asm
	{
		jmp p[36 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDoFrame(int a1)
{
	__asm
	{
		jmp p[37 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkShouldSkip(int a1)
{
	__asm
	{
		jmp p[38 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkNextFrame(int a1)
{
	__asm
	{
		jmp p[39 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetKeyFrame(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[40 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGoto(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[41 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkFreeGlobals()
{
	__asm
	{
		jmp p[42 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkClose(int a1)
{
	__asm
	{
		jmp p[43 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetPlatformInfo()
{
	__asm
	{
		jmp p[44 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkWait(int a1)
{
	__asm
	{
		jmp p[45 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkPause(int a1, int a2)
{
	__asm
	{
		jmp p[46 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetSummary(int a1, int a2)
{
	__asm
	{
		jmp p[47 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetRealtime(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[48 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetRects(int a1, int a2)
{
	__asm
	{
		jmp p[49 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkService(int a1)
{
	__asm
	{
		jmp p[50 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetVolume(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[51 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSpeakerVolumes()
{
	__asm
	{
		jmp p[52 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetPan(int a1, int a2, int a3)
{
	__asm
	{
		jmp p[53 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetTrackType(int a1, int a2)
{
	__asm
	{
		jmp p[54 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetTrackMaxSize(int a1, int a2)
{
	__asm
	{
		jmp p[55 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetTrackID(int a1, int a2)
{
	__asm
	{
		jmp p[56 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenTrack(int a1, int a2)
{
	__asm
	{
		jmp p[57 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkCloseTrack(int a1)
{
	__asm
	{
		jmp p[58 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetTrackData(int a1, int a2)
{
	__asm
	{
		jmp p[59 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetVideoOnOff(int a1, int a2)
{
	__asm
	{
		jmp p[60 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetSoundOnOff(int a1, int a2)
{
	__asm
	{
		jmp p[61 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetMemory(int a1, int a2)
{
	__asm
	{
		jmp p[62 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkSetWillLoop()
{
	__asm
	{
		jmp p[63 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkControlBackgroundIO(int a1, int a2)
{
	__asm
	{
		jmp p[64 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenWaveOut(int a1)
{
	__asm
	{
		jmp p[65 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenDirectSound(int a1)
{
	__asm
	{
		jmp p[66 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenMiles(int a1)
{
	__asm
	{
		jmp p[67 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall RADTimerRead()
{
	__asm
	{
		jmp p[68 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkStartAsyncThread()
{
	__asm
	{
		jmp p[69 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkRequestStopAsyncThread()
{
	__asm
	{
		jmp p[70 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkWaitStopAsyncThread()
{
	__asm
	{
		jmp p[71 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDoFrameAsync()
{
	__asm
	{
		jmp p[72 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkDoFrameAsyncWait()
{
	__asm
	{
		jmp p[73 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkOpenXAudio2()
{
	__asm
	{
		jmp p[74 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkControlPlatformFeatures(int a1, int a2)
{
	__asm
	{
		jmp p[75 * 4];
	}
}

extern "C" __declspec(dllexport) __declspec(naked) void __stdcall BinkGetPalette(int a1)
{
	__asm
	{
		jmp p[76 * 4];
	}
}