#pragma once

#include "Project.h"
using namespace System;
namespace ThomasEditor
{
	public ref class Application
	{
	public:
		delegate void CurrentProjectChangedEvent(Project^ newProject);
		static event CurrentProjectChangedEvent^ currentProjectChanged;
		static Project^ m_currentProject;
		static String^ editorAssets = "..\\Data\\";

		static property Project^ currentProject
		{
			Project^ get() { return m_currentProject; }
			void set(Project^ value) {
				m_currentProject = value;
				currentProjectChanged(value);
			}
		}
	};
}
