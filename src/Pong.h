#ifndef PONG_H
#define PONG_H

#include <SDL.h>
#include <string>

class Pong
{
public:
    /*
     * @brief Constructor for pong game. Requires window and renderer for game
     *        to displayed 
    */
    Pong(SDL_Window* window, SDL_Renderer* renderer);
    
    /*
     * @brief Destructor
    */
    ~Pong() {}

    // Functions to engage different game modes.  
    void Run_main_menu();
    void Run_game_1_player();
    void Run_game_2_player();
    void Run_settings_menu();
    void Run_player_pause(); 

private:
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
    /*
     * @brief Pointer to window that we will draw game on 
    */
    SDL_Window* m_window = nullptr; 

    /*
     * @brief Pointer to renderer we will use to draw game
    */
    SDL_Renderer* m_renderer = nullptr; 

    /*
     * @brief Player 1 paddle 
    */
    SDL_Rect m_paddle_1 = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};

    /*
     * @brief Player 2 paddle
    */
    SDL_Rect m_paddle_2 = {0, 0, PADDLE_WIDTH, PADDLE_HEIGHT};

    /*
     * @brief Pong ball 
    */
    SDL_Rect m_ball = {0, 0, BALL_SIZE, BALL_SIZE}; 

    /*
     * @brief Windows center x position [pixels]
    */
    int m_window_center_x{0};

    /*
     * @brief Window center Y position [pixels]
    */
    int m_window_center_y{0};

    /*
     * @brief Player 1 score 
    */
    int p1_score{0};

    /*
     * @brief Player 2 score 
    */
    int p2_score{0};
};

#endif // PONG_H