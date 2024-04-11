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
    // Get window width & height to derive paddle & ball positions
    int width, height; 
    SDL_GetWindowSize(window, &width, &height);
    m_window_width = width;
    m_window_height = height;
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

void Pong::Init_pong(const std::string& explosion_path)
{
    // If we got an explosion asset load it
    if(!explosion_path.empty())
    {
        try
        {
            Init_explosion(explosion_path);
        }
        catch(const std::exception& e)
        {
            throw;
        }
    }

    // Initialize SDL_ttf for text rendering
    if(TTF_Init() == -1) 
    {
        throw Exception("SDL_ttf initialization failed %s\n",
                        TTF_GetError());
    }
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Init_explosion(const std::string& path)
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

    // Update paddles based on key inputs
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

    // Update ball 
    update_ball();
    
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

void Pong::update_ball()
{
    // Check out of bounds 
    if(handle_ball_out_of_bound(m_ball, m_p1_score, m_p2_score))
    {
        // Set start times and flags
        m_pause_start_t = SDL_GetTicks();
        m_explode_start_t = SDL_GetTicks(); 
        m_reseting = true;
        m_exploding = true;
    }

    // Check timers
    uint32_t now = SDL_GetTicks();
    if(now - m_pause_start_t > RESET_TIME)
    {
        m_reseting = false;
    }
    if(now - m_explode_start_t > EXPLOSION_TIME)
    {
        m_exploding = false; 
    }

    // Update ball position
    if(!m_reseting)
    {
        m_ball.x += m_ball_x_vel;
        m_ball.y += m_ball_y_vel;
    }

    // Check ceiling and paddle collisions
    handle_ceiling_collision(m_ball, m_ball_y_vel);
    handle_paddle_collision(m_ball, m_paddle_1, m_paddle_2, m_ball_x_vel);
}

///////////////////////////////////////////////////////////////////////////////

void Pong::handle_ceiling_collision(const SDL_Rect& ball, int& y_vel)
{
    if(ball.y <= 0 || ball.y + BALL_SIZE >= m_window_height)
    {
        y_vel *= -1; 
    }
}

///////////////////////////////////////////////////////////////////////////////

void Pong::handle_paddle_collision(const SDL_Rect& ball,
                                   const SDL_Rect& paddle_1,
                                   const SDL_Rect& paddle_2,
                                   int& x_vel)
{
    if(SDL_HasIntersection(&ball, &paddle_1) || 
       SDL_HasIntersection(&ball, &paddle_2))
    {
        x_vel *= -1; 
    }
}

///////////////////////////////////////////////////////////////////////////////

bool Pong::handle_ball_out_of_bound(SDL_Rect& ball, 
                                    int& p1_score, 
                                    int& p2_score)
{
    bool reset = false;
    int shift = 0;
    if(ball.x > m_window_width)
    {
        reset = true;
        p1_score++;
        shift = -1;
    }
    else if(ball.x < 0)
    {
        reset = true;
        p2_score++;
    }

    if(reset)
    {
        // Save position
        m_explosion_pos.x = ball.x+(shift*EXPLOSION_SIZE);
        m_explosion_pos.y = ball.y; 

        // Reset ball
        center_rect(ball);
    }

    return reset; 
}

///////////////////////////////////////////////////////////////////////////////

void Pong::center_rect(SDL_Rect& rect)
{
    rect.x = m_window_center_x;
    rect.y = m_window_center_y;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////