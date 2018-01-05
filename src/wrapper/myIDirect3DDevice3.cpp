#include "myIDirect3DDevice3.h"
#include "wrapper.h"

myIDirect3DDevice3::myIDirect3DDevice3(IDirect3DDevice3 * aOriginal) : mOriginal(aOriginal)
{}

myIDirect3DDevice3::~myIDirect3DDevice3()
{}

HRESULT __stdcall myIDirect3DDevice3::QueryInterface(REFIID riid, LPVOID * ppvObj)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->QueryInterface(riid, ppvObj);

  // Try to wrap returned interface
  if (SUCCEEDED(res))
    *ppvObj = wrapItem(riid, *ppvObj);

  LeaveCriticalSection(&gl_CS);
  return res;
}

ULONG __stdcall myIDirect3DDevice3::AddRef()
{ return mOriginal->AddRef(); }

ULONG __stdcall myIDirect3DDevice3::Release()
{
  EnterCriticalSection(&gl_CS);

  ULONG refCount = mOriginal->Release();
  if (refCount == 0)
  {
    deleteWrappedItem(mOriginal);
    mOriginal = nullptr;
    gl_pmyIDirect3DDevice3 = nullptr;
    delete this;
  }

  LeaveCriticalSection(&gl_CS);
  return refCount;
}

HRESULT __stdcall myIDirect3DDevice3::GetCaps(LPD3DDEVICEDESC a, LPD3DDEVICEDESC b)
{ return mOriginal->GetCaps(a, b); }

HRESULT __stdcall myIDirect3DDevice3::GetStats(LPD3DSTATS a)
{ return mOriginal->GetStats(a); }

HRESULT __stdcall myIDirect3DDevice3::AddViewport(LPDIRECT3DVIEWPORT3 a)
{ return mOriginal->AddViewport(a); }

HRESULT __stdcall myIDirect3DDevice3::DeleteViewport(LPDIRECT3DVIEWPORT3 a)
{ return mOriginal->DeleteViewport(a); }

HRESULT __stdcall myIDirect3DDevice3::NextViewport(LPDIRECT3DVIEWPORT3 a, LPDIRECT3DVIEWPORT3 * b, DWORD c)
{ return mOriginal->NextViewport(a, b, c); }

HRESULT __stdcall myIDirect3DDevice3::EnumTextureFormats(LPD3DENUMPIXELFORMATSCALLBACK a, LPVOID b)
{ return mOriginal->EnumTextureFormats(a, b); }

HRESULT __stdcall myIDirect3DDevice3::BeginScene()
{
  EnterCriticalSection(&gl_CS);

  // THIS IS THE MAIN FIX FOR THE BLACK SCREEN PROBLEM
  // https://sourceforge.net/p/dxwnd/discussion/general/thread/444b7535/
  // Basically, SWEP1RCR does not clear the depth buffer after rendering a frame
  // Therefore, we will intercept BeginScene() and set the z buffer to 1.0

  LPDIRECT3DVIEWPORT3 vp;
  HRESULT res = mOriginal->GetCurrentViewport(&vp);

  if (SUCCEEDED(res))
  {
    // Retrieve the viewport registers of the viewport
    D3DVIEWPORT2 vpData;
    vpData.dwSize = sizeof(vpData); // very important, otherwise GetViewport2() will yield DDERR_INVALIDPARAMS
    HRESULT res = vp->GetViewport2(&vpData);

    if (SUCCEEDED(res))
    {
      // Set z buffer to 1.0
      D3DRECT d3dRect = { 0, 0, static_cast<LONG>(vpData.dwWidth), static_cast<LONG>(vpData.dwHeight) };
      vp->Clear2(1, &d3dRect, D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
      //vp->Clear(1, &d3dRect, D3DCLEAR_ZBUFFER);
    }
    vp->Release();
  }

  if (FAILED(res))
    writeLog("Clearing the depth buffer failed");

  // ALTERNATIVE SOLUTION
  // The following way of clearing the z buffer has been proposed by AxXxB:
  // http://www.vogons.org/viewtopic.php?t=39154&p=359689#p420468
  // https://sourceforge.net/p/dxwnd/discussion/general/thread/7258a78d/?limit=25&page=1#6299
  // It works by requesting direct access to the z buffer and then
  // setting each byte to 0xFF. In my experiments, this works equally
  // well, but the Clear2() method should still be the preferred
  // way in accordance with the DirectDraw API.

#if 0
  LPDIRECT3D3 d3d;
  mOriginal->GetDirect3D(&d3d);

  LPDIRECTDRAWSURFACE4 surf, zbuffer;
  mOriginal->GetRenderTarget(&surf);

  // Get pointer to z buffer
  DDSCAPS2 caps = { 0 };
  caps.dwCaps = DDSCAPS_ZBUFFER;
  res = surf->GetAttachedSurface(&caps, &zbuffer);

  if (SUCCEEDED(res))
  {
    DDSURFACEDESC2 desc = {};
    desc.dwSize = sizeof(desc);
    res = zbuffer->Lock(NULL, &desc, 0, NULL);

    if (SUCCEEDED(res))
    {
      memset(desc.lpSurface, 0xFF, desc.lPitch * desc.dwHeight);
      zbuffer->Unlock(NULL);
    }
    zbuffer->Release();
  }
#endif

  // Finally hand over to the original function
  res = mOriginal->BeginScene();

  LeaveCriticalSection(&gl_CS);
  return res;
}

HRESULT __stdcall myIDirect3DDevice3::EndScene()
{ return mOriginal->EndScene(); }

HRESULT __stdcall myIDirect3DDevice3::GetDirect3D(LPDIRECT3D3 * a)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res = mOriginal->GetDirect3D(a);

  if (SUCCEEDED(res))
  {
    // Create wrapper object and store pointer to original interface there
    // Note: The myIDirect3D3 wrapper object should already exist at this point,
    // since IDirect3D3 is required to create the IDirect3DDevice3
    auto wrapper = wrapItem(IID_IDirect3D3, *a);
    gl_pmyIDirect3D3 = static_cast<myIDirect3D3*>(wrapper);

    // Return wrapper instead of original interface
    *a = static_cast<IDirect3D3*>(wrapper);
  }

  LeaveCriticalSection(&gl_CS);
  return res;
}

