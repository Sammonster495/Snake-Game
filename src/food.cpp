#include "food.h"
#include <cstdlib>
#include <ctime>

Food::Food(int blockSize) : blockSize(blockSize) {
    srand(static_cast<unsigned>(time(0)));  // Seed for random number generation
    rect = { 0, 0, blockSize, blockSize };
}

void Food::spawn(int screenWidth, int screenHeight) {
    // Define grid constraints matching `drawGrid` (50-pixel margin on all sides)
    int xMin = 50 / blockSize;
    int xMax = (screenWidth - 50) / blockSize - 1;
    rect.x = (rand() % (xMax - xMin + 1) + xMin) * blockSize;

    int yMin = 70 / blockSize;
    int yMax = (screenHeight - 20) / blockSize - 1;
    rect.y = (rand() % (yMax - yMin + 1) + yMin) * blockSize;
}


void Food::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);  // Red color for food
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Food::getRect() {
    return rect;
}