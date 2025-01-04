#ifndef _BGE_GEOMETRY_HPP_
#define _BGE_GEOMETRY_HPP_

#include <cstddef>

namespace BGE
{
	struct Vertex_UnlitTextured
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texcoord;
	};

	//! Represents indices for vertices.
	using VertexIndex = std::uint32_t;

	using Indices = std::vector<VertexIndex>;

	// TODO: Create Geometry builder interface with functions that take references to vertex and indices lists.
} // End namespace (BGE)

#endif /* !_BGE_GEOMETRY_HPP_ */
