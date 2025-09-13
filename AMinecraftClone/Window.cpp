#include "Window.h"
#include <iostream>

void Window::Init() { //initializes SDL and the window
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    m_Window = SDL_CreateWindow("Minecraft", 0, 0, 1920, 1080, SDL_WINDOW_OPENGL); //creates the window

    if (m_Window == nullptr) {
        std::cerr << "Failed to create the window" << std::endl;
        SDL_Quit();
        exit(-1);
    }

    m_Context = SDL_GL_CreateContext(m_Window);
    if (m_Context == nullptr) {
        std::cerr << "Failed to make OpenGL Context for the window" << std::endl;
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
        exit(-1);
    }

}

void Window::Termintate() { //terminates SDL as well as destroys the window
    SDL_GL_DeleteContext(m_Context);
    SDL_DestroyWindow(m_Window);
    SDL_Quit();
}