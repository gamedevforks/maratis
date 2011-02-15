// provide very simple library functionality

#include <string>

#include "shlib1.h"

PUBLIC std::string dynWhoAmI1( void )
{
	return "I'm the dynamic library 1";
}