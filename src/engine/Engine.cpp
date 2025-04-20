#include <stdexcept>
#include <SDL2/SDL.h>

#include "moose/core/Mat4.h"
#include "moose/core/Vec2.h"
#include "moose/core/Vec3.h"
#include "moose/core/Vec4.h"
#include "moose/graphics/Camera.h"
#include "moose/graphics/Mesh.h"
#include "moose/graphics/ScreenTriangle.h"
#include "moose/graphics/TransformedVertex.h"
#include "moose/graphics/Vertex.h"
#include "moose/graphics/ColorF.h"
#include "moose/graphics/raster/RasterUtils.h"
#include "moose/engine/Engine.h"

#define ASSERT_NOT(throw_condition, error) if( (throw_condition) ) throw std::runtime_error((error));

namespace moose::engine {
/* PUBLIC */
	Engine::Engine(int screenWidth, int screenHeight, std::unique_ptr<graphics::Camera> camera, std::string title)
		: screenWidth(screenWidth), screenHeight(screenHeight), title(std::move(title)), camera(std::move(camera))
	{
		ASSERT_NOT(SDL_Init(SDL_INIT_VIDEO) < 0, SDL_GetError());

		createWindow();
		createRenderer();
		createTexture();

		this->frameBuffer.resize(screenWidth * screenHeight);
		this->depthBuffer.resize(screenWidth * screenHeight);

		this->running = true;
	}

	Engine::~Engine() {
		// tear down in reverse order of creation
		if (texture)	SDL_DestroyTexture(this->texture);
		if (renderer)	SDL_DestroyRenderer(this->renderer);
		if (window)		SDL_DestroyWindow(this->window);

		SDL_Quit();
	}

	void Engine::run() {
		Uint32 lastTime = SDL_GetTicks();

		while (running) {
			Uint32 currentTime = SDL_GetTicks();
			float deltaTime = (currentTime - lastTime) / 1000.0f; // deltaTime in seconds
			lastTime = currentTime;

			this->pollEvents();
			this->render();
		}
	}


/* PRIVATE */
	void Engine::createWindow() {
		SDL_Window *window = SDL_CreateWindow(
			"Software Rasterizer",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			this->screenWidth,
			this->screenHeight,
			SDL_WINDOW_SHOWN
		);
		ASSERT_NOT(window == NULL, SDL_GetError());

		this->window = window;
	}
	
	void Engine::createRenderer() {
		SDL_Renderer *renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
		ASSERT_NOT(renderer == NULL, SDL_GetError());

		this->renderer = renderer;
	}

	void Engine::createTexture() {
		SDL_Texture *texture = SDL_CreateTexture(
			this->renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			this->screenWidth, this->screenHeight
		);
		ASSERT_NOT(texture == NULL, SDL_GetError()); 

		this->texture = texture;
	}

