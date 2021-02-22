#ifndef MOD_CORE_H
#define MOD_CORE_H
#include <list>
#include "../../PluginExtension/IPluginExtension.h"
#include <windows.h>
#include <memory>
class SEMod
{
public:
	void Initialize(HINSTANCE Instance);
	void Process();
	void RegisterExtension(IPluginExtension *extension);
	static HINSTANCE GetDLLInstance();
	SEMod(HINSTANCE Instance);
	SEMod();


private:
	static HINSTANCE hinstDLL;
	std::list<IPluginExtension*> plugins = {};

//	SEMod(HINSTANCE hInst);
//private:

};

#endif