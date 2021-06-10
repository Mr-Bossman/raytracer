#include "common.h"
#include "sdl_funcs.h"
#include "constants.h"
static SDL_Window* gWindow = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture *texture = NULL;
SDL_Window* sdl_getwindow(){
    return gWindow;
}
bool sdl_init()
{
	bool success = true;
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		gWindow = SDL_CreateWindow( "RAY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE |SDL_RENDERER_PRESENTVSYNC);
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
            renderer = SDL_CreateRenderer(gWindow, -1, 0);
            if (renderer == NULL) {
                printf("Unable to create renderer: %s", SDL_GetError());
                sdl_free();
			    success = false;

            }
            SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "cubic");
            SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
            SDL_SetRelativeMouseMode( SDL_TRUE );
            SDL_WarpMouseInWindow(gWindow, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
            texture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGB24,
                SDL_TEXTUREACCESS_STREAMING,
                SCREEN_WIDTH,
                SCREEN_HEIGHT);
                if (texture == NULL) {
                    printf("Unable to create texture: %s", SDL_GetError());
                    sdl_free();
			        success = false;
                }
            
		}

	}

	return success;
}

void sdl_close(int sig)
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_Quit();
    exit(sig);
}
void sdl_free()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    SDL_DestroyTexture(texture);
    texture = NULL;
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	SDL_Quit();
}
void sdl_frame(){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
}

char * sdl_pixels_lock(){
    int texture_pitch = 0;
    void* texture_pixels = NULL;

     if (SDL_LockTexture(texture, NULL, &texture_pixels, &texture_pitch) != 0) {
        SDL_Log("Unable to lock texture: %s", SDL_GetError());
    }
    return (char*)texture_pixels;
}
void sdl_pixels_unlock(){
    SDL_UnlockTexture(texture);
}