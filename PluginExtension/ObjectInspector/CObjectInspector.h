#pragma once
#include <Windows.h>
#include "..\..\include\Formats\CVector.h"
#include "..\IPluginExtension.h"
#include "..\..\include\Utils\PluginUtils.h"
#include <list>
#include "..\..\include\ModulesData\Block.h"

class CObjectInspector : public IPluginExtension
{
public:
	struct CBlockContainer;
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
		CBlockContainer* m_pParentBlock;
	};
	struct CBlockContainer
	{
			CSimpleBlock * _block;
			int* m_pSubBlocks;
			int* m_dwSubBlocksCount;
	};
	static BOOL RegisterDLLWindowClass(wchar_t szClassName[]);
	static LRESULT CALLBACK DLLWindowProc(HWND, UINT, WPARAM, LPARAM);
	static HMENU CreateDLLWindowMenu();
	static DWORD WINAPI ThreadProc(LPVOID lpParam);
	static void UpdateListBox();
	static void LoadParentTree(CSimpleBlock* block);
	static void FindObjectAndLoadTree();
	static void FillListBox(list<char*> *_list);
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



	

	// Унаследовано через IPluginExtension
	virtual void Init() override;

	virtual void Process() override;

};
