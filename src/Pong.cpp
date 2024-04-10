#include "Pong.h"
#include "Exception.h"
#include <SDL_ttf.h>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Pong::Pong(SDL_Window* window,
           SDL_Renderer* renderer)
    :m_window(window),
     m_renderer(renderer)
{
    // Initialize SDL_ttf for text rendering
    if(TTF_Init() == -1) 
    {
        throw Exception("SDL_ttf initialization failed %s\n",
                        TTF_GetError());
    }

    // Get window width & height to derive paddle/ball positions
    int window_w, window_h; 
    SDL_GetWindowSize(window, &window_w, &window_h);
    m_window_center_x = window_w / 2;
    m_window_center_y = window_h / 2;

    // Set initial paddle & ball positions 
    m_paddle_1.x = PADDLE_1_X;
    m_paddle_1.y = window_h/2 - PADDLE_HEIGHT/2;

    m_paddle_2.x = window_w - PADDLE_1_X - PADDLE_WIDTH;
    m_paddle_2.y = window_h/2 - PADDLE_HEIGHT/2;

    m_ball.x = m_window_center_x;
    m_ball.y = m_window_center_y;
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Run_game_2_player()
{
    // This should call all the stuff that draws the 2 player game on the screen
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////