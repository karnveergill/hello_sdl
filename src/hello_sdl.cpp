// #include <stdio.h>
// #include <string>
// #include <SDL.h>

// int main()
// {
//     int var1 = 69;
//     std::string text = "This debugger is lame";
//     printf("Hello SDL!\n");

//     printf("Hello again turd\n");

//     return 0;
// }

#include <iostream>
#include <SDL.h>

// Constants
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const int PADDLE_SPEED = 5;
const int BALL_SIZE = 10;
const int BALL_SPEED = 5;

// Function to handle collision with walls
void handleWallCollision(SDL_Rect& ballRect, int& xVel, int& yVel) {
    if (ballRect.y <= 0 || ballRect.y + BALL_SIZE >= WINDOW_HEIGHT)
        yVel = -yVel;
}

// Function to handle collision with paddles
void handlePaddleCollision(SDL_Rect& ballRect, const SDL_Rect& leftPaddleRect, const SDL_Rect& rightPaddleRect, int& xVel) {
    if (SDL_HasIntersection(&ballRect, &leftPaddleRect) || SDL_HasIntersection(&ballRect, &rightPaddleRect))
        xVel = -xVel;
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Create paddles and ball
    SDL_Rect leftPaddle = { 50, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect rightPaddle = { WINDOW_WIDTH - 50 - PADDLE_WIDTH, WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
    SDL_Rect ball = { WINDOW_WIDTH / 2 - BALL_SIZE / 2, WINDOW_HEIGHT / 2 - BALL_SIZE / 2, BALL_SIZE, BALL_SIZE };

    // Set initial ball velocity
    int ballXVel = BALL_SPEED;
    int ballYVel = BALL_SPEED;

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_W] && leftPaddle.y > 0)
            leftPaddle.y -= PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_S] && leftPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            leftPaddle.y += PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_UP] && rightPaddle.y > 0)
            rightPaddle.y -= PADDLE_SPEED;
        if (keystates[SDL_SCANCODE_DOWN] && rightPaddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT)
            rightPaddle.y += PADDLE_SPEED;

        // Update ball position
        ball.x += ballXVel;
        ball.y += ballYVel;

        // Check for collisions
        handleWallCollision(ball, ballXVel, ballYVel);
        handlePaddleCollision(ball, leftPaddle, rightPaddle, ballXVel);

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw paddles and ball
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &leftPaddle);
        SDL_RenderFillRect(renderer, &rightPaddle);
        SDL_RenderFillRect(renderer, &ball);

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
