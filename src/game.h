#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "snake.h"
#include "food.h"

class Game {
public:
    Game();
    ~Game();
    void run();   // Main game loop
    bool init();  // Initialize SDL and game
    void handleEvents();  // Handle user input
    void update();  // Update game state
    void render();  // Render the game objects
    void drawGrid(int windowWidth, int windowHeight, int blockSize);
    void clean();   // Cleanup SDL

private:
    SDL_Window* window;        // SDL window
    SDL_Renderer* renderer;    // SDL renderer
    bool running;              // Game running flag

    Snake snake;  // The snake object
    Food food;    // The food object
};

#endif