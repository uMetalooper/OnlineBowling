#pragma once
#include <string>

class ResourcesManager
{
public:
	static std::string rootPath;
	static std::string getAbsolutePathOf(const std::string& fpath)
	{
		return rootPath + '\\' + fpath;
	}
};

