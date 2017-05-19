
#ifndef __BASE_HEADER__
#define __BASE_HEADER__
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <list>
#include <sstream>


#include <d3dx10.h>
#include <dxerr.h>


#if defined(_DEBUG) || defined(DEBUG)
	#ifndef D3D_DEBUG_INFO
	#define D3D_DEBUG_INFO
	#endif

	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
	
#endif

#if defined(DEBUG) | defined(_DEBUG)
#ifndef HR
#define HR(x)                                              \
	{                                                          \
		HRESULT hr = (x);                                      \
		if(FAILED(hr))                                         \
		{                                                      \
			DXTrace(__FILE__, (DWORD)__LINE__, hr, L#x, true); \
		}                                                      \
	}
#endif

#else
#ifndef HR
#define HR(x) (x)
#endif
#endif 

#define R(x) { if(x){ x->Release();x = 0; } }

//const float INFINITY = FLT_MAX;
const float PI = 3.14159265358979323f;
const float MATH_EPS = 0.0001f;

const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR RED(1.0f, 0.0f, 0.0f, 1.0f);
const D3DXCOLOR GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const D3DXCOLOR YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const D3DXCOLOR CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const D3DXCOLOR MAGENTA(1.0f, 0.0f, 1.0f, 1.0f);

const D3DXCOLOR BEACH_SAND(1.0f, 0.96f, 0.62f, 1.0f);
const D3DXCOLOR LIGHT_YELLOW_GREEN(0.48f, 0.77f, 0.46f, 1.0f);
const D3DXCOLOR DARK_YELLOW_GREEN(0.1f, 0.48f, 0.19f, 1.0f);
const D3DXCOLOR DARKBROWN(0.45f, 0.39f, 0.34f, 1.0f);


#include "Core\Timer.h"
#include "Core\BaseD3D.h"
#include "Core\SimpleD3D.h"


#endif