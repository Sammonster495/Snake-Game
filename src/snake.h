#ifndef SNAKE_H
#define SNAKE_H

#include <SDL.h>
#include <vector>
#include <deque>

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake {
public:
    Snake(int blockSize);
    void move();  // Update snake position
    void grow();  // Grow the snake when it eats food
    void render(SDL_Renderer* renderer);  // Render the snake
    void setDirection(Direction dir);  // Set the snake's movement direction
    bool checkCollision();  // Check if snake has collided with itself or boundaries
    SDL_Rect getHead() const { return snakeBody.front(); };  // Get the snake's head for collision checks

private:
    std::deque<SDL_Rect> snakeBody;;  // Snake's body segments
    int blockSize;  // Size of each block
    Direction direction;  // Current movement direction
    bool growing;  // Flag to indicate if the snake is growing
};

#endif