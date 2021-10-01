#include "common.h"

extern DWORD g_dwSAMP;
extern stInputInfo* g_stInput;

MH_STATUS MH_CreateAndEnableHook(LPVOID pTarget, LPVOID pDetour, LPVOID* ppOriginal)
{
	MH_CreateHook(pTarget, pDetour, ppOriginal);
	return MH_EnableHook(pTarget);
}

void ToggleCursor(int mode)
{
	if (g_stInput->iInputEnabled) return;

	void* obj = *(void**)(g_dwSAMP + 0x21A10C); // s0b
	((void(__thiscall*) (void*, int, bool)) (g_dwSAMP + 0x09BD30))(obj, mode ? 3 : 0, !mode);
	if (!mode)
		((void(__thiscall*) (void*)) (g_dwSAMP + 0x09BC10))(obj);
}
