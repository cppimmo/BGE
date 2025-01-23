#include "Engine/EngineStd.hpp"
#include "Graphics/GL/Renderer.hpp"

#include <iomanip>

#include "Graphics/Renderer.hpp"
#include "Graphics/GL/Shaders.hpp"
#include "Graphics/GL/ShaderProgram.hpp"

#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

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
		m_options = kOptions; // Set the options

#ifdef BGE_CONFIG_DEBUG
		// Setup debug context
		if (*kOptions.bRendererDebug)
		{
			glDebugMessageCallback(DebugMessageCallback, nullptr);

			// Listen to everything
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

			// However, disable notification severity
			glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);

			// This isn't needed if an "official context has already been hinted by the windowing sytem
			glEnable(GL_DEBUG_OUTPUT);
		}
#endif /* BGE_CONFIG_DEBUG */

		// Intialize ImGui & ImPlot
		IMGUI_CHECKVERSION(); // What does this do?

		// Create ImGui context
		if (!(m_pImGuiContext = ImGui::CreateContext()))
		{
			BGE_ERROR("OpenGL: Couldn't create ImGui context!");
			return false;
		}

		// Create ImPlot context
		if (!(m_pImPlotContext = ImPlot::CreateContext()))
		{
			BGE_ERROR("OpenGL: Couldn't create ImPlot context!");
			return false;
		}

		BGUTSetImGuiContextPtrs(m_pImGuiContext, m_pImPlotContext);

		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable keyboard controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable gamepad controls
		//io.IniFilename = nullptr; // TODO: Set ImGui config filename/location.

		// Set ImGui style colors
		ImGui::StyleColorsDark();

		auto pWindow = BGUTGetWindowPtr();
		// Setup platform/renderer backends
		if (!ImGui_ImplSDL3_InitForOpenGL(pWindow, SDL_GL_GetCurrentContext()))
		{
			BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize SDL2 implementation!");
			return false;
		}

		// TODO: Define the GLSL version string elsewhere.
		constexpr std::string_view kGLSL_VERSION = "#version 330";
		if (!ImGui_ImplOpenGL3_Init(kGLSL_VERSION.data()))
		{
			BGE_ERROR("BGUTInitImGui Failure: Couldn't initialize OpenGL3 implementation!");
			return false;
		}
		
		BGE_LOG("OpenGL", "ImGui Version: %s", ImGui::GetVersion());
#ifdef BGE_CONFIG_DEBUG
		BGE_LOG("OpenGL", "\n\n%s", VGetRendererInfo().c_str());
