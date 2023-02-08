// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl_opengl3/ folder**
// See imgui_impl_sdl.cpp for details.

//#define TEST_TOM_P


#include "UIManager.h"
#include "Odometry.hpp"

#include <SDL.h>
#include <SDL_opengl.h>

static bool done = false;

void handleEvents();

// Main code
int main(int, char**)
{
    Odometry* odometry = new Odometry();
    Inspector* inspector = new Inspector(odometry);
    UIManager* uiManager = new UIManager(inspector);

#ifndef TEST_TOM_P
    // Main loop
    while (!done)
    {
        handleEvents();
        
        odometry->update();
        uiManager->render();
        usleep(10000);
    }
#endif

    uiManager->cleanUp();
    return 0;
}

void handleEvents() {
    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    SDL_Event event;
    SDL_Window* window = SDL_GL_GetCurrentWindow();
    while (SDL_PollEvent(&event))
    {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            done = true;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            done = true;
        
    }
}
