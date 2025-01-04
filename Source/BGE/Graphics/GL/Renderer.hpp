#ifndef _BGE_GL_RENDERER_HPP_
#define _BGE_GL_RENDERER_HPP_

#include "Graphics/Renderer.hpp"
#include "Graphics/GL/Viewport.hpp"

namespace BGE
{
	class GLRenderer; // Forward declare
	BGE_DECLARE_PTR(GLRenderer);

	/**
	 * @brief .
	 */
	class GLRenderer : public IRenderer
	{
	private:
		UniqueIViewportPtr m_pViewport;
		// TODO: What should the default clear color be?
		glm::vec4 m_bgColor;
	public:
		GLRenderer(void);
		virtual ~GLRenderer(void) override;

		virtual bool VInit(void) override;
		virtual void VShutdown(void) override;
		virtual RendererImpl VGetImpl(void) const override;

		virtual void VBeginFrame(void) override {}
		virtual void VEndFrame(void) override {}

		virtual void VSetViewport(const IViewport &kViewport) override;
		virtual const IViewport &VGetViewport(void) const override;
		virtual void VSetBackgroundColor(const glm::vec4 &kColor) override;
		virtual glm::vec4 VGetBackgroundColor(void) override;

		virtual void VEnableDepthTest(bool bEnable) override {}
		virtual void VEnableBlending(bool bEnable) override {}
		virtual void VSetBlendMode(BlendMode mode) override {}
		virtual BlendMode VGetBlendMode(void) const override {}

		virtual StrongIShaderFactoryPtr VCreateShaderFactory(void) override;
		virtual StrongIShaderProgramPtr VCreateShaderProgram(std::string_view name) override;
		virtual StrongIShaderProgramPtr VGetShaderProgram(std::string_view name) override;

		virtual bool VTakeScreenshot(void) override;

		virtual void VEnableDebugOutput(bool bEnable) override {}
		virtual std::string VGetRendererInfo(void) const override {}
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_RENDERER_HPP_ */