#endif /* BGE_CONFIG_DEBUG */

		int width = 0, height = 0;
		BGUTSetWindowSize(BGUTGetWindowPtr(), width, height);
		
		// Set the default viewport
		VSetViewport(GLViewport(glm::ivec2(width, height)));

		m_bInitialized = true;
		return true;
	}

	void GLRenderer::VShutdown(void)
	{
		if (!m_bInitialized)
		{
			return;
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImPlot::DestroyContext(); // Destroy ImPlot context first
		ImGui::DestroyContext();

		m_bInitialized = false;
	}

	RendererImpl GLRenderer::VGetImpl(void) const
	{
		return RendererImpl::kOpenGL;
	}

	void GLRenderer::VBeginFrame(void)
	{
		// When ImGui is enabled prepare the new frame
		if (*m_options.bImGuiEnabled)
		{
			ImGui::SetCurrentContext(m_pImGuiContext);
			ImPlot::SetCurrentContext(m_pImPlotContext);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearBufferfv(GL_COLOR, 0, &m_bgColor[0]);
	}

	void GLRenderer::VEndFrame(void)
	{
		// When ImGui is enabled call end of frame routines
		if (*m_options.bImGuiEnabled)
		{
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}
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

	void GLRenderer::VEnableDepthTest(bool bEnable)
	{
	}

	void GLRenderer::VEnableBlending(bool bEnable)
	{
	}

	void GLRenderer::VSetBlendMode(BlendMode mode)
	{
	}

	BlendMode GLRenderer::VGetBlendMode(void) const
	{
		return BlendMode::kAdditive;
	}

	bool GLRenderer::VTakeScreenshot(const std::filesystem::path &kSaveGameDir)
	{
		auto pWindow = BGUTGetWindowPtr();
		if (!pWindow)
		{
			BGE_ERROR("TakeScreenshot Failure: Window pointer could not be retrieved.");
			return false;
		}

		int width = 0, height = 0; // Retrieve window width & height
		SDL_GetWindowSize(pWindow, &width, &height);

		std::uint32_t rMask, gMask, bMask;
		const std::uint32_t aMask = 0x000000;
#if SDL_BTYEORDER == SDL_BIG_ENDIAN
		rMask = 0xFF0000;
		gMask = 0x00FF00;
		bMask = 0x0000FF;
#else
		rMask = 0x0000FF;
		gMask = 0x00FF00;
		bMask = 0xFF0000;
#endif
		constexpr std::uint32_t kColorBytes = 4u;
		constexpr std::uint32_t kBitsPerPixel = 32u;

		auto createSurface = [&](void) -> auto
		{
			return SDL_CreateSurface(width, height, SDL_GetPixelFormatForMasks(kBitsPerPixel, rMask, gMask, bMask, aMask));
		};

		SDL_Surface *pTemp = createSurface();
		SDL_Surface *pImage = createSurface();
		if (!pTemp || !pImage)
		{
			BGE_ERROR("OpenGL: Could not create surface (%s).", SDL_GetError());
			SDL_free(pTemp); SDL_free(pImage);
			return false;
		}

		// Read the entire screen:
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pImage->pixels);
		for (std::size_t index = 0; index < height; ++index)
		{
			const auto kWidthPerColorBytes = (kColorBytes * width);
			std::memcpy(static_cast<char *>(pTemp->pixels) + kWidthPerColorBytes * index,
						static_cast<char *>(pImage->pixels) + kWidthPerColorBytes * (height - index),
						kWidthPerColorBytes);
		}

		std::memcpy(pImage->pixels, pTemp->pixels, width * height * kColorBytes);
		
		// Write image to file on disk (do not return on failure, so surfaces can be freed):
		BGE_LOG("OpenGL", "Screenshot filename: %s", GetScreenshotFilename(kSaveGameDir.string()).c_str());
		BGE_LOG_IF(!SDL_SaveBMP(pTemp, GetScreenshotFilename(kSaveGameDir.string()).c_str()),
				   "OpenGL", "Could not save file(% s).", SDL_GetError());
		
		// Free surfaces:
		SDL_DestroySurface(pTemp);
		SDL_DestroySurface(pImage);

		return true;
	}

	void GLRenderer::VEnableDebugOutput(bool bEnable)
	{
	}

	std::string GLRenderer::VGetRendererInfo(void) const
	{
		std::ostringstream oss;

		// Output OpenGL context information
		oss << "GL_VENDOR: " << glGetString(GL_VENDOR) << '\n';
		oss << "GL_VERSION: " << glGetString(GL_RENDERER) << '\n';
		oss << "GL_SHADING_LANGUAGE: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << '\n';

		// Output SDL OpenGL attibute values
		auto logAttrib = [&oss](SDL_GLAttr attrib) -> void
		{
			int attribValue = 0;
			SDL_GL_GetAttribute(attrib, &attribValue);
			
			oss << "SDL_GLattr("
				<< std::setfill('0') << std::setw(2) << std::dec << static_cast<int>(attrib)
				<< "): DEC: "
				<< std::setfill(' ') << std::setw(10) << std::dec << attribValue
				<< ", HEX: 0x"
				<< std::setfill('0') << std::setw(2 * sizeof(attribValue)) << std::hex << attribValue << '\n';
		};

		logAttrib(SDL_GL_RED_SIZE);
		logAttrib(SDL_GL_GREEN_SIZE);
		logAttrib(SDL_GL_BLUE_SIZE);
		logAttrib(SDL_GL_ALPHA_SIZE);
		logAttrib(SDL_GL_BUFFER_SIZE);
		logAttrib(SDL_GL_DOUBLEBUFFER);
		logAttrib(SDL_GL_DEPTH_SIZE);
		
		// Reading these attributes cause segfaults
		//logAttrib(SDL_GL_STENCIL_SIZE);
		//logAttrib(SDL_GL_ACCUM_RED_SIZE);
		//logAttrib(SDL_GL_ACCUM_GREEN_SIZE);
		//logAttrib(SDL_GL_ACCUM_BLUE_SIZE);
		//logAttrib(SDL_GL_ACCUM_ALPHA_SIZE);
		
		logAttrib(SDL_GL_STEREO);
		logAttrib(SDL_GL_MULTISAMPLEBUFFERS);
		logAttrib(SDL_GL_MULTISAMPLESAMPLES);
		logAttrib(SDL_GL_ACCELERATED_VISUAL);
		logAttrib(SDL_GL_RETAINED_BACKING);
		logAttrib(SDL_GL_CONTEXT_MAJOR_VERSION);
		logAttrib(SDL_GL_CONTEXT_MINOR_VERSION);
		//logAttrib(SDL_GL_CONTEXT_EGL);
		logAttrib(SDL_GL_CONTEXT_FLAGS);
		logAttrib(SDL_GL_CONTEXT_PROFILE_MASK);
		logAttrib(SDL_GL_SHARE_WITH_CURRENT_CONTEXT);
		logAttrib(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE);
		logAttrib(SDL_GL_CONTEXT_RELEASE_BEHAVIOR);
		logAttrib(SDL_GL_CONTEXT_RESET_NOTIFICATION);
		logAttrib(SDL_GL_CONTEXT_NO_ERROR);
		logAttrib(SDL_GL_FLOATBUFFERS);

		return oss.str(); // Return the stream as a string
	}

	void GLRenderer::ClearErrors(void)
	{
		GLenum error;

		// Pop all errors off the internal stack
		while ((error = glGetError()) != GL_NO_ERROR);
	}

	void GLRenderer::DebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
										  const GLchar *pMessage, const void *pUserParam)
	{
		using namespace std::literals::string_view_literals;

		// Assign strings for enums
		std::string_view sourceName;
		std::string_view typeName;
		std::string_view severityName;
		
		// Decide source string
		switch (source)
		{
		case GL_DEBUG_SOURCE_API:
			sourceName = "API"sv;
			break;
		case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
			sourceName = "WINDOW SYS"sv;
			break;
		case GL_DEBUG_SOURCE_SHADER_COMPILER:
			sourceName = "SHADER COMP"sv;
			break;
		case GL_DEBUG_SOURCE_THIRD_PARTY:
			sourceName = "3RD PARTY"sv;
			break;
		case GL_DEBUG_SOURCE_APPLICATION:
			sourceName = "APP"sv;
			break;
			// Supply for SOURCE_OTHER and default case:
		case GL_DEBUG_SOURCE_OTHER:
		default:
			sourceName = "OTHER"sv;
			break;
		}
		
		// Decide type string
		switch (type)
		{
		case GL_DEBUG_TYPE_ERROR:
			typeName = "ERROR"sv;
			break;
		case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
			typeName = "DB"sv;
			break;
		case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
			typeName = "UB"sv;
			break;
		case GL_DEBUG_TYPE_PERFORMANCE:
			typeName = "PERFORMANCE"sv;
			break;
		case GL_DEBUG_TYPE_PORTABILITY:
			typeName = "PORTABILITY"sv;
			break;
		case GL_DEBUG_TYPE_MARKER:
			typeName = "MARKER"sv;
			break;
		case GL_DEBUG_TYPE_PUSH_GROUP:
			typeName = "PUSH GROUP"sv;
			break;
		case GL_DEBUG_TYPE_POP_GROUP:
			typeName = "POP GROUP"sv;
			break;
			// Supply for TYPE_OTHER and default case:
		case GL_DEBUG_TYPE_OTHER:
		default:
			typeName = "OTHER"sv;
			break;
		}

		// Log format string
		static constexpr std::string_view c_kGL_DEBUG_FMT = "GL_DEBUG(ID:0x%08X)|%s|%s|%s: %s";
		// Macro for arguments to be used with the above format string
#define BGE_GL_DEBUG_ARGS id, sourceName.data(), typeName.data(), severityName.data(), pMessage
		
		// Decide severity string & how message will be output
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:
			severityName = "HIGH";
			BGE_ERROR(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
			break;
		case GL_DEBUG_SEVERITY_MEDIUM:
			severityName = "MEDIUM";
			BGE_WARNING(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
			break;
		case GL_DEBUG_SEVERITY_LOW:
			severityName = "LOW";
			BGE_INFO(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
			break;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			severityName = "NOTIF";
			BGE_INFO(c_kGL_DEBUG_FMT.data(), BGE_GL_DEBUG_ARGS);
			break;
		}
	}
} // End namespace (BGE)
