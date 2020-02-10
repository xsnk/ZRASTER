#include "zraster.h"


ZRASTER::ZRASTER()
{
	fbuf = new uint32_t[sizeof(uint32_t) * WIDTH * HEIGHT * 4];
}

ZRASTER::~ZRASTER()
{
	delete fbuf;
}

bool ZRASTER::init(const char* title, int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);

	window = SDL_CreateWindow(title,
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		640, 480,
		SDL_WINDOW_RESIZABLE);
	
	if (!window) {
		return false;
	}

	renderer = SDL_CreateRenderer(window,
		-1, SDL_RENDERER_PRESENTVSYNC);

	if (!renderer) {
		return false;
	}

	// Since we are going to display a low resolution buffer,
	// it is best to limit the window size so that it cannot
	// be smaller than our internal buffer size.
	SDL_SetWindowMinimumSize(window, width, height);

	SDL_RenderSetLogicalSize(renderer, width, height);
	SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

	screen_texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING,
		width, height);


	return true;
}

void ZRASTER::update()
{

	SDL_RenderClear(renderer);
	SDL_UpdateTexture(screen_texture, NULL, fbuf, WIDTH * 4);
	SDL_RenderCopy(renderer, screen_texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}

void ZRASTER::pollevent()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT) exit(0);
	}
}

void ZRASTER::clear()
{
	SDL_memset(fbuf, 0x00000000, WIDTH * HEIGHT * 4);
}
