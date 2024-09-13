#pragma once 

#ifndef WIN32
	#define BASE_API
#else
	#ifdef BASE_EXPORT
		#define BASE_API  __declspec( dllexport )
	#else
		#define BASE_API  __declspec( dllimport )
	#endif // 0
#endif // !WIN32

