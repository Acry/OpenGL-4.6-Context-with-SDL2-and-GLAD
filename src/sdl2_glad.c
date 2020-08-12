#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#include "glad/glad.h"
#include "helper.h"

#define WW 550
#define WH (WW/16)*9
#define GL_GREY .5, .5, .5, 1
int ww=WW;
int wh=WH;

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    #pragma region setup sdl
    init();

    SDL_SetWindowPosition(Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    SDL_SetWindowSize(Window, ww, wh);
    SDL_SetWindowTitle(Window, "Context 4.6 with GLAD");
    SDL_ShowWindow(Window);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #pragma endregion  setup sdl

    gladLoadGLLoader(SDL_GL_GetProcAddress);

    SDL_Log("Vendor   : %s", glGetString(GL_VENDOR));
    SDL_Log("Renderer : %s", glGetString(GL_RENDERER));
    SDL_Log("Version  : %s", glGetString(GL_VERSION));
    SDL_Log("GLSL     : %s", glGetString(GL_SHADING_LANGUAGE_VERSION));
    int maj;
    int min;
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &maj);
    SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &min);
    SDL_Log("Context  : %d.%d", maj, min);

    glGetIntegerv(GL_MAJOR_VERSION, &maj);
    glGetIntegerv(GL_MINOR_VERSION, &min);
    SDL_Log("Context  : %d.%d", maj, min);


    glClearColor(GL_GREY);
    SDL_Event event;
    int running = 1;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running =0;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    running =0;
                    break;
                default:
                    break;
                }
            }
            if (event.type == SDL_WINDOWEVENT) {
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                    glViewport(0, 0, event.window.data1, event.window.data2);
                }
            }
        }
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        SDL_GL_SwapWindow(Window);
    }
    exit_();
    return EXIT_SUCCESS;
}