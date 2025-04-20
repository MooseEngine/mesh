#include <stdio.h>

#include "moose/core/Mat4.h"
#include "moose/core/Vec4.h"
#include "moose/graphics/Renderer.h"

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
	using moose::graphics::Renderer;
	using moose::core::Mat4;
	using moose::core::Vec4;

	Renderer renderer = Renderer(800, 600);

	return 0;
}
