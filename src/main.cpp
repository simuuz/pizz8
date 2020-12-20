#include "mem.h"
#include "cpu.h"

using cl_hires = std::chrono::high_resolution_clock;

int parseArgs(int argc, char* argv[], int* scale, int* delay) {
    for(int i = 1; i < argc; i++) {
        if (strcmp(argv[i],"--delay") == 0 || strcmp(argv[i],"-d") == 0) {
            if (argv[i+1] != nullptr)
                *delay = atoi(argv[i+1]);
            else {
                fprintf(stderr, "Must specify a delay after \"--delay/-d\"\n");
                return -1;
            }
        } else if (strcmp(argv[i], "--scale") == 0 || strcmp(argv[i], "-s") == 0) {
            if (argv[i+1] != nullptr)
                *scale = atoi(argv[i+1]);
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
        fprintf(stderr, "Must specify a rom\n");
        return -1;
    }

    int delay = 16, scale = 10;

    if(parseArgs(argc,argv,&scale,&delay) != 0) { return -1; }

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

    std::string title = std::string("Chip-8 - ")
                        .append(std::string(argv[1]).erase(
                            0, std::string(argv[1]).find_last_of('/') + 1
                        ));

    SDL_SetWindowTitle(window, title.c_str());
    

    SDL_Texture *texture = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA32,SDL_TEXTUREACCESS_STREAMING,64,32);
    SDL_Event event;

    bool quit = false;

    Mem mem(argv[1]);
    Cpu cpu(&mem);

    while(!quit) {
        auto start = cl_hires::now();

        while(!cpu.draw && !cpu.idle)
            cpu.execute(&event);

        cpu.idle = false;

        float average_core = std::chrono::duration<float, std::milli>(cl_hires::now() - start).count();
        std::string core_fps = std::to_string(1000/average_core);

        cpu.input(&event,&quit);

        if(cpu.draw) {
            SDL_UpdateTexture(texture,NULL,cpu.mem->display,64*sizeof(color));
            SDL_RenderCopy(renderer,texture,NULL,NULL);
            SDL_RenderPresent(renderer);
            cpu.draw = false;
        }

        SDL_Delay(delay);
        
        float average_window = std::chrono::duration<float, std::milli>(cl_hires::now() - start).count();
        std::string window_fps = std::to_string(1000/average_window);
        SDL_SetWindowTitle(window, std::string(title + " [" + window_fps.erase(window_fps.find_first_of('.')) + 
                                            " wfps, " + core_fps.erase(core_fps.find_first_of('.')) + " cfps]").c_str());
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}