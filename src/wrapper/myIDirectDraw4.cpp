#include "myIDirectDraw4.h"
#include "wrapper.h"

myIDirectDraw4::myIDirectDraw4(IDirectDraw4* aOriginal) : mOriginal(aOriginal)
{}

myIDirectDraw4::~myIDirectDraw4()
{}

HRESULT __stdcall myIDirectDraw4::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->QueryInterface(riid, ppvObj);

  // Try to wrap returned interface
  if (SUCCEEDED(res))
    *ppvObj = wrapItem(riid, *ppvObj);

  LeaveCriticalSection(&gl_CS);
  return res;
}

ULONG __stdcall myIDirectDraw4::AddRef()
{ return mOriginal->AddRef(); }

ULONG __stdcall myIDirectDraw4::Release()
{
  EnterCriticalSection(&gl_CS);

  ULONG refCount = mOriginal->Release();
  if (refCount == 0)
  {
    deleteWrappedItem(mOriginal);
    mOriginal = nullptr;
    delete this;
  }

  LeaveCriticalSection(&gl_CS);
  return refCount;
}

HRESULT __stdcall myIDirectDraw4::Compact()
{ return mOriginal->Compact(); }

HRESULT __stdcall myIDirectDraw4::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR * b, IUnknown FAR * c)
{ return mOriginal->CreateClipper(a, b, c); }

HRESULT __stdcall myIDirectDraw4::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR * c, IUnknown FAR * d)
{ return mOriginal->CreatePalette(a, b, c, d); }

HRESULT __stdcall myIDirectDraw4::CreateSurface(LPDDSURFACEDESC2 a, LPDIRECTDRAWSURFACE4 FAR * b, IUnknown FAR * c)
{ return mOriginal->CreateSurface(a, b, c); }

HRESULT __stdcall myIDirectDraw4::DuplicateSurface(LPDIRECTDRAWSURFACE4 a, LPDIRECTDRAWSURFACE4 FAR * b)
{ return mOriginal->DuplicateSurface(a, b); }

HRESULT __stdcall myIDirectDraw4::EnumDisplayModes(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMMODESCALLBACK2 d)
{ return mOriginal->EnumDisplayModes(a, b, c, d); }

HRESULT __stdcall myIDirectDraw4::EnumSurfaces(DWORD a, LPDDSURFACEDESC2 b, LPVOID c, LPDDENUMSURFACESCALLBACK2 d)
{ return mOriginal->EnumSurfaces(a, b, c, d); }

HRESULT __stdcall myIDirectDraw4::FlipToGDISurface()
{ return mOriginal->FlipToGDISurface(); }

HRESULT __stdcall myIDirectDraw4::GetCaps(LPDDCAPS a, LPDDCAPS b)
{ return mOriginal->GetCaps(a, b); }

HRESULT __stdcall myIDirectDraw4::GetDisplayMode(LPDDSURFACEDESC2 a)
{ return mOriginal->GetDisplayMode(a); }

HRESULT __stdcall myIDirectDraw4::GetFourCCCodes(LPDWORD a, LPDWORD b)
{ return mOriginal->GetFourCCCodes(a, b); }

HRESULT __stdcall myIDirectDraw4::GetGDISurface(LPDIRECTDRAWSURFACE4 FAR * a)
{ return mOriginal->GetGDISurface(a); }

HRESULT __stdcall myIDirectDraw4::GetMonitorFrequency(LPDWORD a)
{ return mOriginal->GetMonitorFrequency(a); }

HRESULT __stdcall myIDirectDraw4::GetScanLine(LPDWORD a)
{ return mOriginal->GetScanLine(a); }

HRESULT __stdcall myIDirectDraw4::GetVerticalBlankStatus(LPBOOL a)
{ return mOriginal->GetVerticalBlankStatus(a); }

HRESULT __stdcall myIDirectDraw4::Initialize(GUID FAR * a)
{ return mOriginal->Initialize(a); }

HRESULT __stdcall myIDirectDraw4::RestoreDisplayMode()
{ return mOriginal->RestoreDisplayMode(); }

HRESULT __stdcall myIDirectDraw4::SetCooperativeLevel(HWND a, DWORD b)
{ return mOriginal->SetCooperativeLevel(a, b); }

HRESULT __stdcall myIDirectDraw4::SetDisplayMode(DWORD a, DWORD b, DWORD c, DWORD d, DWORD e)
{ return mOriginal->SetDisplayMode(a, b, c, d, e); }

HRESULT __stdcall myIDirectDraw4::WaitForVerticalBlank(DWORD a, HANDLE b)
{ return mOriginal->WaitForVerticalBlank(a, b); }

HRESULT __stdcall myIDirectDraw4::GetAvailableVidMem(LPDDSCAPS2 a, LPDWORD b, LPDWORD c)
{ return mOriginal->GetAvailableVidMem(a, b, c); }

HRESULT __stdcall myIDirectDraw4::GetSurfaceFromDC(HDC a, LPDIRECTDRAWSURFACE4 * b)
{ return mOriginal->GetSurfaceFromDC(a, b); }

HRESULT __stdcall myIDirectDraw4::RestoreAllSurfaces()
{ return mOriginal->RestoreAllSurfaces(); }

HRESULT __stdcall myIDirectDraw4::TestCooperativeLevel()
{ return mOriginal->TestCooperativeLevel(); }

HRESULT __stdcall myIDirectDraw4::GetDeviceIdentifier(LPDDDEVICEIDENTIFIER a, DWORD b)
{ return mOriginal->GetDeviceIdentifier(a, b); }
