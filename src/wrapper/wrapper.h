#ifndef WRAPPER_H_INCLUDED
#define WRAPPER_H_INCLUDED

#include <Windows.h>

#include "myIDirectDraw.h"
#include "myIDirectDraw4.h"
#include "myIDirect3D3.h"
#include "myIDirect3DDevice3.h"
#include "../util.h"

// Global critical section, defined in main.cpp
extern CRITICAL_SECTION gl_CS;

// Global references to some important Direct3D interfaces (for easy access)
extern myIDirect3DDevice3* gl_pmyIDirect3DDevice3;
extern myIDirect3D3*       gl_pmyIDirect3D3;
extern myIDirectDraw*      gl_pmyIDirectDraw;

// Create/delete a wrapper object around a certain interface
void* wrapItem(REFIID riid, void* ppvObj);
void  deleteWrappedItem(void* ppvObj);

#endif // WRAPPER_H_INCLUDED