	void Engine::pollEvents() {
		SDL_Event event;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) running = 0;
		}

	}

	void Engine::render() {
		this->clear();

		for (auto &m : meshes) {
			this->drawMesh(*m, *this->camera);
		}

		this->present();
	}

	void Engine::clear() {
		SDL_RenderClear(renderer);
	}

	void Engine::drawMesh(graphics::Mesh &mesh, graphics::Camera &camera) {
		const core::Mat4 model = core::Mat4::identity();

		const core::Mat4 view = camera.view();
		const core::Mat4 proj = camera.proj();

		core::Mat4 MVP = proj * view * model; // TODO: implement Mat4 * Mat4

		for (size_t i = 0; i + 2 < mesh.indices.size(); i += 3) {
			const graphics::Vertex v0 = mesh.vertices[ mesh.indices[i + 0] ];
			const graphics::Vertex v1 = mesh.vertices[ mesh.indices[i + 1] ];
			const graphics::Vertex v2 = mesh.vertices[ mesh.indices[i + 2] ];

			graphics::TransformedVertex tv0 = this->transformVertex(v0, MVP);
			graphics::TransformedVertex tv1 = this->transformVertex(v1, MVP);
			graphics::TransformedVertex tv2 = this->transformVertex(v2, MVP);

			this->rasterizeTriangle(tv0, tv1, tv2);
		}
	}

	graphics::TransformedVertex Engine::transformVertex(const graphics::Vertex &vertex, core::Mat4 &MVP) const {
		// Promote to Vec4
		core::Vec4 clipPos = MVP * core::Vec4{
			vertex.position.x,
			vertex.position.y,
			vertex.position.z,
			1
		};

		float oneOverW = 1 / clipPos.w;

		clipPos /= clipPos.w;

		return graphics::TransformedVertex{
			{
				((clipPos.x + 1.0f) * 0.5f * this->screenWidth),			// screenX
				((1.0f - (clipPos.y + 1.0f) * 0.5f) * this->screenHeight)	// screenY
			},
			clipPos.y,												// depth
			oneOverW,												// 1 / w
			vertex.color * oneOverW									// color / w
		};
	}
	void Engine::rasterizeTriangle(
			graphics::TransformedVertex &v0,
			graphics::TransformedVertex &v1,
			graphics::TransformedVertex &v2
			) {
		graphics::ScreenTriangle tri = { v0, v1, v2 };
		for(int i = 0; i < screenWidth * screenHeight; i++) {
			tri.invArea = graphics::raster::edgeFunction(tri.v0.screenPos, tri.v1.screenPos, tri.v2.screenPos);

			// TODO: bounding box
			for(int y = 0; y < this->screenHeight; y++) {
				for(int x = 0; x < this->screenWidth; x++) {
					core::Vec2 p{ x + 0.5f, y + 0.5f }; // Sample from the middle of the pixel

					// Compute the three un‑normalized weights
					float w0 = graphics::raster::edgeFunction(tri.v1.screenPos, tri.v2.screenPos, p);
					float w1 = graphics::raster::edgeFunction(tri.v2.screenPos, tri.v0.screenPos, p);
					float w2 = graphics::raster::edgeFunction(tri.v0.screenPos, tri.v1.screenPos, p);

					// compute true barycentric coordinates
					core::Vec3 bary{ w0, w1, w2 };
					bary *= tri.invArea;

					if (bary.x >= 0 && bary.y >= 0 && bary.z >= 0) {
						// perspective‑correct interpolate:
						float invW = bary.x * tri.v0.oneOverW
							+ bary.y * tri.v1.oneOverW
							+ bary.z * tri.v2.oneOverW;

						graphics::ColorF c = (bary.x * tri.v0.colorOverW
							+ bary.y * tri.v1.colorOverW
							+ bary.z * tri.v2.colorOverW) / invW;

						float depth = (bary.x * tri.v0.depth
							+ bary.y * tri.v1.depth
							+ bary.z * tri.v2.depth) / invW;

						// depth-test and write pixel
						if (this->depthBuffer[y*this->screenWidth + x] > depth) {
							this->depthBuffer[y*this->screenWidth + x] = depth;
							this->frameBuffer[y*this->screenWidth + x] = graphics::packColor(c);
						}
					}
				}
			}
		}
	}

	void Engine::present() {
		// update the texture with the framebuffer content
		// The last parameter is the `pitch` of the pixel data
		// `pitch`/`stride` is the number of bytes between the start of one row of pixel data
		// and the start of the next row (in memory)
		SDL_UpdateTexture(
				this->texture,
				nullptr,
				this->frameBuffer.data(),
				this->screenWidth * static_cast<int>(sizeof(uint32_t)) // pitch in bytes
			);

		// render the texture
		SDL_RenderCopy(this->renderer, this->texture, NULL, NULL);
		SDL_RenderPresent(this->renderer);

		// cap the frame rate at ~60fps
		// 1000ms / 60 ~= 1 frame every 16.67 ms
		SDL_Delay(16);
	}
}
