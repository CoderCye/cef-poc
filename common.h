#ifndef COMMON_H
#define COMMON_H
#include <Windows.h>
#include <MinHook/MinHook.h>

struct stInputInfo
{
	void* pD3DDevice;
	void* pDXUTDialog;
	void* pDXUTEditBox;
	void* pCMDs[144];
	char szCMDNames[144][33];
	int iCMDCount;
	int iInputEnabled;
	char szInputBuffer[129];
	char szRecallBufffer[10][129];
	char szCurrentBuffer[129];
	int iCurrentRecall;
	int iTotalRecalls;
	void* pszDefaultCMD;
};

MH_STATUS MH_CreateAndEnableHook(LPVOID pTarget, LPVOID pDetour, LPVOID *ppOriginal);
void ToggleCursor(int mode);

#endif // COMMON_H