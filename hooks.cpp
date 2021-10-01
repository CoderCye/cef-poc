/*
	Troubleshooting with u8 string literal prefix on C++20 => i switch to C++17
*/

#include "hooks.hpp"
#include <d3d9.h>
#include <d3dx9.h>
#include "render_funcs.h"
//#include <audio/bass.h>
//#include <audio/bass_fx.h>

// по идее можно убрать, но нужно проверить не наебнется ли поддержка русской кодировки utf-8
#pragma execution_character_set("utf-8")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static bool Menu = false;

extern fpPresent_t origPresent;
extern fpReset_t origReset;
extern WndProcInfo stWndProcInfo;

IDirect3DTexture9* pTexture = nullptr;

HRESULT __stdcall Present_Hook(LPDIRECT3DDEVICE9 pDevice, CONST RECT* pSrcRect, CONST RECT* pDestRect, HWND hDestWindow, CONST RGNDATA* pDirtyRegion)
{

	// do stuff here ...
	static bool ImInit = false;
	
	if (!ImInit)
	{
		std::cout << "enter to present hook" << std::endl;
		ImGui::CreateContext();
		ImGui_ImplWin32_Init(*reinterpret_cast<HWND*>(0xC97C1CU));
		// фикс русских символов в CP1251
#pragma warning(push)
#pragma warning(disable: 4996)
		std::string font{ getenv("windir") };
#pragma warning(pop)
		font += "\\Fonts\\Arial.TTF";
		std::cout << font.c_str() << std::endl;
		ImGui::GetIO().Fonts->AddFontFromFileTTF(font.c_str(), 14.0f, NULL, ImGui::GetIO().Fonts->GetGlyphRangesCyrillic());
		ImGui_ImplDX9_Init(pDevice);

		ImGui::StyleColorsLight();
		
		ImInit = true;
		ImGui::SetNextWindowPos({ 550.f, 350.f });

		D3DXCreateTextureFromFile(pDevice, LR"(D:\GTA San Andreas MultiPlayer\fucker.png)", &pTexture);
		
	}
	
	if (Menu)
	{
		static bool bass = false;
		if (!bass)
		{
			BASS_Init(-1, 44100, NULL, 0, 0);
			bass = true;
		}
		//ImGui::CreateContext();
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		

		ImGui::SetNextWindowSize({ 346.f,185.f });
		//ImGui::SetNextWindowPos({ 550.f, 350.f });

		static char szInputID[256];
		static bool bEveryTwoMinutes;
		static float RGBA[4];

		// u8 для использования кириллики (русских символов)
		// [1] First window for menu
		ImGui::Begin(u8"CopChase", &Menu, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoResize );


		ImGui::SetCursorPos({ 25.f,86.f });
		ImGui::Text("Settings:");
		ImGui::SetCursorPos({ 25.f,36.f });
		ImGui::Text("ID of the suspect:");
		ImGui::SetCursorPos({ 25.f,55.f });
		//ImGui::InputText("Select", szInputID, 144);
		//ImGui::ColorEdit4("Color", RGBA);
		ImGui::InputText("url", szInputID, IM_ARRAYSIZE(szInputID));
		ImGui::SetCursorPos({ 130.f,141.f });
		
		if (ImGui::Button("Apply", { 77.f,23.f }))
		{
			/*
			std::cout << szInputID << std::endl;
			
#pragma warning(disable: 4996)
			sscanf(szInputID, "%f %f %f", &r, &g, &b);
#pragma warning(default: 4996)
			*/
			//change_color_value({RGBA[0], RGBA[1], RGBA[2], RGBA[3]});
			//world_add_light_hook();

			if(music_worker(szInputID))
				std::cout << "music_worker() success used" << std::endl;
		}
		ImGui::SetCursorPos({ 25.f,105.f });
		ImGui::Checkbox("Every 2 minutes", &bEveryTwoMinutes);

		//ImGui::Image(pTexture, ImVec2(200, 200));

		ImGui::ProgressBar(35.0, ImVec2(150, 10));

		ImGui::End();

		// [2] Second window for imgui render primitives
		ImGui::SetNextWindowPos({ .0f, .0f });
		ImGui::SetNextWindowSize({ WIN_WIDTH, WIN_HEIGHT });

		ImGui::Begin("renderer", nullptr, ImGuiWindowFlags_::ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_::ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_::ImGuiWindowFlags_NoMove | ImGuiWindowFlags_::ImGuiWindowFlags_NoResize | ImGuiWindowFlags_::ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_::ImGuiWindowFlags_NoInputs);

		//ImGui::GetWindowDrawList()->AddImage(pTexture, { 0,0 }, { 800, 800 });
		ImGui::GetWindowDrawList()->AddText({100, 20}, ImColor(1.0f,1.0f,1.0f, 1.0f), u8"ДОРА ЛУЧШАЯ!");
	
		//ImGui::GetWindowDrawList()->AddBezierCubic(ImVec2(200.f, 20.f), ImVec2(200.f, 50.f), ImVec2(250.f, 50.f), ImVec2(250.f, 20.f), ImColor(0.65f, 0.0f, 1.0f, 1.0f), 1.0);
		//ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(.0f, .0f), ImVec2(WIN_WIDTH, WIN_HEIGHT), ImColor(0.65f, 0.0f, 1.0f, 1.0f));
			
		ImGui::End();

		ImGui::EndFrame();
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	}
	

	return origPresent(pDevice, pSrcRect, pDestRect, hDestWindow, pDirtyRegion);
}

HRESULT __stdcall Reset_Hook(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	// do stuff here ...
	ImGui_ImplDX9_InvalidateDeviceObjects();
	return origReset(pDevice, pPresentationParameters);
}

LRESULT __stdcall WndProc_Hook(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool popen = false;
	if (msg == WM_KEYDOWN)
	{
		if (wParam == 0x49) // I
		{
			bool ret = Menu;
			Menu = !Menu;
			return ret;
		}
		if (wParam == VK_ESCAPE && Menu) // I
		{
			Menu = false;
			return true;
		}
	}

	if (Menu)
	{
		if (!popen)
			ToggleCursor(1);
		popen = true;
		ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
		return true;
	}
	else
	{
		if (popen)
		{
			popen = false;
			ToggleCursor(0);
		}
	}

	return CallWindowProc(stWndProcInfo.WndProcPrev, hWnd, msg, wParam, lParam);
}
