#include "ExtensionRegister.h"
//class ExtensionRegister
//{
//	static void RegisterExtensions(SEMod* core)
//	{
//		core->RegisterExtension(new CObjectInspector());
//	}
//};

void ExtensionRegister::RegisterExtensions(SEMod* core)
{
	core->RegisterExtension(new CObjectInspector());
}