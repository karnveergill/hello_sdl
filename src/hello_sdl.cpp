#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>

///////////////////////////////////////////////////////////////////////////////
// Constants //////////////////////////////////////////////////////////////////

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int WINDOW_CENTER_X = WINDOW_WIDTH / 2;
const int WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const int PADDLE_SPEED = 5;
const int BALL_SIZE = 10;
const int BALL_SPEED = 5;

///////////////////////////////////////////////////////////////////////////////
// Function Declarations //////////////////////////////////////////////////////

// Function to handle collision with walls
void handleWallCollision(SDL_Rect& ballRect, int& xVel, int& yVel);

// Function to handle collision with paddles
void handlePaddleCollision(SDL_Rect& ballRect, 
                           const SDL_Rect& leftPaddleRect, 
                           const SDL_Rect& rightPaddleRect, 
                           int& xVel);

// Function center a rectangle on window
void centerRect(SDL_Rect& rect);

// Function to handle ball out of bounds and increment player score
void handleBallOutOfBound(SDL_Rect& ball, int& p1_score, int& p2_score);

///////////////////////////////////////////////////////////////////////////////
// Function Impelmentations ///////////////////////////////////////////////////

void handleWallCollision(SDL_Rect& ballRect, int& xVel, int& yVel) 
{
    if(ballRect.y <= 0 || ballRect.y + BALL_SIZE >= WINDOW_HEIGHT)
    {
        yVel = -yVel;
    }
}

///////////////////////////////////////////////////////////////////////////////

void handlePaddleCollision(SDL_Rect& ballRect, 
                           const SDL_Rect& leftPaddleRect, 
                           const SDL_Rect& rightPaddleRect, 
                           int& xVel) 
{
    if (SDL_HasIntersection(&ballRect, &leftPaddleRect) || 
        SDL_HasIntersection(&ballRect, &rightPaddleRect))
    {
        xVel = -xVel;
    }
}

///////////////////////////////////////////////////////////////////////////////

void centerRect(SDL_Rect& rect)
{
    rect.x = WINDOW_CENTER_X;
    rect.y = WINDOW_CENTER_Y;
}

///////////////////////////////////////////////////////////////////////////////

void handleBallOutOfBound(SDL_Rect& ball, int& p1_score, int& p2_score)
{
    if(ball.x < 0)
    {
        p1_score++;
        centerRect(ball);
    }
    else if(ball.x > WINDOW_WIDTH)
    {
        p2_score++;
        centerRect(ball);
    }
}

///////////////////////////////////////////////////////////////////////////////
// Application Entry///////////////////////////////////////////////////////////

int main() 
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL Initialization failed: %s \n", SDL_GetError());
        return 1;
    }

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Pong", 
                                          SDL_WINDOWPOS_CENTERED, 
                                          SDL_WINDOWPOS_CENTERED, 
                                          WINDOW_WIDTH, 
                                          WINDOW_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Initialize SDL_ttf for text rendering
    if(TTF_Init() == -1)
    {
        printf("Failed to initialize SDL_ttf: %s \n", TTF_GetError());
        return 1;
    }

    // Create paddles and ball
    SDL_Rect leftPaddle = {50, 
                           WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, 
                           PADDLE_WIDTH, 
                           PADDLE_HEIGHT};

    SDL_Rect rightPaddle = {WINDOW_WIDTH - 50 - PADDLE_WIDTH, 
                            WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, 
                            PADDLE_WIDTH, 
                            PADDLE_HEIGHT };

    SDL_Rect ball = {WINDOW_WIDTH / 2 - BALL_SIZE / 2, 
                     WINDOW_HEIGHT / 2 - BALL_SIZE / 2, 
                     BALL_SIZE, 
                     BALL_SIZE};

    // Set initial ball velocity
    int ballXVel = BALL_SPEED;
    int ballYVel = BALL_SPEED;

    // Set initial player scores
    int p1_score = 0;
    int p2_score = 0;

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

        // Move paddles based on key inputs
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_W] && leftPaddle.y > 0)
        {
            leftPaddle.y -= PADDLE_SPEED;
        }
        if (keystates[SDL_SCANCODE_S] && leftPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
        {
            leftPaddle.y += PADDLE_SPEED;
        }
        if (keystates[SDL_SCANCODE_UP] && rightPaddle.y > 0)
        {
            rightPaddle.y -= PADDLE_SPEED;
        }
        if (keystates[SDL_SCANCODE_DOWN] && rightPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
        {
            rightPaddle.y += PADDLE_SPEED;
        }

        // Update ball position
        ball.x += ballXVel;
        ball.y += ballYVel;

        // Check for collisions
        handleWallCollision(ball, ballXVel, ballYVel);
        handlePaddleCollision(ball, leftPaddle, rightPaddle, ballXVel);

        // Check for out of bounds
        handleBallOutOfBound(ball, p1_score, p2_score);

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw paddles and ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &ball);

        // Draw Score on screen 
        std::string p1_score_str = "P1: " + std::to_string(p1_score);
        std::string p2_score_str = "P2: " + std::to_string(p2_score);
        static const SDL_Color textColor = {255, 255, 255, 255}; 
        static TTF_Font* font = TTF_OpenFont("/Library/Fonts/Arial Unicode.ttf", 24);

        SDL_Surface* textSurface1 = TTF_RenderText_Solid(font, p1_score_str.c_str(), textColor);
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, textSurface1);
        SDL_Rect textRect1 = {10, 10, textSurface1->w, textSurface1->h};

        SDL_RenderCopy(renderer, texture1, NULL, &textRect1);
        SDL_FreeSurface(textSurface1);
        SDL_DestroyTexture(texture1);

        SDL_Surface* textSurface2 = TTF_RenderText_Solid(font, p2_score_str.c_str(), textColor);
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, textSurface2);
        SDL_Rect textRect2 = {WINDOW_WIDTH - 100, 10, textSurface1->w, textSurface1->h};

        SDL_RenderCopy(renderer, texture2, NULL, &textRect2);
        SDL_FreeSurface(textSurface2);
        SDL_DestroyTexture(texture2);

        // Update the screen
        SDL_RenderPresent(renderer);
        
        // Add a small delay to control the frame rate
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
