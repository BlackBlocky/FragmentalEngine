#include <frag/Core.h>

#include <iostream>

#include <frag/Log.h>

#include <GLFW/glfw3.h>

namespace frag {
    
    void Core::initAndStart(std::string gameName, int pixelPerUnit, int ratioX, int ratioY, int sizeX, int sizeY) {
        if (instanceIsRunning) {

            return;
        }

        instanceIsRunning = true;
    }

    int Core::initGLFWWindow() {
        return 0;
    }
    
}

