#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Renderer.hpp"

#include "Graphics/GL/Shaders.hpp"
#include "Graphics/GL/ShaderProgram.hpp"
#include "Renderer.hpp"
#include "Renderer.hpp"

namespace BGE
{
	GLRenderer::GLRenderer(void)
		: m_pViewport(std::make_unique<GLViewport>(glm::ivec2(1280, 720)))
	{
	}

	GLRenderer::~GLRenderer(void)
	{
	}

	bool GLRenderer::VInit(const EngineOptions &kOptions)
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

	void GLRenderer::VOnResize(std::int32_t width, std::int32_t height)
	{
		const glm::ivec2 kOffset = m_pViewport->VGetOffset(); // (x, y)
		const glm::ivec2 kSize = m_pViewport->VGetSize(); // (width, height)

		glViewport(kOffset.x, kOffset.y, kSize.x, kSize.y);
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

	bool GLRenderer::VTakeScreenshot(const std::filesystem::path &kSaveGameDir)
	{
		return true;
	}
} // End namespace (BGE)
