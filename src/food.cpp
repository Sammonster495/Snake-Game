#include "food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int blockSize) : blockSize(blockSize) {
    srand(static_cast<unsigned>(time(0)));  // Seed for random number generation
    rect = { 0, 0, blockSize, blockSize };
}

void Food::spawn(int screenWidth, int screenHeight) {
    rect.x = (rand() % (screenWidth / blockSize)) * blockSize;
    rect.y = (rand() % (screenHeight / blockSize)) * blockSize;
}

void Food::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red color for food
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Food::getRect() {
    return rect;
}