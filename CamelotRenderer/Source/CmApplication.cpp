#include "CmApplication.h"

#include "CmRenderSystem.h"
#include "CmRenderSystemManager.h"

#include "CmWindowEventUtilities.h"
#include "CmHardwareBufferManager.h"
#include "CmRenderWindow.h"
#include "CmCamera.h"
#include "CmViewport.h"
#include "CmVector2.h"
#include "CmHighLevelGpuProgram.h"
#include "CmHighLevelGpuProgramManager.h"
#include "CmDynLib.h"
#include "CmDynLibManager.h"
#include "CmSceneManager.h"
#include "CmImporter.h"
#include "CmResources.h"
#include "CmMesh.h"
#include "CmGameObject.h"
#include "CmTime.h"
#include "CmInput.h"
#include "CmRendererManager.h"
#include "CmRenderer.h"

#include "CmMaterial.h"
#include "CmShader.h"
#include "CmTechnique.h"
#include "CmPass.h"

#include "CmRendererManager.h"

namespace CamelotEngine
{
	Application::Application()
		:mPrimaryRenderWindow(nullptr)
	{ }

	void Application::startUp(const String& renderSystemName, const String& rendererName)
	{
		Time::startUp(new Time());
		Input::startUp(new Input());
		DynLibManager::startUp(new DynLibManager());
		HighLevelGpuProgramManager::startUp(new HighLevelGpuProgramManager());

		RenderSystemManager::startUp(renderSystemName);

		loadPlugin(rendererName);
		RendererManager::setActive("ForwardRenderer");

		RenderSystem* renderSystem = RenderSystemManager::getActive();
		renderSystem->startUp();

		mPrimaryRenderWindow = renderSystem->createRenderWindow("Camelot Renderer", 1280, 720, false);

		SceneManager::startUp(new SceneManager());
		Resources::startUp(new Resources("D:\\CamelotResourceMetas"));

		Importer::startUp(new Importer());
		loadPlugin("CamelotFreeImgImporter"); // TODO - Load this automatically somehow
		loadPlugin("CamelotFBXImporter"); // TODO - Load this automatically somehow

		loadPlugin("CamelotOISInput"); // TODO - Load this automatically somehow

		renderSystem->addPreRenderThreadUpdateCallback(boost::bind(&Application::updateMessagePump, this));
		renderSystem->addPreRenderThreadUpdateCallback(boost::bind(&Application::updateResourcesCallback, this));
	}

	void Application::runMainLoop()
	{
		while(true)
		{
			gSceneManager().update();

			RendererManager::getActive()->renderAll();

			RenderSystem* renderSystem = RenderSystemManager::getActive();
			renderSystem->update();

			gTime().update();
			gInput().update();
		}
	}

	void Application::updateMessagePump()
	{
		WindowEventUtilities::messagePump();
	}

	void Application::updateResourcesCallback()
	{
		gResources().update();
	}

	void Application::shutDown()
	{
		SceneManager::shutDown();

		if(RenderSystemManager::getActive() != nullptr)
			RenderSystemManager::getActive()->shutdown();

		HighLevelGpuProgramManager::shutDown();
		DynLibManager::shutDown();
		Resources::shutDown();
		Input::shutDown();
		Time::shutDown();
	}

	void Application::loadPlugin(const String& pluginName)
	{
		String name = pluginName;
#if CM_PLATFORM == CM_PLATFORM_LINUX
		// dlopen() does not add .so to the filename, like windows does for .dll
		if (name.substr(name.length() - 3, 3) != ".so")
			name += ".so";
#elif CM_PLATFORM == CM_PLATFORM_APPLE
		// dlopen() does not add .dylib to the filename, like windows does for .dll
		if (name.substr(name.length() - 6, 6) != ".dylib")
			name += ".dylib";
#elif CM_PLATFORM == CM_PLATFORM_WIN32
		// Although LoadLibraryEx will add .dll itself when you only specify the library name,
		// if you include a relative path then it does not. So, add it to be sure.
		if (name.substr(name.length() - 4, 4) != ".dll")
			name += ".dll";
#endif

		DynLib* library = gDynLibManager().load(name);

		if(library != nullptr)
		{
			typedef const void (*LoadPluginFunc)();

			LoadPluginFunc loadPluginFunc = (LoadPluginFunc)library->getSymbol("loadPlugin");
			loadPluginFunc();
		}
	}

	UINT64 Application::getAppWindowId()
	{
		if(!mPrimaryRenderWindow)
		{
			CM_EXCEPT(InternalErrorException, "Unable to get window handle. No active window is set!");
		}

		UINT64 windowId = 0;
		mPrimaryRenderWindow->getCustomAttribute_internal("WINDOW", &windowId);

		return windowId;
	}

	Application& gApplication()
	{
		static Application application;
		return application;
	}
}