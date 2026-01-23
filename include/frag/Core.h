#pragma once

#include <string>

namespace frag {

    enum class FrameMode {
        FIXED,
        VARIABLE
    };

    class Core {
        public:
            void initAndStart(std::string gameName, int pixelPerUnit, int ratioX, int ratioY, int windowWith);

            // Setters

            // Use targetFPS < 0 for unlimited FPS. e.g.: -1
            // FIXED mode requires targetFPS > 0.
            void setFrameMode(FrameMode mode, int targetFPS);

        private:
            // -- Status variables --          
            bool windowIsInitialized = false;

            // -- General Information --
            std::string gameName;
            FrameMode frameMode = FrameMode::VARIABLE;
            int targetFPS = 60;

            int initGLFWWindow();
            void enterGameLoop();
    };
    
}

