#pragma once
#include <Windows.h>
#include <string>
#include <cstdarg>
using std::string;

namespace StringUtils
{
	string Sprintf(const char* inFormat, ...);

	void	Log(const char* inFormat, ...);
}

#define LOG( ... ) StringUtils::Log( __VA_ARGS__ );

