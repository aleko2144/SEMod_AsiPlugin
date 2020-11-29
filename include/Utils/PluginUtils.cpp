#include <windows.h>
#include <iostream>
#include <string>
#include "PluginUtils.h"

using namespace std;

int UseFuncLog = -1;
int UseVehLog = -1;

bool GetPrivateProfileBoolean(string selection, string varname, string default_val, string filename){
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	string result = returnedString;

	if (result.find("on") != std::string::npos) //Case Switch
	{
		return true;
	}
	else if (result.find("true") != std::string::npos){
		return true;
	}
	else
	{
		return false;
	}
}
float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename){
	float to_return;
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	to_return = atof(returnedString);
	return to_return;
}

string GetPrivateProfileLine(string selection, string varname, string default_val, string filename){
	char* returnedString = new char[512];
	GetPrivateProfileStringA(selection.c_str(), varname.c_str(), default_val.c_str(), returnedString, 512, filename.c_str());
	string result = returnedString;
	return result;
}

void WriteLog(const char *file_name, const char *text)
{
	FILE *file = fopen(file_name, "a");
	if (file != NULL)
	{
		fputs(text, file);
		fputs("\n", file);
		fclose(file);
	}
}

bool IsKeyPressed(int key)
{
	return HIBYTE(GetKeyState(key)) == 0xFF;
}

void WriteDebugLog(const char *text)
{
	if (UseFuncLog == -1){
		if(GetPrivateProfileIntA("COMMON", "WriteDebugLog", 0, ".\\SEMod.ini")){
			UseFuncLog = 1;
		}
	}
	if (UseFuncLog > 0){
		FILE *file = fopen("SEMod_functions.log", "a");
		if (file != NULL)
		{
			fputs(text, file);
			fputs("\n", file);
			fclose(file);
		}
	}
}

void WriteVehicleLog(const char *file_name, const char *text)
{
	if (UseVehLog == -1){
		if(GetPrivateProfileIntA("COMMON", "VehicleDebugMode", 0, ".\\SEMod.ini")){
			UseVehLog = 1;
		}
	}
	if (UseVehLog > 0){
		FILE *file = fopen(file_name, "a");
		if (file != NULL)
		{
			fputs(text, file);
			fputs("\n", file);
			fclose(file);
		}
	}
}

bool keyPressed;
bool IsKeyJustPressed(int key)
{
	if (IsKeyPressed(key)){
		if (!keyPressed){
			keyPressed = true;
			return true;
		}
	}
	else {
		keyPressed = false;
	}
}