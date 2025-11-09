set(RenderingSrc
        Renderer.cpp
        Renderer.h
        CommandQueue.cpp
        CommandQueue.h
        RenderPass.cpp
        RenderPass.h
        Texture.cpp
        Texture.h
        TransferBuffer.cpp
        TransferBuffer.h
        Color.cpp
        Color.h
        Viewport.cpp
        Viewport.h
        Display.cpp
        Display.h
        Shader.cpp
        Shader.h
        Pipeline.cpp
        Pipeline.h
        DrawPass.cpp
        DrawPass.h
        CopyPass.cpp
        CopyPass.h
        Buffer.cpp
        Buffer.h
)

add_library(Rendering STATIC
        ${RenderingSrc}
)

target_link_libraries(Rendering PUBLIC Core Dependencies Platform)