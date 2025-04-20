#ifndef MOOSE_GRAPHICS_RENDERER_H
#define MOOSE_GRAPHICS_RENDERER_H

#include <SDL2/SDL.h>
#include <vector>

#include "moose/graphics/Camera.h"
#include "moose/graphics/Mesh.h"

namespace moose::graphics {
	class Renderer {
	public:
		Renderer(int width, int height);
		~Renderer();

		bool clear();
		bool drawMesh(Mesh mesh, Camera camera);
		bool present();

	private:
		int width, height;
		std::vector<uint32_t> frameBuffer;
		std::vector<float> depthBuffer;

		SDL_Window *window		= nullptr;
		SDL_Renderer *renderer	= nullptr;
		SDL_Texture *texture	= nullptr;

		void createWindow();
		void createRenderer();
		void createTexture();
	};
}

#endif /* MOOSE_GRAPHICS_RENDERER_H */

