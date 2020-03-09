#ifndef __redis_define_h__
#define __redis_define_h__

#ifndef HIREDIS3CPP_API
#	ifdef HIREDIS3CPP_DLL
#		ifdef HIREDIS3CPP_EXPORTS
#			define HIREDIS3CPP_API __declspec(dllexport)
#		else
#			define HIREDIS3CPP_API __declspec(dllimport)
#		endif
#	else
#		define HIREDIS3CPP_API
#	endif
#endif

#include <vector>
#include <iostream>
#include <string>
#include <utility>

#endif // end of __redis_define_h__