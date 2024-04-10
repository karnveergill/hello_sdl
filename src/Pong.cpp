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

    // Get window width & height to derive paddle & ball positions
    int m_window_width, m_window_height; 
    SDL_GetWindowSize(window, &m_window_width, &m_window_height);
    m_window_center_x = m_window_width / 2;
    m_window_center_y = m_window_height / 2;

    // Set initial paddle & ball positions 
    m_paddle_1.x = PADDLE_1_X;
    m_paddle_1.y = m_window_height/2 - PADDLE_HEIGHT/2;

    m_paddle_2.x = m_window_width - PADDLE_1_X - PADDLE_WIDTH;
    m_paddle_2.y = m_window_height/2 - PADDLE_HEIGHT/2;

    m_ball.x = m_window_center_x;
    m_ball.y = m_window_center_y;
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Init_explosion(std::string path)
{
    SDL_Texture* m_explosion = IMG_LoadTexture(m_renderer, path.c_str());
    if(!m_explosion)
    {
        throw Exception("Failed to load explosion texture: %s", 
                        IMG_GetError());
    }
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Run_game_2_player()
{
    // This should call all the stuff that draws the 2 player game on the screen

    // Move paddles based on key inputs
    const Uint8* keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_W] && m_paddle_1.y > 0)
    {
        m_paddle_1.y -= PADDLE_SPEED;
    }
    if (keystates[SDL_SCANCODE_S] && m_paddle_1.y + PADDLE_HEIGHT < m_window_height)
    {
        m_paddle_1.y += PADDLE_SPEED;
    }
    if (keystates[SDL_SCANCODE_UP] && m_paddle_2.y > 0)
    {
        m_paddle_2.y -= PADDLE_SPEED;
    }
    if (keystates[SDL_SCANCODE_DOWN] && m_paddle_2.y + PADDLE_HEIGHT < m_window_height)
    {
        m_paddle_2.y += PADDLE_SPEED;
    }

    // Clear the screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Draw paddles and ball
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(m_renderer, &m_paddle_1);
    SDL_RenderFillRect(m_renderer, &m_paddle_2);
    SDL_RenderFillRect(m_renderer, &m_ball);

    // Update window display
    SDL_RenderPresent(m_renderer);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////