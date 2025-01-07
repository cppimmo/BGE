#ifndef _BGE_GEOMETRY_HPP_
#define _BGE_GEOMETRY_HPP_

#include <cstddef>

namespace BGE
{
	/*
Basic Geometries:
	VertexPosition
	VertexPositionNormal
	VertexPositionNormalTex
Advanced Meshes:
	VertexPositionNormalTexTangent
	VertexSkinned
	VertexMorph
Specialized Applications:
	VertexScreenSpace
	VertexTerrain
	VertexParticle
    */
#pragma pack(push, 1)
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 tangent;
		glm::vec2 texcoord;
	};
#pragma pack(pop)


	using Vertices = std::vector<Vertex>;
	//! Represents indices for vertices.
	using VertexIndex = std::uint32_t;

	using Indices = std::vector<VertexIndex>;

	struct MeshData
	{
		Vertices vertices;
		Indices indices;
	};

	class GeometryGenerator final
	{
	public:
		void CreateBox(float width, float height, float depth, MeshData &meshData);

		void CreateSphere(float radius, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData);

		void CreateGeosphere(float radius, std::uint32_t subdivisionCount, MeshData &meshData);

		void CreateCylinder(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData);

		void CreateGrid(float width, float depth, std::uint32_t rows, std::uint32_t columns, MeshData &meshData);

		void CreateFullscreenQuad(MeshData &meshData);
	private:
		void Subdivide(MeshData &meshData);
		void BuildCylinderTopCap(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData);
		void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData);
	};
	// TODO: Create Geometry builder interface with functions that take references to vertex and indices lists.
} // End namespace (BGE)

#endif /* !_BGE_GEOMETRY_HPP_ */
