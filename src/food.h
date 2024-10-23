#ifndef FOOD_H
#define FOOD_H

#include <SDL.h>

class Food {
public:
    Food(int blockSize);
    void spawn(int screenWidth, int screenHeight);  // Spawn food at random location
    void render(SDL_Renderer* renderer);  // Render the food
    SDL_Rect getRect();  // Get the food's location

private:
    SDL_Rect rect;  // Food's rectangle
    int blockSize;  // Size of the food block
};

#endif