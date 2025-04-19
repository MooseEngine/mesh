#ifndef MOOSE_GRAPHICS_RENDERER_H
#define MOOSE_GRAPHICS_RENDERER_H

#include <SDL2/SDL.h>

#include "moose/graphics/Camera.h"
#include "moose/graphics/Mesh.h"

namespace moose::graphics {
	class Renderer {
	private:
		int width, height;
		SDL_Window *window;
		SDL_Renderer *renderer;
		SDL_Texture *texture;

		bool initializeSDL(); // TODO: is there a better name for this? abstract more?
		SDL_Window createWindow();
		SDL_Renderer createRenderer();
		SDL_Texture createTexture();

		void cleanup();
	public:
		Renderer(int width, int height);

		bool clear();
		bool drawMesh(Mesh mesh, Camera camera);
		bool present();
	};
}

#endif /* MOOSE_GRAPHICS_RENDERER_H */

