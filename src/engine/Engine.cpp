#include <iostream>
#include <ostream>
#include <stdexcept>
#include <SDL2/SDL.h>

#include "moose/core/Mat4.h"
#include "moose/core/Vec2.h"
#include "moose/core/Vec3.h"
#include "moose/core/Vec4.h"
#include "moose/graphics/Camera.h"
#include "moose/graphics/Mesh.h"
#include "moose/graphics/ScreenTriangle.h"
#include "moose/graphics/ClipVertex.h"
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

		std::cout << "Running\n" << std::flush;

		while (running) {
			Uint32 currentTime = SDL_GetTicks();
			float deltaTime = (currentTime - lastTime) / 1000.0f; // deltaTime in seconds
			lastTime = currentTime;

			this->pollEvents();
			this->render();
		}
	}

	void Engine::addMesh(std::shared_ptr<graphics::Mesh> mesh) {
		meshes.push_back(mesh);
	}
	void Engine::clearMeshes() {
		meshes.clear();
	}

/* PRIVATE */
	void Engine::createWindow() {
		SDL_Window *window = SDL_CreateWindow(
			this->title.c_str(),
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
			if (event.type == SDL_QUIT) running = false;
		}

	}

	void Engine::render() {
		this->clear();

		for (auto &m : meshes) {
			this->drawMesh(*m, *this->camera);
		}
		/* wipe your CPU buffer to opaque red
		std::fill(
			frameBuffer.begin(),
			frameBuffer.end(),
			0xFFFF0000u    // ARGB: A=0xFF,R=0xFF,G=B=0
		);
		*/

		this->present();
	}

	void Engine::clear() {
		SDL_RenderClear(renderer);
		std::fill(depthBuffer.begin(), depthBuffer.end(), 1.0f);  // “far” plane
		std::fill(frameBuffer.begin(), frameBuffer.end(), 0xFF000000);  // black opaque
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

			graphics::ClipVertex tv0 = this->transformVertex(v0, MVP);
			/* NOTE: DEBUG
			std::cout << "V0 → (" 
				<< tv0.screenPos.x << ", " 
				<< tv0.screenPos.y << "), depth=" 
				<< tv0.depth << "\n";
			// NOTE: END DEBUG */
			graphics::ClipVertex tv1 = this->transformVertex(v1, MVP);
			graphics::ClipVertex tv2 = this->transformVertex(v2, MVP);

			/* NOTE: DEBUG
			auto plotPixel = [&](const graphics::ClipVertex &tv){
				int sx = int(tv.screenPos.x);
				int sy = int(tv.screenPos.y);
				if (sx >= 0 && sx < screenWidth && sy >= 0 && sy < screenHeight) {
					size_t idx = sy * screenWidth + sx;
					frameBuffer[idx] = 0xFFFFFFFFu;  // white opaque
				}
			};

			plotPixel(tv0);
			plotPixel(tv1);
			plotPixel(tv2);
			// NOTE: END DEBUG */

			this->rasterizeTriangle(tv0, tv1, tv2);
		}
	}

	graphics::ClipVertex Engine::transformVertex(const graphics::Vertex &vertex, core::Mat4 &MVP) const {
		// Promote to Vec4
		core::Vec4 clipPos = MVP * core::Vec4{
			vertex.position.x,
			vertex.position.y,
			vertex.position.z,
			1
		};

		float oneOverW = 1 / clipPos.w;

		clipPos = clipPos / clipPos.w;

		return graphics::ClipVertex{
			{
				((clipPos.x + 1.0f) * 0.5f * this->screenWidth),			// screenX
				((1.0f - (clipPos.y + 1.0f) * 0.5f) * this->screenHeight)	// screenY
			},
			clipPos.z,												// depth
			oneOverW,												// 1 / w
			vertex.color * oneOverW									// color / w
		};
	}

	void Engine::rasterizeTriangle(
			graphics::ClipVertex &tv0,
			graphics::ClipVertex &tv1,
			graphics::ClipVertex &tv2
			) {
		
		graphics::ScreenTriangle tri {tv0, tv1, tv2};

		// bounding‐box optimization (optional, but *highly* recommended)
		int minX = std::max(0,   int(std::floor(std::min({ tv0.screenPos.x, tv1.screenPos.x, tv2.screenPos.x }))));
		int maxX = std::min(screenWidth-1,  int(std::ceil (std::max({ tv0.screenPos.x, tv1.screenPos.x, tv2.screenPos.x }))));
		int minY = std::max(0,   int(std::floor(std::min({ tv0.screenPos.y, tv1.screenPos.y, tv2.screenPos.y }))));
		int maxY = std::min(screenHeight-1, int(std::ceil (std::max({ tv0.screenPos.y, tv1.screenPos.y, tv2.screenPos.y }))));

		for (int y = minY; y <= maxY; ++y) {
			for (int x = minX; x <= maxX; ++x) {
				core::Vec2 p{ x + 0.5f, y + 0.5f };

				graphics::raster::Bary3 bary = graphics::raster::computeBarycentrics(tri, p);
			
				/* NOTE: disabled for debugging 
				
				// perspective‑correct interpolation
				float invW = bary.alpha * tv0.oneOverW
					+ bary.beta * tv1.oneOverW
					+ bary.gamma * tv2.oneOverW;

				graphics::ColorF c = (bary.alpha * tv0.colorOverW
					+ bary.beta * tv1.colorOverW
					+ bary.gamma * tv2.colorOverW) / invW;

				float depth = (bary.alpha * tv0.depth
					+ bary.beta * tv1.depth
					+ bary.gamma * tv2.depth) / invW;

				// depth test
				size_t idx = y * screenWidth + x;
				if (depth < depthBuffer[idx]) {
					depthBuffer[idx]    = depth;
					frameBuffer[idx]    = graphics::packColor(c);
				}
				*/

				if ((bary.alpha >= 0 && bary.beta >= 0 && bary.gamma >= 0)
				 && (bary.alpha <= 1 && bary.beta <= 1 && bary.gamma <= 1)) {
					// then is inside triangle
					
					float denom = bary.alpha * tri.v0.oneOverW + bary.beta * tri.v1.oneOverW + bary.gamma * tri.v2.oneOverW;

					graphics::ColorF c =
						( bary.alpha	* tri.v0.colorOverW
						+ bary.beta		* tri.v1.colorOverW
						+ bary.gamma	* tri.v2.colorOverW ) / denom;

					float depth = 
						( bary.alpha	* tri.v0.depth * tri.v0.oneOverW
						+ bary.beta		* tri.v1.depth * tri.v1.oneOverW
						+ bary.gamma	* tri.v2.depth * tri.v2.oneOverW ) / denom;

					size_t idx = y * screenWidth + x;

					if(depth < depthBuffer[idx]) {
						depthBuffer[idx] = depth;
						frameBuffer[idx] = graphics::packColor(c);
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
