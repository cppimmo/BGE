#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Renderer.hpp"

#include "Graphics/GL/Shaders.hpp"
#include "Graphics/GL/ShaderProgram.hpp"

namespace BGE
{
	GLRenderer::GLRenderer(void)
		: m_pViewport(std::make_unique<GLViewport>(glm::ivec2(1280, 720)))
	{
	}

	GLRenderer::~GLRenderer(void)
	{
	}

	bool GLRenderer::VInit(void)
	{
		return true;
	}

	void GLRenderer::VShutdown(void)
	{
	}

	RendererImpl GLRenderer::VGetImpl(void) const
	{
		return RendererImpl::kOpenGL;
	}

	StrongIShaderFactoryPtr GLRenderer::VCreateShaderFactory(void)
	{
		return std::make_shared<GLShaderFactory>();
	}

	StrongIShaderProgramPtr GLRenderer::VCreateShaderProgram(std::string_view name)
	{
		auto pShaderProgram = std::make_shared<GLShaderProgram>();
		return pShaderProgram;
	}

	StrongIShaderProgramPtr GLRenderer::VGetShaderProgram(std::string_view name)
	{
		return nullptr;
	}

	void GLRenderer::VSetViewport(const IViewport &kViewport)
	{
		m_pViewport = std::make_unique<GLViewport>(dynamic_cast<const GLViewport &>(kViewport));
	}

	const IViewport &GLRenderer::VGetViewport(void) const
	{
		BGE_ASSERT(m_pViewport);
		return *m_pViewport.get();
	}

	void GLRenderer::VSetBackgroundColor(const glm::vec4 &kColor)
	{
		m_bgColor = kColor;
	}

	glm::vec4 GLRenderer::VGetBackgroundColor(void)
	{
		return m_bgColor;
	}

	bool GLRenderer::VTakeScreenshot(void)
	{
		return true;
	}
} // End namespace (BGE)
