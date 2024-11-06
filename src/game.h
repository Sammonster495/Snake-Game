#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "snake.h"
#include "food.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <chrono>
#include <thread>

class Game {
public:
    Game();
    ~Game();
    void run();   // Main game loop
    bool init();  // Initialize SDL and game
    void handleEvents();  // Handle user input
    void update();  // Update game state
    void render();  // Render the game objects
    void renderScore();  // Render the score
    void renderText(const std::string &message, int x, int y, int fontSize, SDL_Color color);
    void drawGrid(int windowWidth, int windowHeight, int blockSize);
    void clean();   // Cleanup SDL

private:
    SDL_Window* window;        // SDL window
    SDL_Renderer* renderer;    // SDL renderer
    bool running, paused, gameOver;              // Game running flag
    int score, lives;

    Snake snake;  // The snake object
    Food food;    // The food object
    SDL_Texture* heartTexture;
};

#endif