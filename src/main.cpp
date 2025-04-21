#include <stdio.h>

#include "moose/core/Mat4.h"
#include "moose/core/Vec3.h"
#include "moose/core/Vec4.h"
#include "moose/engine/Engine.h"
#include "moose/graphics/Camera.h"
#include "moose/graphics/Vertex.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

int main() {
	/*
	 * Create renderer
	 * Create camera
	 *
	 * Load object/mesh/triangle
	 *
	 * renderer.clear()
	 * renderer.drawMesh(Mesh, Camera)
	 * renderer.present()
	 */

	auto camera = std::make_unique<moose::graphics::Camera>(
		moose::core::Vec3{ 334, 119, 0	},
		moose::core::Vec3{ 334, 119, 300 }, // center of triangle
		moose::core::Vec3{ 0  , 1  , 0	},
		60.0f, (float) SCREEN_WIDTH / (float) SCREEN_HEIGHT, 1.0f, 1000.0f
	);

	moose::engine::Engine engine = moose::engine::Engine(800, 600, std::move(camera), "Moose Engine");

	std::vector<moose::graphics::Vertex> vertices {
		{{502, 3  , 200}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
		{{381, 242, 300}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
		{{119, 113, 400}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
	};
	std::vector<uint32_t> indices {0, 1, 2};

	auto triangle = std::make_shared<moose::graphics::Mesh>(std::move(vertices), std::move(indices));

	engine.addMesh(std::move(triangle));

	engine.run();

	return 0;
}
