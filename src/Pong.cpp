#include "Pong.h"
#include "Exception.h"

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

    // Set p2 score X position 
    m_score_2_rect.x = m_window_width - P2_SCORE_X_SHIFT; 
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Init_pong(const std::string& font_path, 
                     const std::string& explosion_path)
{
    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw Exception("SDL Initialization failed: %s \n", SDL_GetError());
    }

    // Initialize SDL_ttf for text rendering
    if(TTF_Init() == -1) 
    {
        throw Exception("SDL_ttf initialization failed %s\n",
                        TTF_GetError());
    }

    // Load font
    m_font = TTF_OpenFont(font_path.c_str(), 24);
    if(!m_font)
    {
        throw Exception("Failed to load font %s: %s",
                        font_path.c_str(),
                        TTF_GetError());
    }

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
}

///////////////////////////////////////////////////////////////////////////////

void Pong::Init_explosion(const std::string& path)
{
    m_explosion = IMG_LoadTexture(m_renderer, path.c_str());
    if(!m_explosion)
    {
        throw Exception("Failed to load explosion texture from %s: %s\n",
                        path.c_str(), 
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
    
    // Update dipslay graphics
    try
    {
        update_game_display();
    }
    catch(const Exception& e)
    {
        throw Exception("Failed to updated game display: %s", e.what());
    }
}

///////////////////////////////////////////////////////////////////////////////

void Pong::update_game_display()
{
    // Clear screen
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    // Draw paddles & ball
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(m_renderer, &m_paddle_1);
    SDL_RenderFillRect(m_renderer, &m_paddle_2);
    SDL_RenderFillRect(m_renderer, &m_ball);

    // Draw score
    std::string p1_score_str = "P1: " + std::to_string(m_p1_score);
    SDL_Texture* p1_texture = nullptr;
    try
    {
        p1_texture = create_score_texture(p1_score_str, m_score_1_rect);
    }
    catch(const Exception& e)
    {
        throw Exception("Failed to generate P1 score SDL_Texture: %s", e.what());
    }
    SDL_RenderCopy(m_renderer, p1_texture, NULL, &m_score_1_rect);

    // Player 2 score
    std::string p2_score_str = "P2: " + std::to_string(m_p2_score);
    SDL_Texture* p2_texture = nullptr;
    try
    {
        p2_texture = create_score_texture(p2_score_str, m_score_2_rect);
    }
    catch(const Exception& e)
    {
        throw Exception("Failed to generate P2 score SDL_Texture: %s", e.what());
    }
    SDL_RenderCopy(m_renderer, p2_texture, NULL, &m_score_2_rect);

    // Draw explosion
    if(m_exploding)
    {
        // copy explosion to renderer
        SDL_RenderCopy(m_renderer, m_explosion, NULL, &m_explosion_pos);
    }

    // Update window display
    SDL_RenderPresent(m_renderer);
}

///////////////////////////////////////////////////////////////////////////////

SDL_Texture* Pong::create_score_texture(const std::string& score, 
                                        SDL_Rect& score_rect)
{
    // Create temporary surface 
    SDL_Surface* txt_surface = TTF_RenderText_Solid(m_font, 
                                                    score.c_str(), 
                                                    m_txt_color);
    if(!txt_surface)
    {
        throw Exception("Failed to create SDL_Surface from text: %s", 
                        TTF_GetError());
    }

    // Set height/widgth of rectangle
    score_rect.w = txt_surface->w;
    score_rect.h = txt_surface->h;

    // Generate texture
    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, 
                                                        txt_surface);

    // Clean up 
    SDL_FreeSurface(txt_surface);

    return texture; 
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