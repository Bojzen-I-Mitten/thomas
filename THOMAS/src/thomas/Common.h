#pragma once
#include <iostream>
#include <sstream>
#include <comdef.h>
#include "ThomasCore.h"
#define THOMAS_API


#ifdef _DEBUG
#define LOG(msg){std::stringstream __buff__; __buff__ << msg; thomas::ThomasCore::LogOutput(__buff__.str());}
#else
#define LOG(msg) ;
#endif

#ifdef _DEBUG
#define LOG_HR(hr){	_com_error err(hr); LPCTSTR  errMsg = err.ErrorMessage(); std::stringstream __buff__;  __buff__ << errMsg; thomas::ThomasCore::LogOutput(__buff__.str());}
#else
#define LOG_HR(hr) 
#endif

#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }


#define THOMAS_AA_COUNT 4
#define THOMAS_AA_QUALITY 0

#define _XM_NO_INTRINSICS_ 1; //Bullet fix