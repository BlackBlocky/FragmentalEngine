#pragma once

#include <IRenderModule.h>

namespace frag {

    class Renderer {
        public:
            void setRenderModule(const IRenderModule* module);
    };

}