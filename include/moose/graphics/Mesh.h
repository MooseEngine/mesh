#ifndef MOOSE_GRAPHICS_MESH_H
#define MOOSE_GRAPHICS_MESH_H

#include <cstdint>
#include <vector>

namespace moose::core {
	struct Vec4;
}

namespace moose::graphics {
	struct Vertex;

	struct Mesh {
		std::vector<Vertex>		vertices;	// All points in the mesh
		std::vector<uint32_t>	indices;	// Indices of `vertices`. Every 3 indices reference 3 vertices that form a triangle
	};
}

#endif /* MOOSE_GRAPHICS_MESH_H */
