#include <iostream>
#include <string>
#include <SDL.h>
#include "version.h"
#include "Pong.h"
#include "Exception.h"

///////////////////////////////////////////////////////////////////////////////
// Constants //////////////////////////////////////////////////////////////////

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

///////////////////////////////////////////////////////////////////////////////
// Application Entry///////////////////////////////////////////////////////////

int main(int argc, char* argv[]) 
{
    // Set application version 
    static const std::string app_version = "v" 
                                           + std::to_string(PROJECT_VERSION_MAJOR) + "." 
                                           + std::to_string(PROJECT_VERSION_MINOR) + "." 
                                           + std::to_string(PROJECT_VERSION_PATCH); 

    // Handle application arguments 
    for(int i = 1; i < argc; i++)
    {
        // Print version and leave application
        if(std::string(argv[i]) == "--version" || std::string(argv[i]) == "-v")
        {
            printf("hello_sdl - pong : %s\n", app_version.c_str());
            return 0; 
        }
    }

    // Initialize SDL 
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Initialization failed: %s \n", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    static const std::string window_title = "Pong " + app_version;
    SDL_Window* window = SDL_CreateWindow(window_title.c_str(), 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, 
                                          WINDOW_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

        
    static const int USE_FIRST_AVAILABLE_RENDERING_DRIVER = -1;
    SDL_Renderer* renderer = SDL_CreateRenderer(window, 
                                                USE_FIRST_AVAILABLE_RENDERING_DRIVER,
                                                SDL_RENDERER_ACCELERATED);

    Pong pong_game(window, renderer);
    try
    {
        pong_game.Init_pong("resources/arial_unicode.ttf",
                            "resources/spark_flame.png");
    }
    catch(const Exception& e)
    {
        printf("Failed to initialize Pong game: %s", e.what());
        return 1;
    }

    // Run game 
    bool running = true;
    while (running) 
    {
        // Check for exit
        SDL_Event event;
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        try
        {
            pong_game.Run_game_2_player();
        }
        catch(const std::exception& e)
        {
            printf("Error Occured while running pong 2 player: %s",
                   e.what());
        }

        // Add a small delay to control the frame rate
        SDL_Delay(10);
    }

    // Destroy SDL assets
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
