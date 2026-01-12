#pragma once

#include <string>

namespace frag {

    class Core {
        public:
            void initAndStart(std::string gameName, int pixelPerUnit, int ratioX, int ratioY, int sizeX, int sizeY);

        private:
            static bool instanceIsRunning; 

            int initGLFWWindow();
    };
    
}

