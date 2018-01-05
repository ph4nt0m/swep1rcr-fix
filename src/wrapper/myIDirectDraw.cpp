#include "myIDirectDraw.h"
#include "wrapper.h"

myIDirectDraw::myIDirectDraw(IDirectDraw* aOriginal) : mOriginal(aOriginal)
{}

myIDirectDraw::~myIDirectDraw()
{}

HRESULT __stdcall myIDirectDraw::QueryInterface(REFIID riid, LPVOID FAR * ppvObj)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->QueryInterface(riid, ppvObj);

  // Try to wrap returned interface
  if (SUCCEEDED(res))
    *ppvObj = wrapItem(riid, *ppvObj);

  LeaveCriticalSection(&gl_CS);
  return res;
}

ULONG __stdcall myIDirectDraw::AddRef()
{ return mOriginal->AddRef(); }

ULONG __stdcall myIDirectDraw::Release()
{
  EnterCriticalSection(&gl_CS);

  ULONG refCount = mOriginal->Release();
  if (refCount == 0)
  {
    deleteWrappedItem(mOriginal);
    mOriginal = nullptr;
    gl_pmyIDirectDraw = nullptr;
    delete this;
  }

  LeaveCriticalSection(&gl_CS);
  return refCount;
}

HRESULT __stdcall myIDirectDraw::Compact()
{ return mOriginal->Compact(); }

HRESULT __stdcall myIDirectDraw::CreateClipper(DWORD a, LPDIRECTDRAWCLIPPER FAR * b, IUnknown FAR * c)
{ return mOriginal->CreateClipper(a, b, c); }

HRESULT __stdcall myIDirectDraw::CreatePalette(DWORD a, LPPALETTEENTRY b, LPDIRECTDRAWPALETTE FAR * c, IUnknown FAR * d)
{ return mOriginal->CreatePalette(a, b, c, d); }

HRESULT __stdcall myIDirectDraw::CreateSurface(LPDDSURFACEDESC a, LPDIRECTDRAWSURFACE FAR * b, IUnknown FAR * c)
{ return mOriginal->CreateSurface(a, b, c); }

HRESULT __stdcall myIDirectDraw::DuplicateSurface(LPDIRECTDRAWSURFACE a, LPDIRECTDRAWSURFACE FAR * b)
{ return mOriginal->DuplicateSurface(a, b); }

HRESULT __stdcall myIDirectDraw::EnumDisplayModes(DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMMODESCALLBACK d)
{ return mOriginal->EnumDisplayModes(a, b, c, d); }

HRESULT __stdcall myIDirectDraw::EnumSurfaces(DWORD a, LPDDSURFACEDESC b, LPVOID c, LPDDENUMSURFACESCALLBACK d)
{ return mOriginal->EnumSurfaces(a, b, c, d); }

HRESULT __stdcall myIDirectDraw::FlipToGDISurface()
{ return mOriginal->FlipToGDISurface(); }

HRESULT __stdcall myIDirectDraw::GetCaps(LPDDCAPS a, LPDDCAPS b)
{ return mOriginal->GetCaps(a, b); }

HRESULT __stdcall myIDirectDraw::GetDisplayMode(LPDDSURFACEDESC a)
{ return mOriginal->GetDisplayMode(a); }

HRESULT __stdcall myIDirectDraw::GetFourCCCodes(LPDWORD a, LPDWORD b)
{ return mOriginal->GetFourCCCodes(a, b); }

HRESULT __stdcall myIDirectDraw::GetGDISurface(LPDIRECTDRAWSURFACE FAR * a)
{ return mOriginal->GetGDISurface(a); }

HRESULT __stdcall myIDirectDraw::GetMonitorFrequency(LPDWORD a)
{ return mOriginal->GetMonitorFrequency(a); }

HRESULT __stdcall myIDirectDraw::GetScanLine(LPDWORD a)
{ return mOriginal->GetScanLine(a); }

HRESULT __stdcall myIDirectDraw::GetVerticalBlankStatus(LPBOOL a)
{ return mOriginal->GetVerticalBlankStatus(a); }

HRESULT __stdcall myIDirectDraw::Initialize(GUID FAR * a)
{ return mOriginal->Initialize(a); }

HRESULT __stdcall myIDirectDraw::RestoreDisplayMode()
{ return mOriginal->RestoreDisplayMode(); }

HRESULT __stdcall myIDirectDraw::SetCooperativeLevel(HWND a, DWORD b)
{ return mOriginal->SetCooperativeLevel(a, b); }

HRESULT __stdcall myIDirectDraw::SetDisplayMode(DWORD a, DWORD b, DWORD c)
{ return mOriginal->SetDisplayMode(a, b, c); }

HRESULT __stdcall myIDirectDraw::WaitForVerticalBlank(DWORD a, HANDLE b)
{ return mOriginal->WaitForVerticalBlank(a, b); }
