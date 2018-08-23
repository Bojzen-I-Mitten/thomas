#include "ScriptingManager.h"


void ThomasEngine::ScriptingManger::OnChanged(System::Object ^sender, System::IO::FileSystemEventArgs ^e)
{
	fsw->EnableRaisingEvents = false;
	FileInfo^ fInfo = gcnew FileInfo(e->FullPath);
	while (IsFileLocked(fInfo)) {
		Thread::Sleep(500);
	}
	Thread::Sleep(1000);
	LoadAssembly();
}