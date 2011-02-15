#ifndef __shlib2__
#define __shlib2__

#include <string>

// DLL EXPORT
#ifdef WIN32
	#ifdef EXPORT_DLL
		#define PUBLIC __declspec( dllexport )
	#else
		#define PUBLIC __declspec( dllimport )
	#endif
#else
	#define PUBLIC
#endif

PUBLIC std::string dynWhoAmI2( void );

#endif
