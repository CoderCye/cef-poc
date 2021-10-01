#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <sampapi\CChat.h>
#include "kthook\include\kthook.hpp"
#include <MinHook\MinHook.h>
#include "megahelp.h"
#include "hooks.hpp"
#include <audio/bass.h>
//#include <d3d.h>
//#include <d3dx9.h>
//#include "libs\imgui\imgui.h"
//#include "libs\imgui\imgui_impl_win32.h"
//#include "libs\imgui\imgui_impl_dx9.h"

#pragma execution_character_set("utf-8")

using CHud__DrawRadar_t = void(__cdecl*)();
kthook::kthook_simple_t<CHud__DrawRadar_t> samp_init{ reinterpret_cast<void*>(0x58A330) };

fpPresent_t origPresent = nullptr;
fpReset_t origReset = nullptr;
DWORD g_dwSAMP;
stInputInfo* g_stInput;
WndProcInfo stWndProcInfo;

void AfterSampInit_HOOK()
{
	static bool inited = false;
	namespace samp = sampapi::v037r1;
	if (!inited && samp::RefChat != nullptr)
	{
		g_dwSAMP = reinterpret_cast<DWORD>(GetModuleHandleW(L"samp.dll"));       
		g_stInput = *(stInputInfo**)(g_dwSAMP + 0x21A0E8);

		stWndProcInfo.WndProcPrev = reinterpret_cast<WNDPROC>(SetWindowLongPtr(*reinterpret_cast<HWND*>(0xC97C1CU), GWL_WNDPROC, reinterpret_cast<LONG>(&WndProc_Hook)));
		void** VTableD3D9Interface = *reinterpret_cast<void***>(*reinterpret_cast<DWORD*>(0xC97C28));
		MH_CreateAndEnableHook(reinterpret_cast<LPVOID>(VTableD3D9Interface[16]), &Reset_Hook, reinterpret_cast<void**>(&origReset));
		MH_CreateAndEnableHook(reinterpret_cast<LPVOID>(VTableD3D9Interface[17]), &Present_Hook, reinterpret_cast<void**>(&origPresent));
		inited ^= true;
	}
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD fdwReason, LPVOID)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	{
		MH_Initialize();
		MegaHelp::DebugConsole();
		DisableThreadLibraryCalls(hModule);
		samp_init.after.connect(AfterSampInit_HOOK);
		break;
	}
	case DLL_PROCESS_DETACH:
	{
		MH_Uninitialize();
		BASS_Free();
		break;
	}
	}

	return TRUE;
}