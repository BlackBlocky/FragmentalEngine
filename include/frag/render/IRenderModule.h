#pragma once

#include <expected>
#include <string>

#include <RenderStructs.h>

namespace frag {

    class IRenderModule {
        public:
            virtual ~IRenderModule() = default;
            virtual std::expected<void, std::string> init();
            
            virtual void swapBuffers();
            virtual void fillColor(const Color);
            virtual void fillRect(Color color, int x, int y, int w, int h);
    };

}