#pragma once
#pragma unmanaged
#include "thomas\resource\Resource.h"
#include <memory>
#pragma managed

#include "../Utility.h"
#include "../Scene.h"

using namespace System;
using namespace System::Runtime::Serialization;
using namespace System::ComponentModel;

namespace ThomasEditor
{
	[DataContractAttribute]
	public ref class Resource : public INotifyPropertyChanged
	{
	internal:
		thomas::resource::Resource* m_nativePtr;

		[DataMemberAttribute]
		String^ m_path;

		Resource(String^ path, thomas::resource::Resource* ptr)
		{
			m_path = path;
			m_nativePtr = ptr;
		}
		virtual ~Resource()
		{
		}

		void Rename(String^ newPath) {
			m_path = newPath;
			m_nativePtr->Rename(Utility::ConvertString(newPath));
			OnPropertyChanged("name");
		}

	public:
		virtual event PropertyChangedEventHandler^ PropertyChanged;
		void OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew PropertyChangedEventArgs(info));
		}

		virtual void Reload() {
			System::Threading::Monitor::Enter(Scene::CurrentScene->GetGameObjectsLock());
			m_nativePtr->Reload();
			System::Threading::Monitor::Exit(Scene::CurrentScene->GetGameObjectsLock());
		};

		String ^ GetPath()
		{
			return m_path;
		}

		virtual property String^ name
		{
			String^ get() { return System::IO::Path::GetFileNameWithoutExtension(m_path); }
		};
	};
}