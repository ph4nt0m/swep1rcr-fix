#define STRICT
#define WIN32_LEAN_AND_MEAN
#define INITGUID
#define _CRT_SECURE_NO_WARNINGS

#include "proxy.h"
#include "wrapper/wrapper.h"
#include <windows.h>

static HMODULE originalDLL = NULL;

// These are the exported functions of the original ddraw.dll
static const char* originalExports[] =
{
  "AcquireDDThreadLock",
  "CompleteCreateSysmemSurface",
  "D3DParseUnknownCommand",
  "DDGetAttachedSurfaceLcl",
  "DDInternalLock",
  "DDInternalUnlock",
  "DirectDrawCreate",
  "DirectDrawCreateClipper",
  "DirectDrawCreateEx",
  "DirectDrawEnumerateA",
  "DirectDrawEnumerateExA",
  "DirectDrawEnumerateExW",
  "DirectDrawEnumerateW",
  "DllCanUnloadNow",
  "DllGetClassObject",
  "DSoundHelp",
  "GetDDSurfaceLocal",
  "GetOLEThunkData",
  "GetSurfaceFromDC",
  "RegisterSpecialCase",
  "ReleaseDDThreadLock",
  "SetAppCompatData"
};

static FARPROC originalExportAddr[22] = { 0 };

bool loadOriginalDLL()
{
  char path[MAX_PATH];
  GetSystemDirectoryA(path, MAX_PATH);
  strcat(path, "\\ddraw.dll");

  originalDLL = LoadLibraryA(path);

  if (!originalDLL)
    return false;

  // If loading succeeded, get addresses of all DLL functions
  for (unsigned int i = 0; i < sizeof(originalExports) / sizeof(originalExports[0]); ++i)
    originalExportAddr[i] = GetProcAddress(originalDLL, originalExports[i]);

  return true;
}

void unloadOriginalDLL()
{
  if (originalDLL)
  {
    FreeLibrary(originalDLL);
    originalDLL = NULL;
  }
}

// All exported functions (especially undocumented ones) of the original
// ddraw.dll can be forwarded to by a simple jmp instruction.
// The __declspec(naked) will leave the stack unchanged (see
// https://www.codeproject.com/Articles/16541/Create-your-Proxy-DLLs-automatically
// for more details).

extern "C" void __declspec(naked) myAcquireDDThreadLock()
{ __asm jmp originalExportAddr[0 * 4] }
extern "C" void __declspec(naked) myCompleteCreateSysmemSurface()
{ __asm jmp originalExportAddr[1 * 4] }
extern "C" void __declspec(naked) myD3DParseUnknownCommand()
{ __asm jmp originalExportAddr[2 * 4] }
extern "C" void __declspec(naked) myDDGetAttachedSurfaceLcl()
{ __asm jmp originalExportAddr[3 * 4] }
extern "C" void __declspec(naked) myDDInternalLock()
{ __asm jmp originalExportAddr[4 * 4] }
extern "C" void __declspec(naked) myDDInternalUnlock()
{ __asm jmp originalExportAddr[5 * 4] }
//extern "C" void __declspec(naked) myDirectDrawCreate()
//{ __asm jmp originalExportAddr[6 * 4] }
extern "C" void __declspec(naked) myDirectDrawCreateClipper()
{ __asm jmp originalExportAddr[7 * 4] }
extern "C" void __declspec(naked) myDirectDrawCreateEx()
{ __asm jmp originalExportAddr[8 * 4] }
extern "C" void __declspec(naked) myDirectDrawEnumerateA()
{ __asm jmp originalExportAddr[9 * 4] }
extern "C" void __declspec(naked) myDirectDrawEnumerateExA()
{ __asm jmp originalExportAddr[10 * 4] }
extern "C" void __declspec(naked) myDirectDrawEnumerateExW()
{ __asm jmp originalExportAddr[11 * 4] }
extern "C" void __declspec(naked) myDirectDrawEnumerateW()
{ __asm jmp originalExportAddr[12 * 4] }
extern "C" void __declspec(naked) myDllCanUnloadNow()
{ __asm jmp originalExportAddr[13 * 4] }
extern "C" void __declspec(naked) myDllGetClassObject()
{ __asm jmp originalExportAddr[14 * 4] }
extern "C" void __declspec(naked) myDSoundHelp()
{ __asm jmp originalExportAddr[15 * 4] }
extern "C" void __declspec(naked) myGetDDSurfaceLocal()
{ __asm jmp originalExportAddr[16 * 4] }
extern "C" void __declspec(naked) myGetOLEThunkData()
{ __asm jmp originalExportAddr[17 * 4] }
extern "C" void __declspec(naked) myGetSurfaceFromDC()
{ __asm jmp originalExportAddr[18 * 4] }
extern "C" void __declspec(naked) myRegisterSpecialCase()
{ __asm jmp originalExportAddr[19 * 4] }
extern "C" void __declspec(naked) myReleaseDDThreadLock()
{ __asm jmp originalExportAddr[20 * 4] }
extern "C" void __declspec(naked) mySetAppCompatData()
{ __asm jmp originalExportAddr[21 * 4] }

// All functions that really need to be modified will now be treated separately

extern "C" HRESULT __stdcall myDirectDrawCreate(GUID* a, LPDIRECTDRAW* b, IUnknown* c)
{
  // Get function pointer to real DirectDrawCreate()
  typedef HRESULT(__stdcall *CreateProc)(GUID* a, LPDIRECTDRAW* b, IUnknown* c);
  CreateProc pCreate = reinterpret_cast<CreateProc>(originalExportAddr[6]);

  // Call the real DirectDrawCreate()
  HRESULT hr = pCreate(a, b, c);

  if (SUCCEEDED(hr))
  {
    // Create wrapper object and store a pointer to the original object there
    auto wrapper = wrapItem(IID_IDirectDraw, *b);
    gl_pmyIDirectDraw = static_cast<myIDirectDraw*>(wrapper);

    // Return wrapper instead of original object
    *b = static_cast<IDirectDraw*>(wrapper);
  }

  return hr;
}
