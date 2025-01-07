#include "Engine/EngineStd.hpp"
#include "Graphics/Geometry.hpp"

#include <array>

#include <glm/ext/scalar_constants.hpp>

namespace BGE
{
	void GeometryGenerator::CreateBox(float width, float height, float depth, MeshData &meshData)
	{
		const float kHalfWidth = width / 2.0f;
		const float kHalfHeight = height / 2.0f;
		const float kHalfDepth = depth / 2.0f;

		std::array<Vertex, 24> vertices;
		// Fill in the front face vertex data
		vertices[0] = Vertex({ -kHalfWidth, -kHalfHeight, -kHalfDepth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
		vertices[1] = Vertex({ -kHalfWidth, +kHalfHeight, -kHalfDepth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
		vertices[2] = Vertex({ +kHalfWidth, +kHalfHeight, -kHalfDepth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });
		vertices[3] = Vertex({ +kHalfWidth, -kHalfHeight, -kHalfDepth }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });

		// Fill in the back face vertex data
		vertices[4] = Vertex({ -kHalfWidth, -kHalfHeight, +kHalfDepth }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });
		vertices[5] = Vertex({ +kHalfWidth, -kHalfHeight, +kHalfDepth }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
		vertices[6] = Vertex({ +kHalfWidth, +kHalfHeight, +kHalfDepth }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
		vertices[7] = Vertex({ -kHalfWidth, +kHalfHeight, +kHalfDepth }, { 0.0f, 0.0f, 1.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });

		// Fill in the top face vertex data
		vertices[8]  = Vertex({ -kHalfWidth, +kHalfHeight, -kHalfDepth }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
		vertices[9]  = Vertex({ -kHalfWidth, +kHalfHeight, +kHalfDepth }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
		vertices[10] = Vertex({ +kHalfWidth, +kHalfHeight, +kHalfDepth }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });
		vertices[11] = Vertex({ +kHalfWidth, +kHalfHeight, -kHalfDepth }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });

		// Fill in the bottom face vertex data
		vertices[12] = Vertex({ -kHalfWidth, -kHalfHeight, -kHalfDepth }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f });
		vertices[13] = Vertex({ +kHalfWidth, -kHalfHeight, -kHalfDepth }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });
		vertices[14] = Vertex({ +kHalfWidth, -kHalfHeight, +kHalfDepth }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
		vertices[15] = Vertex({ -kHalfWidth, -kHalfHeight, +kHalfDepth }, { 0.0f, -1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f });

		// Fill in the left face vertex data
		vertices[16] = Vertex({ -kHalfWidth, -kHalfHeight, +kHalfDepth }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 1.0f });
		vertices[17] = Vertex({ -kHalfWidth, +kHalfHeight, +kHalfDepth }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 0.0f, 0.0f });
		vertices[18] = Vertex({ -kHalfWidth, +kHalfHeight, -kHalfDepth }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 0.0f });
		vertices[19] = Vertex({ -kHalfWidth, -kHalfHeight, -kHalfDepth }, { -1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, { 1.0f, 1.0f });

		// Fill in the right face vertex data
		vertices[20] = Vertex({ +kHalfWidth, -kHalfHeight, -kHalfDepth }, { 1.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f });
		vertices[21] = Vertex({ +kHalfWidth, +kHalfHeight, -kHalfDepth }, { 1.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f });
		vertices[22] = Vertex({ +kHalfWidth, +kHalfHeight, +kHalfDepth }, { 1.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f });
		vertices[23] = Vertex({ +kHalfWidth, -kHalfHeight, +kHalfDepth }, { 1.0f, 0.0f, 0.0f },  { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f });

		meshData.vertices.assign(vertices.begin(), vertices.end());

		// Create the indices
		std::array<VertexIndex, 36> indices =
		{
			// Front face
			0, 1, 2, 0, 2, 3,
			// Back face
			4, 5, 6, 4, 6, 7,
			// Top face
			8, 9, 10, 8, 10, 11,
			// Bottom face
			12, 13, 14, 12, 14, 15,
			// Left face
			16, 17, 18, 16, 18, 19,
			// Right face
			20, 21, 22, 20, 22, 23
		};

		meshData.indices.assign(indices.begin(), indices.end());
	}

	void GeometryGenerator::CreateSphere(float radius, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData)
	{
		//meshData.vertices.clear();
		//meshData.indices.clear();

		// Compute the vertices starting at the top pole & moving down the stacks
		Vertex topVertex({ 0.0f, +radius, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f });
		Vertex bottomVertex({ 0.0f, -radius, 0.0f }, { 0.0f, -1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f });

		meshData.vertices.push_back(topVertex);

		float phiStep = glm::pi<float>() / static_cast<float>(stackCount);
		float thetaStep = (2.0f * glm::pi<float>()) / static_cast<float>(sliceCount);

		for (std::uint32_t i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			for (std::uint32_t j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex vertex;

				// Spherical to Cartesian
				vertex.position.x = radius * std::sinf(phi) * std::cosf(theta);
				vertex.position.y = radius * std::cosf(phi);
				vertex.position.z = radius * std::sinf(phi) * std::sinf(theta);

				// Partial derivate of P with respect to theta
				vertex.tangent.x = -radius * std::sinf(phi) * std::sinf(theta);
				vertex.tangent.y = 0.0f;
				vertex.tangent.z = +radius * std::sinf(phi) * std::cosf(theta);

				vertex.tangent = glm::normalize(vertex.tangent);
				vertex.normal = glm::normalize(vertex.position);

				vertex.texcoord.x = theta / glm::pi<float>();
				vertex.texcoord.y = phi / glm::pi<float>();

				meshData.vertices.push_back(vertex);
			}
		}

		meshData.vertices.push_back(bottomVertex);

		/*
		 *
		 */
		for (std::uint32_t i = 1; i <= sliceCount; ++i)
		{
			meshData.indices.push_back(0);
			meshData.indices.push_back(i + 1);
			meshData.indices.push_back(i);
		}

		/*
		 *
		 */
		std::uint32_t baseIndex = 1;
		std::uint32_t ringVertexCount = sliceCount + 1;
		for (std::uint32_t i = 0; i < stackCount - 2; ++i)
		{
			for (std::uint32_t j = 0; j < sliceCount; ++j)
			{
				meshData.indices.push_back(baseIndex + i * ringVertexCount + j);
				meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.indices.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.indices.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		/*
		 *
		 */

		// South pole vertex was added last
		std::uint32_t southPoleIndex = (std::uint32_t)meshData.vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring
		baseIndex = southPoleIndex - ringVertexCount;
		for (std::uint32_t i = 0; i < sliceCount; ++i)
		{
			meshData.indices.push_back(southPoleIndex);
			meshData.indices.push_back(baseIndex + i);
			meshData.indices.push_back(baseIndex + i + 1);
		}
	}

	void GeometryGenerator::CreateGeosphere(float radius, std::uint32_t subdivisionCount, MeshData &meshData)
	{
	}

	void GeometryGenerator::CreateCylinder(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData)
	{
	}

	void GeometryGenerator::CreateGrid(float width, float depth, std::uint32_t rows, std::uint32_t columns, MeshData &meshData)
	{
	}

	void GeometryGenerator::CreateFullscreenQuad(MeshData &meshData)
	{
	}

	void GeometryGenerator::Subdivide(MeshData &meshData)
	{
	}

	void GeometryGenerator::BuildCylinderTopCap(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData)
	{
	}

	void GeometryGenerator::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height, std::uint32_t sliceCount, std::uint32_t stackCount, MeshData &meshData)
	{
	}
} // End namespace (BGE)
