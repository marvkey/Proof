#pragma once

#ifdef PF_PLATFORM_WINDOW64
#ifdef  PF_BUILD_DLL
		#define Proof_API __declspec(dllexport)
#else
		#define Proof_API __declspec(dllimport)
	#endif
#else We Only support winodws 64 Bit
#endif