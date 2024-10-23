#include "snake.h"

// Constructor to initialize the snake's direction to RIGHT
Snake::Snake(int blockSize) : blockSize(blockSize), direction(Direction::RIGHT) {
    // Initialize snake with a single block (head) at a starting position
    snakeBody.push_back({100, 100, blockSize, blockSize});
}

// Set the snake's direction
void Snake::setDirection(Direction dir) {
    // Prevent reversing the snake (no 180-degree turns)
    if ((dir == Direction::UP && direction != Direction::DOWN) ||
        (dir == Direction::DOWN && direction != Direction::UP) ||
        (dir == Direction::LEFT && direction != Direction::RIGHT) ||
        (dir == Direction::RIGHT && direction != Direction::LEFT)) {
        direction = dir;
    }
}

// Update the snake's position based on the direction
void Snake::move() {
    SDL_Rect newHead = snakeBody.front();  // Get the current head

    // Move the head in the current direction
    switch (direction) {
        case Direction::UP:
            newHead.y -= blockSize;
            break;
        case Direction::DOWN:
            newHead.y += blockSize;
            break;
        case Direction::LEFT:
            newHead.x -= blockSize;
            break;
        case Direction::RIGHT:
            newHead.x += blockSize;
            break;
    }

    // Add new head to the front of the snake
    snakeBody.push_front(newHead);

    // Remove the last block (unless the snake has just eaten food)
    if (!growing) {
        snakeBody.pop_back();  // Remove the last block to simulate movement
    } else {
        growing = false;  // Reset growing flag
    }
}

// Grow the snake (triggered when it eats food)
void Snake::grow() {
    growing = true;  // Set flag so the snake doesn't remove its tail in the next move
}

void Snake::render(SDL_Renderer* renderer) {
    // Draw the border around the entire snake body

    // Set the color for the snake body (green)
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);

    // Draw each block of the snake's body except the head
    for (size_t i = 1; i < snakeBody.size(); ++i) {
        SDL_RenderFillRect(renderer, &snakeBody[i]);
    }

    // Draw the head separately with a larger size
    SDL_Rect head = snakeBody.front();  // Get the head
    SDL_Rect enlargedHead = { head.x - blockSize / 4, head.y - blockSize / 4, head.w + blockSize / 2, head.h + blockSize / 2 };

    // Set the color for the snake head (light green)
    SDL_SetRenderDrawColor(renderer, 0x80, 0xFF, 0x80, 0xFF);  // A different shade for the head
    SDL_RenderFillRect(renderer, &enlargedHead);
}

bool Snake::checkCollision() {
    SDL_Rect head = getHead();

    // Check if the snake's head hits its own body
    for (size_t i = 1; i < snakeBody.size(); ++i) {
        if (SDL_HasIntersection(&head, &snakeBody[i])) {
            return true;  // Collision detected
        }
    }

    // Check if the snake hits the window boundaries (assume a window size of 1400x1000)
    if (head.x < 0 || head.y < 0 || head.x >= 1200 || head.y >= 750) {
        return true;  // Snake hit a boundary
    }

    return false;  // No collision
}