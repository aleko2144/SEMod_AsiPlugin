#ifndef PLUGIN_UTILS_H
#define PLUGIN_UTILS_H

#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

bool GetPrivateProfileBoolean(string selection, string varname, string default_val, string filename);
float GetPrivateProfileFloat(string selection, string varname, string default_val, string filename);
string GetPrivateProfileLine(string selection, string varname, string default_val, string filename);
void PrintLog(const char *file_name, const char *text);
bool IsKeyPressed(int key);
	
#endif
