#include "myIDirect3D3.h"
#include "wrapper.h"

myIDirect3D3::myIDirect3D3(IDirect3D3 * aOriginal) : mOriginal(aOriginal)
{}

myIDirect3D3::~myIDirect3D3()
{}

HRESULT __stdcall myIDirect3D3::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->QueryInterface(riid, ppvObj);

  // Try to wrap returned interface
  if (SUCCEEDED(res))
    *ppvObj = wrapItem(riid, *ppvObj);

  LeaveCriticalSection(&gl_CS);
  return res;
}

ULONG __stdcall myIDirect3D3::AddRef()
{ return mOriginal->AddRef(); }

ULONG __stdcall myIDirect3D3::Release()
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

HRESULT __stdcall myIDirect3D3::EnumDevices(LPD3DENUMDEVICESCALLBACK a, LPVOID b)
{ return mOriginal->EnumDevices(a, b); }

HRESULT __stdcall myIDirect3D3::CreateLight(LPDIRECT3DLIGHT * a, LPUNKNOWN b)
{ return mOriginal->CreateLight(a, b); }

HRESULT __stdcall myIDirect3D3::CreateMaterial(LPDIRECT3DMATERIAL3 * a, LPUNKNOWN b)
{ return mOriginal->CreateMaterial(a, b); }

HRESULT __stdcall myIDirect3D3::CreateViewport(LPDIRECT3DVIEWPORT3 * a, LPUNKNOWN b)
{ return mOriginal->CreateViewport(a, b); }

HRESULT __stdcall myIDirect3D3::FindDevice(LPD3DFINDDEVICESEARCH a, LPD3DFINDDEVICERESULT b)
{ return mOriginal->FindDevice(a, b); }

HRESULT __stdcall myIDirect3D3::CreateDevice(REFCLSID a, LPDIRECTDRAWSURFACE4 b, LPDIRECT3DDEVICE3 * c, LPUNKNOWN d)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->CreateDevice(a, b, c, d);

  if (SUCCEEDED(res))
  {
    // Create wrapper object and store pointer to original interface there
    auto wrapper = wrapItem(IID_IDirect3DDevice3, *c);
    gl_pmyIDirect3DDevice3 = static_cast<myIDirect3DDevice3*>(wrapper);

    // Return wrapper instead of original interface
    *c = static_cast<IDirect3DDevice3*>(wrapper);
  }

  LeaveCriticalSection(&gl_CS);
  return res;
}

HRESULT __stdcall myIDirect3D3::CreateVertexBuffer(LPD3DVERTEXBUFFERDESC a, LPDIRECT3DVERTEXBUFFER * b, DWORD c, LPUNKNOWN d)
{ return mOriginal->CreateVertexBuffer(a, b, c, d); }

/*
HRESULT WINAPI EnumZBufferCallback(DDPIXELFORMAT* pddpf, LPVOID lpContext)
{
  // If this is ANY type of depth-buffer, stop
  if (pddpf->dwFlags == DDPF_ZBUFFER)
  {
    writeLog("Z-Buffer with format %d, %d, %d\n", pddpf->dwZBufferBitDepth, pddpf->dwRGBBitCount, pddpf->dwStencilBitDepth);

    if (pddpf->dwZBufferBitDepth < 32)
      return D3DENUMRET_OK;
  }

  void** arr = (void**)lpContext;

  LPD3DENUMPIXELFORMATSCALLBACK orgCallback = (LPD3DENUMPIXELFORMATSCALLBACK) (arr[0]);
  LPVOID orgContext = arr[1];

  return orgCallback(pddpf, orgContext);
}
*/

HRESULT __stdcall myIDirect3D3::EnumZBufferFormats(REFCLSID a, LPD3DENUMPIXELFORMATSCALLBACK b, LPVOID c)
{
  EnterCriticalSection(&gl_CS);

  //void* arr[2] = { b, c };
  //HRESULT x = mOriginal->EnumZBufferFormats(a, EnumZBufferCallback, arr);

  HRESULT x = mOriginal->EnumZBufferFormats(a, b, c);

  LeaveCriticalSection(&gl_CS);
  return x;
}

HRESULT __stdcall myIDirect3D3::EvictManagedTextures()
{ return mOriginal->EvictManagedTextures(); }
