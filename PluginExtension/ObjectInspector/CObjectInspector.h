#pragma once
#include <Windows.h>
#include "..\..\include\Formats\CVector.h"
#include "..\IPluginExtension.h"
#include "..\..\include\Utils\PluginUtils.h"

class CObjectInspector : public IPluginExtension
{
public:
	static BOOL RegisterDLLWindowClass(wchar_t szClassName[]);
	static LRESULT CALLBACK DLLWindowProc(HWND, UINT, WPARAM, LPARAM);
	static HMENU CreateDLLWindowMenu();
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	static void UpdateListBox();
	static void LoadParentTree();
	static void FindObjectAndLoadTree();
	static HWND _hwnd;

	struct HeapNameDef
	{
		int* m_pDataDynamic;
		int* m_dwMaxSize;
		int* m_pData;
		int* m_pCurrentModule;
	};
	struct HeapAllocatorFile
	{
		int* vmt;
		int* m_pPrev;
		int* m_pNext;
		int* m_sName;
		int* data;
		int* _module;
		int* field_18;
		int* field_1C;
	};
	static HeapNameDef* heapDef;

	struct CSimpleBlock
	{
		int* vmt;
		int* m_pPrev;
		int* m_pNext;
		int* m_sName;
		int* type;
		CVector4* pos;
		int* m_pModule;
		int* flags;
		int* field_29;
		int* field_2A;
		int* field_2B;
		int* m_pParentBlock;
	};

	

	// Унаследовано через IPluginExtension
	virtual void Init() override;

	virtual void Process() override;

};
