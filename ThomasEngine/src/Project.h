#pragma once

#include "Scene.h"
using namespace System;
using namespace System::Runtime::Serialization;
using namespace System::ComponentModel;
namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Project
	{
	private:
		[DataMemberAttribute]
		String ^ m_name;
		[DataMemberAttribute]
		String^ m_path;
		[DataMemberAttribute]
		String^ m_resourcePath;
		[DataMemberAttribute]
		String^ m_assemblyPath;
		
	internal:
		Project() {};

	public:
		Project(String^ name, String^ absolutePath)
		{
			m_name = name;
			m_path = absolutePath + "\\" + name;
			m_resourcePath = m_path + "\\" + "Assets";
			m_assemblyPath = m_path + "\\" + "Assembly";
			System::IO::Directory::CreateDirectory(m_path);
			System::IO::Directory::CreateDirectory(m_resourcePath);
			System::IO::Directory::CreateDirectory(m_assemblyPath);
			System::IO::File::Create(m_path + "\\" + m_name + ".thomas");
		}
		Project(String^ projectFile) {
			m_name = System::IO::Path::GetFileNameWithoutExtension(projectFile);
			m_path = System::IO::Path::GetDirectoryName(projectFile);
			m_resourcePath = m_path + "\\" + "Assets";
			m_assemblyPath = m_path + "\\" + "Assembly";
		}

		property String^ name
		{
			String^ get() { return m_name; }
		}
		property String^ path
		{
			String^ get() { return m_path; }
		}
		property String^ assetPath
		{
			String^ get() { return m_resourcePath; }
		}
		property String^ assembly
		{
			String^ get() { return m_assemblyPath; }
		}
	};
}