HRESULT __stdcall myIDirect3DDevice3::SetCurrentViewport(LPDIRECT3DVIEWPORT3 a)
{ return mOriginal->SetCurrentViewport(a); }

HRESULT __stdcall myIDirect3DDevice3::GetCurrentViewport(LPDIRECT3DVIEWPORT3 * a)
{ return mOriginal->GetCurrentViewport(a); }

HRESULT __stdcall myIDirect3DDevice3::SetRenderTarget(LPDIRECTDRAWSURFACE4 a, DWORD b)
{ return mOriginal->SetRenderTarget(a, b); }

HRESULT __stdcall myIDirect3DDevice3::GetRenderTarget(LPDIRECTDRAWSURFACE4 * a)
{ return mOriginal->GetRenderTarget(a); }

HRESULT __stdcall myIDirect3DDevice3::Begin(D3DPRIMITIVETYPE a, DWORD b, DWORD c)
{ return mOriginal->Begin(a, b, c); }

HRESULT __stdcall myIDirect3DDevice3::BeginIndexed(D3DPRIMITIVETYPE a, DWORD b, LPVOID c, DWORD d, DWORD e)
{ return mOriginal->BeginIndexed(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::Vertex(LPVOID a)
{ return mOriginal->Vertex(a); }

HRESULT __stdcall myIDirect3DDevice3::Index(WORD a)
{ return mOriginal->Index(a); }

HRESULT __stdcall myIDirect3DDevice3::End(DWORD a)
{ return mOriginal->End(a); }

HRESULT __stdcall myIDirect3DDevice3::GetRenderState(D3DRENDERSTATETYPE a, LPDWORD b)
{ return mOriginal->GetRenderState(a, b); }

HRESULT __stdcall myIDirect3DDevice3::SetRenderState(D3DRENDERSTATETYPE a, DWORD b)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res;

  // Experimental modification of the depth buffer comparison function to mitigate z-fighting
  if (a == D3DRENDERSTATE_ZFUNC)
    res = mOriginal->SetRenderState(D3DRENDERSTATE_ZFUNC, D3DCMP_LESSEQUAL);
  else
    res = mOriginal->SetRenderState(a, b);

  LeaveCriticalSection(&gl_CS);
  return res;
}

HRESULT __stdcall myIDirect3DDevice3::GetLightState(D3DLIGHTSTATETYPE a, LPDWORD b)
{ return mOriginal->GetLightState(a, b); }

HRESULT __stdcall myIDirect3DDevice3::SetLightState(D3DLIGHTSTATETYPE a, DWORD b)
{ return mOriginal->SetLightState(a, b); }

HRESULT __stdcall myIDirect3DDevice3::SetTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b)
{
  EnterCriticalSection(&gl_CS);

  HRESULT res;
  if (a == D3DTRANSFORMSTATE_PROJECTION)
  {
    // The projection matrix is set by SWEP1RCR despite using pre-transformed vertices
    // Therefore, the following mod has no effect anyway
    // To mitigate the z-fighting, other measures would have to be taken
    float zn = 10.0f, zf = 10999.011f;
    b->_33 =  zf / (zf - zn);
    b->_43 = -zn * b->_33;
    res = mOriginal->SetTransform(a, b);
  }
  else
  {
    res = mOriginal->SetTransform(a, b);
  }

  LeaveCriticalSection(&gl_CS);
  return res;
}

