#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Viewport.hpp"

namespace BGE
{
	GLViewport::GLViewport(const glm::ivec2 &kSize, const glm::ivec2 &kOffset)
		: m_size(kSize), m_offset(kOffset)
	{
	}

	glm::ivec2 GLViewport::VGetSize(void) const
	{
		return m_size;
	}

	glm::ivec2 GLViewport::VGetOffset(void) const
	{
		return m_offset;
	}
} // End namespace (BGE)
