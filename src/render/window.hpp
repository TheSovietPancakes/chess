#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

class RenderWindow {
    public:
    RenderWindow(const char *title, int width, int height) {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            std::cerr << "Window failed to init. SDL error:" << std::endl;
            std::cerr << SDL_GetError() << std::endl;
            exit(-1);
        }
        // Woohoo, GPU acceleration baby!!
        // -1 for "find the default driver I dont want to"
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    }

    SDL_Texture *loadSVG(const char* path) {
        SDL_Surface *surface = IMG_Load(path);
        if (surface == NULL) {
            std::cerr << "Failed to load image: " << path << std::endl;
            std::cerr << IMG_GetError() << std::endl;
            exit(-1);
        }
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            std::cerr << "Failed to create texture from surface" << std::endl;
            std::cerr << SDL_GetError() << std::endl;
            exit(-1);
        }
        SDL_FreeSurface(surface);
        return texture;
    }

    void render() {
        SDL_RenderPresent(renderer);
    }

    void renderTex(SDL_Texture *texture, SDL_Rect *pos) {
        SDL_RenderCopy(renderer, texture, NULL, pos);
    }

    int getFPS() {
        SDL_DisplayMode mode;
        SDL_GetWindowDisplayMode(window, &mode);
        return mode.refresh_rate;
    }

    void highlightSquare(int square) {
        const int squareSize = 100;
        int x = square % 8;
        int y = square / 8;
        SDL_Rect rect = {x * squareSize, y * squareSize, squareSize, squareSize};
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x99);
        SDL_RenderFillRect(renderer, &rect);
    }

    void drawGrid() {
        // Draw a chess board background of black and white squares
        // blackCol = #a3855b;
        // whiteCol = #fad8a7;
        int squareSize = 100;
        for (int i = 0; i < 64; i++) {
            int x = i % 8;
            int y = i / 8;
            SDL_Rect rect = {x * squareSize, y * squareSize, squareSize, squareSize};
            if ((x + y) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 0xfa, 0xd8, 0xa7, 0xff);
            } else {
                SDL_SetRenderDrawColor(renderer, 0xa3, 0x85, 0x5b, 0xff);
            }
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    void cleanup() {
        SDL_DestroyWindow(window);
    }

    ~RenderWindow() {
        cleanup();
    }

    private:

    SDL_Window *window;
    SDL_Renderer *renderer;
};