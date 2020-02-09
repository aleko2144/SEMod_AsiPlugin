#ifndef _DLL_H_
#define _DLL_H_
#define BUILDING_DLL 1

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT DllClass
{
  public:
	DllClass();
	//virtual ~DllClass();
	//void HelloWorld();
};

#endif
