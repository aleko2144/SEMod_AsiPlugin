#include "CObjectInspector.h"
#include "../../dllmain.h"
#include "../../include/Core/SEMod_Core.h"
#include <list>
#include <atlbase.h>
#define BUTTON_UPDATE	(WM_APP+1001)
#define LIST_BOX		(WM_APP+1002)
#define FIND_EDITBOX	(WM_APP+1003)
#define BUTTON_FIND		(WM_APP+1004)

HWND CObjectInspector::_hwnd;


DWORD WINAPI CObjectInspector::ThreadProc(LPVOID lpParam)
{
	heapDef = (HeapNameDef*)0x0071E840;

	RegisterDLLWindowClass(L"Object Inspector");
	return 0;
}

CObjectInspector::HeapNameDef* CObjectInspector::heapDef;

void CObjectInspector::UpdateListBox()
{
	std::list<char*> names = {};
	for (int i = 0; i < 500; i++)
	{
		HeapAllocatorFile* v4 = (HeapAllocatorFile*)heapDef->m_pData[i];
		if (v4)
		{
			while (v4)
			{
				//if((char*)v4->m_sName[0] != '\0')
				names.push_back((char*)v4->m_sName);
				v4 = (HeapAllocatorFile*)v4->m_pNext;
			}
		}
		else

		{
			break;
		}
	}

	HWND hwndList = GetDlgItem(_hwnd, LIST_BOX);

	for (auto iter = names.begin(); iter != names.end(); iter++)
	{

		CA2T name(*iter);
		int pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,
			(LPARAM)name.m_psz);
		SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)pos);

	}
	SetFocus(hwndList);
}
void CObjectInspector::LoadParentTree()
{
}
void CObjectInspector::FindObjectAndLoadTree()
{
	HWND hwndFindEditBox = GetDlgItem(_hwnd, FIND_EDITBOX);
	LPMSG msg = (LPMSG)malloc(sizeof(LPMSG));
	GetMessage(msg, hwndFindEditBox, 0, 0);


	return;

}
void CObjectInspector::Init()
{

}
void CObjectInspector::Process()
{
	if(GetKeyDown(Keys::KEY_U))
	{
		CreateThread(0, NULL, ThreadProc, NULL, NULL, NULL);
	}
}
/// <summary>
/// 
/// </summary>
/// <param name="szClassName"></param>
/// <returns></returns>
BOOL CObjectInspector::RegisterDLLWindowClass(wchar_t szClassName[])
{
	WNDCLASSEX wc;
	wc.hInstance = SEMod::hinstDLL;
	wc.lpszClassName = (LPCWSTR)szClassName;
	wc.lpfnWndProc = DLLWindowProc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.cbSize = sizeof(wc);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.lpszMenuName = NULL;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND;
	if (!RegisterClassEx(&wc))
	{
		DWORD ac = GetLastError();
		char error[40];
		sprintf(error, "RegisterClassEx Call Error %d", ac);
		CA2T _error(error);

		MessageBox(NULL, _error, _T("ERROR"),
			MB_ICONERROR && MB_OK);
		return 0;
	}
	MSG messages;
	int wHeight = 600, wWidth = 400;

	HWND prnt_wnd = FindWindow(L"KoTR", L"KoTR");
	_hwnd = CreateWindowEx(0, (LPCWSTR)szClassName, NULL, WS_EX_PALETTEWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, wWidth, wHeight, prnt_wnd, NULL, SEMod::hinstDLL, NULL);
	CreateWindow(L"EDIT", 0, WS_CHILD | WS_VISIBLE, 5, 2, wWidth - 60, 17, _hwnd, (HMENU)FIND_EDITBOX, (HINSTANCE)GetWindowLong(_hwnd, GWL_HINSTANCE), NULL);
	CreateWindow(L"BUTTON", L"Find", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, wWidth - 55, 1, 50, 17, _hwnd, (HMENU)BUTTON_FIND, (HINSTANCE)GetWindowLong(_hwnd, GWL_HINSTANCE), NULL);
	CreateWindow(L"BUTTON", L"btn", BS_DEFPUSHBUTTON | WS_CHILD | WS_VISIBLE, 5, wHeight - 50, 60, 20, _hwnd, (HMENU)BUTTON_UPDATE, (HINSTANCE)GetWindowLong(_hwnd, GWL_HINSTANCE), NULL);
	CreateWindow(L"COMBOLBOX", L"lBox", WS_CHILD | WS_VISIBLE, 5, 20, wWidth - 15, wHeight - 80, _hwnd, (HMENU)LIST_BOX, (HINSTANCE)GetWindowLong(_hwnd, GWL_HINSTANCE), NULL);
	ShowWindow(_hwnd, SW_SHOWNORMAL);
	while (GetMessage(&messages, prnt_wnd, 0, 0))
	{
		TranslateMessage(&messages);
		DispatchMessage(&messages);
	}
	return 1;

}

LRESULT CALLBACK CObjectInspector::DLLWindowProc(HWND _hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		break;
	case WM_COMMAND:
		switch (wParam)
		{
		case BUTTON_UPDATE:
			UpdateListBox();
			break;
		case BUTTON_FIND:
			FindObjectAndLoadTree();
			break;
		case LIST_BOX:
			switch (lParam)
			{
			case WM_RBUTTONDBLCLK:
				break;
			}
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(_hwnd, message, wParam, lParam);
	}
	return 0;
}
