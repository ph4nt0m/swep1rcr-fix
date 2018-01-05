#include "wrapper.h"
#include <map>
#include <string>

myIDirect3DDevice3* gl_pmyIDirect3DDevice3 = nullptr;
myIDirect3D3*       gl_pmyIDirect3D3       = nullptr;
myIDirectDraw*      gl_pmyIDirectDraw      = nullptr;

static std::map<void*, void*> wrappedItems;

std::string getStringFromIID(const IID& iid)
{
  if (iid == IID_IDirectDraw)
    return "IDirectDraw";
  else if (iid == IID_IDirectDraw4)
    return"IDirectDraw4";
  else if (iid == IID_IDirect3D3)
    return "IDirect3D3";
  else if (iid == IID_IDirect3DDevice3)
    return "IDirect3DDevice3";
  else
    return "";
}

void* wrapItem(REFIID riid, void* ppvObj)
{
  // Check if this object has already been wrapped earlier
  auto it = wrappedItems.find(ppvObj);
  if (it != wrappedItems.end())
    return it->second;

  // Otherwise, create a new wrapper if possible
  void* wrapper = nullptr;

  if (riid == IID_IDirectDraw)
    wrapper = new myIDirectDraw(static_cast<IDirectDraw*>(ppvObj));
  else if (riid == IID_IDirectDraw4)
    wrapper = new myIDirectDraw4(static_cast<IDirectDraw4*>(ppvObj));
  else if (riid == IID_IDirect3D3)
    wrapper = new myIDirect3D3(static_cast<IDirect3D3*>(ppvObj));
  else if (riid == IID_IDirect3DDevice3)
    wrapper = new myIDirect3DDevice3(static_cast<IDirect3DDevice3*>(ppvObj));

  // If successful, store and return wrapper object
  if (wrapper)
  {
    wrappedItems[ppvObj] = wrapper;
    writeLog("Successfully wrapped interface object %s (0x%08x)",
      getStringFromIID(riid).c_str(), ppvObj);
    return wrapper;
  }
  else
  {
    writeLog("Unknown interface has not been wrapped (0x%08x)", ppvObj);
    return ppvObj;
  }
}

void deleteWrappedItem(void* ppvObj)
{
  wrappedItems.erase(ppvObj);
  writeLog("Wrapper object has been deleted (0x%08x)", ppvObj);
}
