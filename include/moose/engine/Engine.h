#ifndef MOOSE_GRAPHICS_ENGINE_H
#define MOOSE_GRAPHICS_ENGINE_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include <string>

#include "moose/graphics/Mesh.h"

// Forward declaration so you don't have to include the whole header file.
// This is fine when you only use references/pointers of the forward-declared
// classes.
namespace moose::graphics {
	struct ClipVertex;
	struct Vertex;
	struct ScreenTriangle;
	class Camera;
}

namespace moose::core {
	struct Mat4;
}

namespace moose::engine {
	class Engine {
	public:
		Engine(int screenWidth, int screenHeight, std::unique_ptr<graphics::Camera> camera, std::string title);
		~Engine();

		void run();

		void addMesh(std::shared_ptr<graphics::Mesh> mesh);
		void clearMeshes();

	private:
		int screenWidth, screenHeight;
		std::string title;
		std::unique_ptr<graphics::Camera> camera;

		std::vector<uint32_t> frameBuffer; // color buffer
		std::vector<float> depthBuffer;

		std::vector<std::shared_ptr<graphics::Mesh>> meshes;

		SDL_Window *window		= nullptr;
		SDL_Renderer *renderer	= nullptr;
		SDL_Texture *texture	= nullptr;

		bool running = false;

		void createWindow();
		void createRenderer();
		void createTexture();

		void pollEvents();

		/* NOTE: should probably abstract these to a Renderer class */
		void render();
		void clear();
		void drawMesh(graphics::Mesh &mesh, graphics::Camera &camera);
		graphics::ClipVertex transformVertex(const graphics::Vertex &vertex, core::Mat4 &MVP) const;
		void rasterizeTriangle(
				graphics::ClipVertex &v0,
				graphics::ClipVertex &v1,
				graphics::ClipVertex &v2
				);
		void present();
		
	};
}

#endif /* MOOSE_GRAPHICS_ENGINE_H */

