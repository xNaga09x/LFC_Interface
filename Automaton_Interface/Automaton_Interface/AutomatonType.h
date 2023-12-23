#pragma once
#include <string>
enum class AutomatonType
{
	AFDType,
	AFNType,
	AFNLType,
	APDType
};

std::string toString(AutomatonType type);
