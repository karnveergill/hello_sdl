#ifndef PONG_H
#define PONG_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Pong
{
public:
    /*
     * @brief Constructor for pong game. Requires window and renderer for game
     *        construction
    */
    Pong(SDL_Window* window, 
         SDL_Renderer* renderer, 
         std::string explosion_asset="");
    
    /*
     * @brief Destructor
    */
    ~Pong() {}

    /*
     * @brief Initialize use of explosion graphic in game
     * @param path Path to explosion image to load
    */
    void Init_explosion(std::string path);

    // Functions to engage different game modes.  
    void Run_main_menu();
    void Run_game_1_player();
    void Run_game_2_player();
    void Run_settings_menu();
    void Run_player_pause(); 

private:
    // Functions
    /**
     * Run check on all game collisions (ceilings, paddles, out of bounds)
    */
    void check_collisions();

    /**
     * @brief Handle collision between ball & ceiling
    */
    void handle_ceiling_collision(const SDL_Rect& ball, int& y_vel);

    /**
     * @brief Handle collision between ball & paddle
    */
    void handle_paddle_collision(const SDL_Rect& ball,
                                 const SDL_Rect& paddle_1,
                                 const SDL_Rect& paddle_2,
                                 int& x_vel);

    /**
     * @brief Handle ball out of bounds
    */
    void handle_ball_out_of_bound(SDL_Rect& ball, int& p1_score, int& p2_score);

    /**
     * @brief Center a rectangle on window
    */
    void center_rect(SDL_Rect& rect);

    /**
     * @brief Update game display (ball, paddle, explosion, score)
    */
    void update_game_display(); 

    // Constants
    const int PADDLE_WIDTH = 10;
    const int PADDLE_HEIGHT = 100;
    const int PADDLE_SPEED = 10;
    const int PADDLE_1_X = 50;
    const int BALL_SIZE = 10;
    const int BALL_SPEED = 5;
    const int RESET_TIME = 500;
    const int EXPLOSION_SIZE = 40;
    const int EXPLOSION_TIME = 100;

    // Member Variables 
    /**
     * @brief Pointer to window that we will draw game on 
    */
    SDL_Window* m_window = nullptr; 

    /**
     * @brief Pointer to renderer we will use to draw game
    */
    SDL_Renderer* m_renderer = nullptr; 

    /**
     * @brief Player 1 paddle 
    */
    SDL_Rect m_paddle_1 = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};

    /**
     * @brief Player 2 paddle
    */
    SDL_Rect m_paddle_2 = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};

    /**
     * @brief Pong ball 
    */
    SDL_Rect m_ball = {0, 0, BALL_SIZE, BALL_SIZE}; 

    /**
     * @brief Ball x velocity
    */
    int m_ball_x_vel = BALL_SPEED;

    /**
     * @brief Ball Y velocity
    */
    int m_ball_y_vel = BALL_SPEED;

    /**
     * @brief Windows center x position [pixels]
    */
    int m_window_center_x = 0;

    /**
     * @brief Window center Y position [pixels]
    */
    int m_window_center_y = 0;

    /**
     * @brief Window width [pixels]
    */
    int m_window_width = 0;

    /**
     * @brief Window height [pixels]
    */
    int m_window_height = 0;

    /**
     * @brief Player 1 score 
    */
    int p1_score = 0;

    /**
     * @brief Player 2 score 
    */
    int p2_score = 0;

    /**
     * @brief Texture of explosion image
    */
    SDL_Texture* m_explosion = nullptr;

    /**
     * @brief Rectangle to describe explosion position / size
    */
    SDL_Rect m_explosion_pos = {0, 0, EXPLOSION_SIZE, EXPLOSION_SIZE};

    /**
     * @brief Start time when pausing ball on reset
    */
    uint32_t m_pause_start_t = 0;

    /**
     * @brief Start time when animating explosion
    */
    uint32_t m_explode_start_t = 0;

    /**
     * @brief Flag indicating that ball is reseting
    */
    bool m_reseting = false; 

    /**
     * @brief Flag inidicating that explosion is being shown
    */
    bool m_exploding = false; 
};

#endif // PONG_H