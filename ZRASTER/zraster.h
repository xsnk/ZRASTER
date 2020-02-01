#pragma once

#include <SDL.h>
#undef main

#include "geometry.h"

const int WIDTH = 320;
const int HEIGHT = 240;

class ZRASTER {
public:

	ZRASTER();

	bool init(const char* title, int width, int height);
	void update();
	void pollevent();
	void clear();

	inline void putpixel(int x, int y, uint32_t color)
	{
		fbuf[x + y * WIDTH] = color;
	}

	inline void drawline(int x0, int y0, int x1, int y1, uint32_t color)
	{
		bool steep = false;
		if (std::abs(x0 - x1) < std::abs(y0 - y1)) {
			std::swap(x0, y0);
			std::swap(x1, y1);
			steep = true;
		}

		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}

		int dx = x1 - x0;
		int dy = y1 - y0;

		int derror2 = std::abs(dy) * 2;
		int error2 = 0;
		int y = y0;

		for (int x = x0; x <= x1; x++) {
			if (steep) {
				putpixel(y, x, color);
			}
			else {
				putpixel(x, y, color);
			}
			error2 += derror2;
			if (error2 > dx) {
				y += (y1 > y0) ? 1 : -1;
				error2 -= dx * 2;
			}
		}
	}

	inline void drawtriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
	{
		drawline(x0, y0, x1, y1, color);
		drawline(x1, y1, x2, y2, color);
		drawline(x2, y2, x0, y0, color);
	}

private:
	uint32_t* fbuf = nullptr;

private:
	SDL_Texture* screen_texture;
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Event event;

};