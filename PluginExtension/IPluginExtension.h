#pragma once
class IPluginExtension
{
public:
	virtual void Init() = 0;
	virtual void Process() = 0;
};

