#include <stdexcept>

#include "moose/graphics/Renderer.h"

#define ASSERT_NOT(throw_condition, error) if( (throw_condition) ) throw std::runtime_error((error));

namespace moose::graphics {
	Renderer::Renderer(int width, int height) {
		this->width = width;
		this->height = height;

		ASSERT_NOT(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError());

		createWindow();
		createRenderer();
		createTexture();

		this->frameBuffer.resize(width * height);
		this->depthBuffer.resize(width * height);
	}

	Renderer::~Renderer() {
		// tear down in reverse order of creation
		if (texture)	SDL_DestroyTexture(this->texture);
		if (renderer)	SDL_DestroyRenderer(this->renderer);
		if (window)		SDL_DestroyWindow(this->window);

		SDL_Quit();
	}

	void Renderer::createWindow() {
		SDL_Window *window = SDL_CreateWindow(
			"Software Rasterizer",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			this->width,
			this->height,
			SDL_WINDOW_SHOWN
		);
		ASSERT_NOT(window == NULL, SDL_GetError());

		this->window = window;
	}
	
	void Renderer::createRenderer() {
		SDL_Renderer *renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
		ASSERT_NOT(renderer == NULL, SDL_GetError());

		this->renderer = renderer;
	}

	void Renderer::createTexture() {
		SDL_Texture *texture = SDL_CreateTexture(
			this->renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			this->width, this->height
		);
		ASSERT_NOT(texture == NULL, SDL_GetError()); 

		this->texture = texture;
	}

}
