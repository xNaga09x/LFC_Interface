#include "AutomatonType.h"

std::string toString(AutomatonType type)
{
	switch (type)
	{
	case AutomatonType::AFDType:
		return "AFD";
	case AutomatonType::AFNType:
		return "AFN";
	case AutomatonType::AFNLType:
		return "AFNL";
	case AutomatonType::APDType:
		return "APD";
	default:
		return "NA";
	}
}
