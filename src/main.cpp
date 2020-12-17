#include "mem.h"
#include "cpu.h"
#include "key.h"

int main (int argc, char* argv[]) {
    srand(time(NULL));

    if (argc < 2) {
        fprintf(stderr, "Must specify a rom\n");
        return -1;
    }

    SDL_Window *window;
    SDL_Renderer *renderer;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(64*10, 32*10, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_SetWindowTitle(window, std::string("Chip-8 - ").append(argv[1]).c_str());

    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,64,32);
    SDL_Event event;

    bool quit = false;

    Mem mem(argv[1]);
    Cpu cpu(&mem);

    while(!quit) {
        cpu.execute(&event);

        SDL_UpdateTexture(texture,NULL,cpu.mem->display,64*sizeof(color));
        SDL_RenderCopy(renderer,texture,NULL,NULL);
        SDL_RenderPresent(renderer);

        cpu.input(&event,&quit);

        SDL_Delay(2);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}