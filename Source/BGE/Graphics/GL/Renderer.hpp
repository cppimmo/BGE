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
		EngineOptions m_options; //!< .
		UniqueIViewportPtr m_pViewport; //!< .
		glm::vec4 m_bgColor; //!< .
		ImGuiContext *m_pImGuiContext = nullptr; //< Pointer to ImGui context.
		ImPlotContext *m_pImPlotContext = nullptr; //< Pointer to ImPlot context.
		bool m_bInitialized = false; //!< .
	public:
		GLRenderer(void);
		virtual ~GLRenderer(void) override;

		virtual bool VInit(const EngineOptions &kOptions) override;
		virtual void VShutdown(void) override;
		virtual RendererImpl VGetImpl(void) const override;

		virtual void VBeginFrame(void) override;
		virtual void VEndFrame(void) override;

		virtual void VOnResize(std::int32_t width, std::int32_t height) override;
		virtual void VSetViewport(const IViewport &kViewport) override;
		virtual const IViewport &VGetViewport(void) const override;
		virtual void VSetBackgroundColor(const glm::vec4 &kColor) override;
		virtual glm::vec4 VGetBackgroundColor(void) override;

		virtual void VEnableDepthTest(bool bEnable) override;
		virtual void VEnableBlending(bool bEnable) override;
		virtual void VSetBlendMode(BlendMode mode) override;
		virtual BlendMode VGetBlendMode(void) const override;

		virtual StrongIShaderFactoryPtr VCreateShaderFactory(void) override;
		virtual StrongIShaderProgramPtr VCreateShaderProgram(std::string_view name) override;
		virtual StrongIShaderProgramPtr VGetShaderProgram(std::string_view name) override;

		virtual bool VTakeScreenshot(const std::filesystem::path &kSaveGameDir) override;

		virtual void VEnableDebugOutput(bool bEnable) override;
		virtual std::string VGetRendererInfo(void) const override;
	private:
		static void ClearErrors(void);
#if BGE_PLATFORM_WIN // Use __stdcall in declaration on Windows.
		static void APIENTRY DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
												  const GLchar *pMessage, const void *pUserParam);
#else
		static void DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
										 const GLchar *pMessage, const void *pUserParam);
#endif /* BGE_PLATFORM_WIN */
	};
} // End namespace (BGE)

#endif /* !_BGE_GL_RENDERER_HPP_ */
