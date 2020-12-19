#include "mem.h"
#include "cpu.h"

int main (int argc, char* argv[]) {
    srand(time(NULL));
    
    if (argc < 2) {
        fprintf(stderr, "Must specify a rom\n");
        return -1;
    }

    int delay = 1;

    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"--delay") == 0 || strcmp(argv[i],"-d") == 0) {
            if (argv[i+1] != nullptr)
                delay = atoi(argv[i+1]);
            else {
                fprintf(stderr, "Must specify a delay after \"--delay/-d\"\n");
                return -1;
            }
        }
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

    SDL_SetWindowTitle(window, std::string("Chip-8 - ")
                               .append(std::string(argv[1]).erase(
                                   0, std::string(argv[1]).find_last_of('/') + 1
                               ))
                               .c_str());

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

        SDL_Delay(delay);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}