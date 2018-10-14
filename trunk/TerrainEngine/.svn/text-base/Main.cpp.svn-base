#include "TerrainEngine.h"

//#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"



//#else
//int main(int argc, char **argv)
//#endif


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, int)
{
	try
    {
		TerrainEngine engine;
		engine.start();
    }
	catch(std::exception& e)
    {
#if OGRE_PLATFORM == PLATFORM_WIN32 || OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        //MessageBoxW(NULL, e.what(), "An exception has occurred!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        fprintf(stderr, "An exception has occurred: %s\n", e.what());
#endif
    }

    return 0;
}