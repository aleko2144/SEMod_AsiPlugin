#ifndef MOD_CORE_H
#define MOD_CORE_H
#include <list>
#include "../../PluginExtension/IPluginExtension.h"

class SEMod
{
public:
	void Initialize();
	void Process();
	void RegisterExtension(IPluginExtension *extension);
	std::list<IPluginExtension*> plugins = {};
	static HINSTANCE hinstDLL;

	//	HINSTANCE GetDLLInstance();
//	SEMod(HINSTANCE hInst);
//private:

};

#endif