#pragma once

#include "CmPrerequisites.h"

// some D3D commonly used macros
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

// enable extended d3d debug 
#if CM_DEBUG_MODE
#	define D3D_DEBUG_INFO
#endif

#define WIN32_LEAN_AND_MEAN
#if !defined(NOMINMAX) && defined(_MSC_VER)
#	define NOMINMAX // required to stop windows.h messing up std::min
#endif

#include <d3d11.h>
#include <d3d11shader.h>
#include <D3Dcompiler.h>

namespace CamelotEngine
{
	// Predefine classes
	class D3D11RenderSystem;
	class D3D11RenderWindowBase;
	class D3D11Texture;
	class D3D11TextureManager;
	class D3D11DepthBuffer;
	class D3D11Driver;
	class D3D11DriverList;
	class D3D11VideoMode;
	class D3D11VideoModeList;
	class D3D11GpuProgram;
	class D3D11GpuProgramManager;
	class D3D11HardwareBufferManager;
	class D3D11HardwareIndexBuffer;
	class D3D11HLSLProgramFactory;
	class D3D11HLSLProgram;
	class D3D11VertexDeclaration;
	class D3D11Device;
	class D3D11HardwareBuffer;
	class D3D11HardwarePixelBuffer;

	enum TypeID_D3D9
	{
		TID_D3D9_HLSLProgram = 12000
	};

// Should we ask D3D to manage vertex/index buffers automatically?
// Doing so avoids lost devices, but also has a performance impact
// which is unacceptably bad when using very large buffers
#define CM_D3D_MANAGE_BUFFERS 1 // TODO - Keep this on or off? I'll probably want to turn it off at some point

    //-------------------------------------------
	// Windows setttings
	//-------------------------------------------
#if (CM_PLATFORM == CM_PLATFORM_WIN32) && !defined(CM_STATIC_LIB)
#	ifdef CM_RSD3D11_EXPORTS
#		define CM_D3D11_EXPORT __declspec(dllexport)
#	else
#       if defined( __MINGW32__ )
#           define CM_D3D11_EXPORT
#       else
#    		define CM_D3D11_EXPORT __declspec(dllimport)
#       endif
#	endif
#else
#	define CM_D3D11_EXPORT
#endif	// CM_PLATFORM_WIN32
}