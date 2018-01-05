#define STRICT
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include "proxy.h"
#include "util.h"

CRITICAL_SECTION gl_CS;

BOOL APIENTRY DllMain(HANDLE, DWORD dwReason, LPVOID)
{
  switch (dwReason)
  {
  case DLL_PROCESS_ATTACH:
    InitializeCriticalSection(&gl_CS);
    initLog();
    loadOriginalDLL();
    break;

  case DLL_PROCESS_DETACH:
    unloadOriginalDLL();
    closeLog();
    DeleteCriticalSection(&gl_CS);
    break;
  }

  return TRUE;
}
