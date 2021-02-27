#include "emu.h"

using cl_hires = std::chrono::high_resolution_clock;

int parseArgs(int argc, char* argv[], int& scale, int& delay) {
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"--delay") == 0 || strcmp(argv[i],"-d") == 0) {
            if (argv[i+1] != nullptr)
                delay = atoi(argv[i+1]);
            else {
                fprintf(stderr, "Must specify a delay after \"--delay/-d\"\n");
                return -1;
            }
        } else if (strcmp(argv[i], "--scale") == 0 || strcmp(argv[i], "-s") == 0) {
            if (argv[i+1] != nullptr)
                scale = atoi(argv[i+1]);
            else {
                fprintf(stderr, "Must specify a scale after \"--scale/-s\"\n");
                return -1;
            }
        }
    }
    return 0;
}

int main (int argc, char* argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [options]\n\t--delay/-d: specify the delay between frames in ms\n\t--scale/-s: specify the window's scale factor\n", argv[0]);
        return -1;
    }

    int delay = 16, scale = 15;

    if(parseArgs(argc, argv, scale, delay)) { return -1; }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(64*scale, 32*scale, SDL_WINDOW_SHOWN, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    std::string title = "CHIP-8";

    SDL_SetWindowTitle(window, title.c_str());

    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,64,32);
    SDL_Event event;

    bool quit = false;

    Emu emulator;
    emulator.load(std::string(argv[1]));

    while(!quit) {
        auto start = cl_hires::now();

        while(!emulator.draw && !emulator.idle)
            emulator.execute(&event);

        emulator.idle = false;
        emulator.input(&event, quit);

        if(emulator.draw) {
            SDL_UpdateTexture(texture, NULL, emulator.display.data(), 64 * sizeof(Color));
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            emulator.draw = false;
        }

        SDL_Delay(delay);

        float ft = std::chrono::duration<float, std::milli>(cl_hires::now() - start).count();
        std::string ftstr = std::to_string(ft);
        std::string fps = std::to_string(1000/ft);
        SDL_SetWindowTitle(window, std::string(title + " | " + fps.erase(fps.find_first_of('.') + 2) +
                                            " fps | " + ftstr.erase(ftstr.find_first_of('.') + 2) + " ms").c_str());
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
