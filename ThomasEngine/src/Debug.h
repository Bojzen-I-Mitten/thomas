#pragma once
#include "ThomasManaged.h"
using namespace System;
namespace ThomasEditor {
	public ref class Debug {
	public:
		static void Log(Object^ message) {
			ThomasWrapper::OutputLog->Add(message->ToString());
		}
	};
}