HRESULT __stdcall myIDirect3DDevice3::GetTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b)
{ return mOriginal->GetTransform(a, b); }

HRESULT __stdcall myIDirect3DDevice3::MultiplyTransform(D3DTRANSFORMSTATETYPE a, LPD3DMATRIX b)
{ return mOriginal->MultiplyTransform(a, b); }

HRESULT __stdcall myIDirect3DDevice3::DrawPrimitive(D3DPRIMITIVETYPE a, DWORD b, LPVOID c, DWORD d, DWORD e)
{ return mOriginal->DrawPrimitive(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::DrawIndexedPrimitive(D3DPRIMITIVETYPE a, DWORD b, LPVOID c, DWORD d, LPWORD e, DWORD f, DWORD g)
{ return mOriginal->DrawIndexedPrimitive(a, b, c, d, e, f, g); }

HRESULT __stdcall myIDirect3DDevice3::SetClipStatus(LPD3DCLIPSTATUS a)
{ return mOriginal->SetClipStatus(a); }

HRESULT __stdcall myIDirect3DDevice3::GetClipStatus(LPD3DCLIPSTATUS a)
{ return mOriginal->GetClipStatus(a); }

HRESULT __stdcall myIDirect3DDevice3::DrawPrimitiveStrided(D3DPRIMITIVETYPE a, DWORD b, LPD3DDRAWPRIMITIVESTRIDEDDATA c, DWORD d, DWORD e)
{ return mOriginal->DrawPrimitiveStrided(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE a, DWORD b, LPD3DDRAWPRIMITIVESTRIDEDDATA c, DWORD d, LPWORD e, DWORD f, DWORD g)
{ return mOriginal->DrawIndexedPrimitiveStrided(a, b, c, d, e, f, g); }

HRESULT __stdcall myIDirect3DDevice3::DrawPrimitiveVB(D3DPRIMITIVETYPE a, LPDIRECT3DVERTEXBUFFER b, DWORD c, DWORD d, DWORD e)
{ return mOriginal->DrawPrimitiveVB(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::DrawIndexedPrimitiveVB(D3DPRIMITIVETYPE a, LPDIRECT3DVERTEXBUFFER b, LPWORD c, DWORD d, DWORD e)
{ return mOriginal->DrawIndexedPrimitiveVB(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::ComputeSphereVisibility(LPD3DVECTOR a, LPD3DVALUE b, DWORD c, DWORD d, LPDWORD e)
{ return mOriginal->ComputeSphereVisibility(a, b, c, d, e); }

HRESULT __stdcall myIDirect3DDevice3::GetTexture(DWORD a, LPDIRECT3DTEXTURE2 * b)
{ return mOriginal->GetTexture(a, b); }

HRESULT __stdcall myIDirect3DDevice3::SetTexture(DWORD a, LPDIRECT3DTEXTURE2 b)
{ return mOriginal->SetTexture(a, b); }

HRESULT __stdcall myIDirect3DDevice3::GetTextureStageState(DWORD a, D3DTEXTURESTAGESTATETYPE b, LPDWORD c)
{ return mOriginal->GetTextureStageState(a, b, c); }

HRESULT __stdcall myIDirect3DDevice3::SetTextureStageState(DWORD a, D3DTEXTURESTAGESTATETYPE b, DWORD c)
{ return mOriginal->SetTextureStageState(a, b, c); }

HRESULT __stdcall myIDirect3DDevice3::ValidateDevice(LPDWORD a)
{ return mOriginal->ValidateDevice(a); }
