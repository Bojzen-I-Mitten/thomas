#pragma once
#include "ThomasManaged.h"
using namespace System;
namespace ThomasEngine {
	public ref class Debug {
	public:
		static void Log(Object^ message) {
			ThomasWrapper::OutputLog->Add(message->ToString());
		}
	};
}