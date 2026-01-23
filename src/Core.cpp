#include <frag/Core.h>

#include <iostream>

#include <frag/Log.h>

#include <GLFW/glfw3.h>

namespace frag {
    
    // ------------------------------------
    // -- Public Methods Implementation --
    // ------------------------------------

    static bool instanceIsRunning = false; 
    void Core::initAndStart(std::string gameName, int pixelPerUnit, int ratioX, int ratioY, int windowWith) {
        logInfo("Starting Core instance for game '{}'", gameName);

        if (instanceIsRunning) {
            logError("Core instance is already running, cannot start another instance.");
            return;
        }

        this->gameName = gameName;

        initGLFWWindow();
        enterGameLoop();

        instanceIsRunning = true;
    }

    void Core::setFrameMode(FrameMode mode, int targetFPS) {
        logDebug("Setting frame mode...");

        if (mode == FrameMode::FIXED && targetFPS <= 0) {
            logError("Cannot set FIXED frame mode with non-positive targetFPS ({}).", targetFPS);
            return;
        }

        this->frameMode = mode;
        this->targetFPS = targetFPS;
    }



    // -------------------------------------
    // -- Private Methods Implementation --
    // -------------------------------------

    static GLFWwindow* window;
    int Core::initGLFWWindow() {
        logDebug("Initializing GLFW window...");

         /* Initialize the library */
        if (!glfwInit()) {
            logFatal("Failed to initialize GLFW.");
            return -1;
        }

         /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, gameName.c_str(), NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            logFatal("Failed to create GLFW window.");
            return -1;
        }

         /* Make the window's context current */
        glfwMakeContextCurrent(window);

        windowIsInitialized = true;
        return 0;
    }

    void Core::enterGameLoop() {
        logDebug("Entering game loop...");

        if (!windowIsInitialized) {
            logFatal("Window is not initialized, cannot enter game loop.");
            return;
        }

        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            //glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
            
        }
        glfwTerminate();

        logDebug("Exited game loop.");
    }
    
}

