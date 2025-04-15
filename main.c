#include <complex.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "lib/linalg.h"
#include "lib/model.h"

// 0xAARRGGBB
#define WHITE	0xFFFFFFFF
#define BLACK	0xFF000000
#define RED	0xFFFF0000
#define ORANGE	0xFFFFA500

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define CLAMP(x, min, max) (((x) < (min)) ? (min) : (((x) > (max)) ? (max) : (x)))
#define COLOR_TO_INT(channel) (int)(CLAMP(channel, 0.0f, 1.0f) * 255)

/*
typedef struct {
	float minX, maxX, minY, maxY;
} BoundingBox;
*/


// BoundingBox calculateBoundingBox(Triangle transformedTriangle) {
	// TODO: implement
// }

/**
 * @brief	Applies MVP to a triangle given the triangle and a camera. Uses some default settings for the projection matrix.
 *
 * @param	t	The triangle in question
 * @param	c	The camera in question
 *
 * @return	A	`Triangle` struct whose x, y, and z coordinates are the projected screen coordinates (where z is depth for depth
 *				buffering).
 */
Triangle transformAndProjectTriangle(Triangle t, Camera c) {
	Triangle screenTriangle = {
		.vertices = {
			{ .x = 0, .y = 0, .z = 0 },
			{ .x = 0, .y = 0, .z = 0 },
			{ .x = 0, .y = 0, .z = 0 }
		},
	};
	memcpy(screenTriangle.colors, t.colors, sizeof(screenTriangle.colors));

	const float fov		= deg2rad(60.0f);
	const float aspect	= (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;
	const float near	= 1.0f;
	const float far		= 1000.0f;

	Mat4 modelMatrix = getModelMatrix();
	Mat4 viewMatrix = getViewMatrix(c);
	Mat4 projMatrix = getProjectionMatrix(fov, aspect, near, far);

	// for each vertex in your triangle...
	for (int i = 0; i < NUM_TRIANGLE_VERTICES; i++) {
		Vec3 v = t.vertices[i];

		// Convert the vertex to homogenous coordinates
		Vec4 v4 = { v.x, v.y, v.z, 1.0f };

		// Apply transformations: model -> view -> projection

		Vec4 transformed = multiplyMat4Vec4(
				projMatrix, multiplyMat4Vec4(
				viewMatrix, multiplyMat4Vec4(
				modelMatrix, v4
			)));

		transformed.x /= transformed.w;
		transformed.y /= transformed.w;
		transformed.z /= transformed.w;

		screenTriangle.vertices[i].x = (int)((transformed.x + 1.0f) * 0.5f * SCREEN_WIDTH); // screenX
		screenTriangle.vertices[i].y = (int)((1.0f - (transformed.y + 1.0f) * 0.5f) * SCREEN_HEIGHT); // screenY
		screenTriangle.vertices[i].z = transformed.z; // depth for depth buffering

		screenTriangle.reciprocalDepths[i] = 1/transformed.z; // NOTE: left off here
		screenTriangle.icolors[i] = (Color) {
			.A = screenTriangle.colors[i].A / transformed.z,
			.R = screenTriangle.colors[i].R / transformed.z,
			.G = screenTriangle.colors[i].G / transformed.z,
			.B = screenTriangle.colors[i].B / transformed.z
		};
	}

	return screenTriangle;
}

int main() {
	// Prepare transformation matrices
	const Triangle triangle = {
		.vertices = {
			{ .x = 502, .y = 3  , .z = 200 },
			{ .x = 381, .y = 242, .z = 300 },
			{ .x = 119, .y = 113, .z = 400 }
		},
		.colors = {
			{ .A = 1.0f, .R = 1.0f, .G = 0.0f, .B = 0.0f },
			{ .A = 1.0f, .R = 0.0f, .G = 1.0f, .B = 0.0f },
			{ .A = 1.0f, .R = 0.0f, .G = 0.0f, .B = 1.0f },
		}
	};
	const float cameraSpeed = 300.0f;
	Camera camera = {
		.position	= (Vec3) { 334, 119, 0	},
		.target		= (Vec3) { 334, 119, 300}, // center of triangle
		.up			= (Vec3) { 0  , 1  , 0	}
	};

	// TODO: BoundingBox boundingBox = calculateBoundingBox(tScreen);

	
	// Initialize SDL
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// Create a window
	SDL_Window *window = SDL_CreateWindow(
		"Software Rasterizer",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN
	);
	if(window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_window;
	}

	// Create a renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if(renderer == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_renderer;
	}

	// Create a texture that will serve as our framebuffer.
	// Each pixel is 32 bits (ARGB format)
	SDL_Texture *texture = SDL_CreateTexture(
		renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT
	);
	if(texture == NULL) {
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		goto cleanup_texture;
	}

	uint32_t *framebuffer = malloc(SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint32_t));
	if(framebuffer == NULL) {
		fprintf(stderr, "Failed to allocate memory for the framebuffer.\n");
		goto cleanup_framebuffer;
	}

	
	// begin render loop
	{
		Uint32 lastTime = SDL_GetTicks();

		int quit = 0;
		SDL_Event event;
		while (!quit) {
			Uint32 currentTime = SDL_GetTicks();

			// deltaTime in seconds
			float deltaTime = (currentTime - lastTime) / 1000.0f;
			lastTime = currentTime;

			// handle any events
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) quit = 1;
				else if (event.type == SDL_KEYDOWN) { // Handle keypressed events (e.g. camera movement)
					switch(event.key.keysym.sym) { // switch based off type of key -- `sym` --> `symbol`
						case SDLK_w:  // Move forward (increase z)
							camera.position.z += cameraSpeed * deltaTime;
							camera.target.z += cameraSpeed * deltaTime;
							break;
						case SDLK_s:  // Move backward (decrease z)
							camera.position.z -= cameraSpeed * deltaTime;
							camera.target.z -= cameraSpeed * deltaTime;
							break;
						case SDLK_a:  // Move left (decrease x)
							camera.position.x -= cameraSpeed * deltaTime;
							camera.target.x -= cameraSpeed * deltaTime;
							break;
						case SDLK_d:  // Move right (increase x)
							camera.position.x += cameraSpeed * deltaTime;
							camera.target.x += cameraSpeed * deltaTime;
							break;
						case SDLK_q:  // Move upward (decrease y)
							camera.position.y -= cameraSpeed * deltaTime;
							camera.target.y -= cameraSpeed * deltaTime;
							break;
						case SDLK_e:  // Move downward (increase y)
							camera.position.y += cameraSpeed * deltaTime;
							camera.target.y += cameraSpeed * deltaTime;
							break;
						default:
						break;
					}
				}
			}

			// Rasterize the camera (this time within the loop so we get updated camera placement info
			Triangle tt = transformAndProjectTriangle(triangle, camera);

			// Fill framebuffer with BLACK if not inside triangle.
			// If inside triangle, make it ORANGE
			for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
				// You can optimize this by calculating the max and min x and y values that the triangle can be.
				Vec3 P = { .x = i % SCREEN_WIDTH, .y = i / SCREEN_WIDTH };
				Bary3 bary = getBarycentricCoordinates(P, tt);

				if (isInsideTriangleFromBary(bary)) {

					Color interpolatedColor = addColors(3,
						multiplyFloatColor(bary.alpha,	tt.icolors[0]),
						multiplyFloatColor(bary.beta,	tt.icolors[1]),
						multiplyFloatColor(bary.gamma,	tt.icolors[2])
					);

					float interpolatedDepth =
						bary.alpha	* tt.reciprocalDepths[0] +
						bary.beta	* tt.reciprocalDepths[1] +
						bary.gamma	* tt.reciprocalDepths[2];

					Color final = multiplyFloatColor(1.0f / interpolatedDepth, interpolatedColor);

					framebuffer[i] = (COLOR_TO_INT(final.R) << 16) | (COLOR_TO_INT(final.G) << 8) | COLOR_TO_INT(final.B);
				} else {
					framebuffer[i] = BLACK;
				}
			}
	
			// update the texture with the framebuffer content
			// The last parameter is the `pitch` of the pixel data
			// `pitch`/`stride` is the number of bytes between the start of one row of pixel data
			// and the start of the next row (in memory)
			SDL_UpdateTexture(texture, NULL, framebuffer, SCREEN_WIDTH * sizeof(uint32_t));

			// render the texture
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);

			// cap the frame rate at ~60fps
			// 1000ms / 60 ~= 1 frame every 16.67 ms
			SDL_Delay(16);
		}
	}

cleanup_framebuffer:
	if (framebuffer) {
		free(framebuffer);
		framebuffer = NULL;
	}
cleanup_texture:
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
cleanup_renderer:
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
cleanup_window:
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();

	return 0;
}



