#include "game.h"
#include <iostream>
#include <thread>
#include <chrono>

Game::Game() : window(nullptr), renderer(nullptr), running(true), snake(10), food(10), paused(false), gameOver(false) {
    lives = 3;
    score = 0;
}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() == -1) {  // Initialize SDL_ttf for text rendering
        std::cout << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
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

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cout << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Load heart.png as the texture for lives
    SDL_Surface* heartSurface = IMG_Load("./heart.png");
    if (!heartSurface) {
        std::cout << "Unable to load image heart.png! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
    SDL_FreeSurface(heartSurface);
    if (!heartTexture) {
        std::cout << "Unable to create texture from heart.png! SDL_Error: " << SDL_GetError() << std::endl;
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
    if (paused || gameOver) {  // Skip update if paused
        return;
    }

    snake.move();  // Move the snake
    SDL_Rect snakeHead = snake.getHead();   // Store the head in a variable
    SDL_Rect foodRect = food.getRect();     // Store the food rect in a variable

    if (SDL_HasIntersection(&snakeHead, &foodRect)) {  // Use the addresses of the variables
        snake.grow();
        score += 10;  // Increase score
        std::cout << "Score: " << score << std::endl;
        food.spawn(1200, 750);  // Respawn food
    }
    
    if (snake.checkCollision()) {
        lives--;
        if (lives == 0) {
            gameOver = true;
            return;
        } else {
            paused = true;
            snake.reset();
            food.spawn(1200, 750);
            render();  // Render to show the message
            SDL_RenderPresent(renderer);
            SDL_Delay(2000);  // Pause for the player to see the message
            paused = false;
        }
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    drawGrid(1200, 750, 10);
    for (int i = 0; i < lives; ++i) {
        SDL_Rect heartRect = {1160 - 60 * (i + 1), 15, 50, 40};
        SDL_RenderCopy(renderer, heartTexture, nullptr, &heartRect);
    }
    renderScore();
    snake.render(renderer);
    food.render(renderer);

    if (paused) {
        std::string livesLeftMessage = "You have " + std::to_string(lives) + " lives left!";
        SDL_Color white = {255, 255, 255, 255};
        renderText(livesLeftMessage, 480, 350, 30, white);
    } else if(gameOver) {
        SDL_Color red = {255, 0, 0, 255};
        renderText("Game Over", 370, 270, 100, red);
    }

    SDL_RenderPresent(renderer);
}


void Game::renderScore() {
    // Create a font object (ensure to call TTF_Init() in init() before using)
    TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", 24);  // Use a proper font path
    if (!font) {
        std::cout << "Error loading font: " << TTF_GetError() << std::endl;
        return;
    }

    // Convert the score to a string
    std::string scoreText = "Score: " + std::to_string(score);

    // Create a surface and texture for the score
    SDL_Color textColor = {255, 255, 255, 255};  // White text color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText.c_str(), textColor);
    if (!textSurface) {
        std::cout << "Error creating text surface: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_FreeSurface(textSurface);  // Free the surface after creating the texture
    if (!textTexture) {
        std::cout << "Error creating text texture: " << SDL_GetError() << std::endl;
        return;
    }

    // Set the position of the score (just below the hearts)
    SDL_Rect scoreRect = {70, 20, 200, 40};  // Position to the left of hearts

    // Render the score texture
    SDL_RenderCopy(renderer, textTexture, nullptr, &scoreRect);

    // Clean up
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void Game::drawGrid(int windowWidth, int windowHeight, int blockSize) {
    // Set grid color (light gray)
    SDL_SetRenderDrawColor(renderer, 0x30, 0x30, 0x30, 0xFF);

    // Draw vertical lines (columns)
    for (int x = 50; x < windowWidth - 49; x += blockSize) {
        SDL_RenderDrawLine(renderer, x, 70, x, windowHeight - 20);
    }

    // Draw horizontal lines (rows)
    for (int y = 70; y < windowHeight - 19; y += blockSize) {
        SDL_RenderDrawLine(renderer, 50, y, windowWidth - 50, y);
    }

    // Set border color (e.g., dark gray)
    SDL_SetRenderDrawColor(renderer, 0xFF, 0x10, 0x10, 0xFF);

    // Draw border around the grid
    SDL_RenderDrawLine(renderer, 50, 70, windowWidth - 50, 70);                // Top border
    SDL_RenderDrawLine(renderer, 50, windowHeight - 20, windowWidth - 50, windowHeight - 20);  // Bottom border
    SDL_RenderDrawLine(renderer, 50, 70, 50, windowHeight - 20);               // Left border
    SDL_RenderDrawLine(renderer, windowWidth - 50, 70, windowWidth - 50, windowHeight - 20);   // Right border
}


void Game::renderText(const std::string &message, int x, int y, int fontSize, SDL_Color color) {
    TTF_Font* font = TTF_OpenFont("Roboto-Regular.ttf", fontSize);
    if (!font) {
        std::cout << "Error loading font for message: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, message.c_str(), color);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = { x, y, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
}

void Game::clean() {
    if (heartTexture) {
        SDL_DestroyTexture(heartTexture);  // Destroy heart texture
        heartTexture = nullptr;
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
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