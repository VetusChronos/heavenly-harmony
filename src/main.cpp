#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLAYER_SPEED = 4;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gPlayerTexture = nullptr;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Error initializing SDL! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gWindow = SDL_CreateWindow("Movimentação 8 Direções", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        std::cerr << "Error creating window! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        std::cerr << "Error creating the renderer! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        std::cerr << "Error initializing SDL_image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    SDL_Surface* loadedSurface = IMG_Load("assets/player.png");
    if (loadedSurface == nullptr) {
        std::cerr << "Error loading image! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    gPlayerTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (gPlayerTexture == nullptr) {
        std::cerr << "Error creating texture! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void close() {
    SDL_DestroyTexture(gPlayerTexture);
    gPlayerTexture = nullptr;

    SDL_DestroyRenderer(gRenderer);
    gRenderer = nullptr;

    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main() {
    if (!init()) {
        std::cerr << "Initialization failed!" << std::endl;
        return -1;
    }

    if (!loadMedia()) {
        std::cerr << "Media loading failed!" << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    int x = SCREEN_WIDTH / 2;
    int y = SCREEN_HEIGHT / 2;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        int xMove = 0;
        int yMove = 0;

        if (currentKeyStates[SDL_SCANCODE_W]) {
            yMove -= 1;
        }

        if (currentKeyStates[SDL_SCANCODE_S]) {
            yMove += 1;
        }

        if (currentKeyStates[SDL_SCANCODE_A]) {
            xMove -= 1;
        }

        if (currentKeyStates[SDL_SCANCODE_D]) {
            xMove += 1;
        }

        if (xMove != 0 && yMove != 0) {
            xMove = static_cast<int>(std::round(xMove / std::sqrt(2.0)));
            yMove = static_cast<int>(std::round(yMove / std::sqrt(2.0)));
        }

        if (x + xMove >= 0 && x + xMove <= SCREEN_WIDTH - 32) {
            x += xMove * PLAYER_SPEED;
        }

        if (y + yMove >= 0 && y + yMove <= SCREEN_HEIGHT - 32) {
            y += yMove * PLAYER_SPEED;
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect playerRect = {x, y, 32, 32};
        SDL_RenderCopy(gRenderer, gPlayerTexture, nullptr, &playerRect);

        SDL_RenderPresent(gRenderer);

        std::cout << "Move X: " << xMove << " | Move Y: " << yMove << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    close();

    return 0;
}
