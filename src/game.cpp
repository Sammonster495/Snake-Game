#include "game.h"
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(true), snake(10), food(10) {}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 750, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {
        if (event.type == SDL_QUIT)
            running = false;
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    snake.setDirection(Direction::UP);
                    break;
                case SDLK_DOWN:
                    snake.setDirection(Direction::DOWN);
                    break;
                case SDLK_LEFT:
                    snake.setDirection(Direction::LEFT);
                    break;
                case SDLK_RIGHT:
                    snake.setDirection(Direction::RIGHT);
                    break;
            }
        }
    }
}

void Game::update() {
    snake.move();  // Move the snake
    SDL_Rect snakeHead = snake.getHead();   // Store the head in a variable
    SDL_Rect foodRect = food.getRect();     // Store the food rect in a variable

    if (SDL_HasIntersection(&snakeHead, &foodRect)) {  // Use the addresses of the variables
        snake.grow();
        food.spawn(1200, 750);  // Respawn food
    }
    
    if (snake.checkCollision()) {
        running = false;  // End game on collision
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);  // Black background
    SDL_RenderClear(renderer);

    // Draw the grid
    drawGrid(1200, 750, 10);  // Window width = 1000, height = 800, block size = 10

    // Render snake and food
    snake.render(renderer);
    food.render(renderer);

    SDL_RenderPresent(renderer);  // Update the screen
}

void Game::drawGrid(int windowWidth, int windowHeight, int blockSize) {
    // Set grid color (light gray)
    SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);

    // Draw vertical lines (columns)
    for (int x = 0; x < windowWidth; x += blockSize) {
        SDL_RenderDrawLine(renderer, x, 0, x, windowHeight);
    }

    // Draw horizontal lines (rows)
    for (int y = 0; y < windowHeight; y += blockSize) {
        SDL_RenderDrawLine(renderer, 0, y, windowWidth, y);
    }
}

void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::run() {
    if (!init()) return;

    food.spawn(1200, 750);  // Spawn the first food

    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(100);  // Delay to control game speed
    }

    clean();
}