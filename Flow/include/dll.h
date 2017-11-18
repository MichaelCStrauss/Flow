#ifdef WINDOWS
	#pragma once

	#ifdef FLOW_EXPORTS
		#define FLOW_API __declspec(dllexport)
	#else
		#define FLOW_API __declspec(dllimport)
	#endif
#else
	#define FLOW_API
#endif