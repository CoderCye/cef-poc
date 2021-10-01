#ifndef HOOKS_H
#define HOOKS_H

#include <Windows.h>
#include <iostream>
#include <string_view>
#include "common.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "libs\imgui\imgui.h"
#include "libs\imgui\imgui_impl_dx9.h"
#include "libs\imgui\imgui_impl_win32.h"

#define WIN_WIDTH 1440.0f
#define WIN_HEIGHT 990.0f


// EndScene Hook
using fpPresent_t = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9 , CONST RECT*, CONST RECT*, HWND, CONST RGNDATA* );

HRESULT __stdcall Present_Hook(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion);

// Reset Hook
using fpReset_t = HRESULT(__stdcall*)(LPDIRECT3DDEVICE9, D3DPRESENT_PARAMETERS*);

HRESULT __stdcall Reset_Hook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);

// WndProc Hook
struct WndProcInfo
{
	WNDPROC WndProcPrev;
};
LRESULT __stdcall WndProc_Hook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


#endif // HOOKS_H