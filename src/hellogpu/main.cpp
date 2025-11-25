#ifdef __clang__
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wmicrosoft-redeclare-static"
#endif

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>

#ifdef __clang__
#    pragma clang diagnostic pop
#endif

#include <iostream>
#include <new>

SDL_AppResult SDL_AppInit([[maybe_unused]] void** appstate, [[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    SDL_InitFlags initFlags = SDL_INIT_EVENTS | SDL_INIT_VIDEO;
    if(!SDL_Init(initFlags))
    {
        return SDL_APP_FAILURE;
    }

    SDL_DisplayID primaryDisplayId = SDL_GetPrimaryDisplay();

    if(primaryDisplayId == 0)
    {
        return SDL_APP_FAILURE;
    }

    SDL_Rect primaryDisplayBounds;
    if(!SDL_GetDisplayBounds(primaryDisplayId, &primaryDisplayBounds))
    {
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow("Test", primaryDisplayBounds.w * 3 / 4, primaryDisplayBounds.h * 3 / 4, 0);

    if(window == nullptr)
    {
        std::cout << SDL_GetError();
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate([[maybe_unused]] void* appstate)
{
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent([[maybe_unused]] void* appstate, SDL_Event* event)
{
    switch(event->type)
    {
    case SDL_EVENT_QUIT:                   return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: return SDL_APP_SUCCESS;
    case SDL_EVENT_WINDOW_DESTROYED:       return SDL_APP_SUCCESS;
    default:                               return SDL_APP_CONTINUE;
    }
}

void SDL_AppQuit([[maybe_unused]] void* appstate, [[maybe_unused]] SDL_AppResult result) {}