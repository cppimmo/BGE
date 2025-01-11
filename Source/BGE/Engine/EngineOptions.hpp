#ifndef _BGE_ENGINE_ENGINEOPTIONS_HPP_
#define _BGE_ENGINE_ENGINEOPTIONS_HPP_

#include <cstdint>
#include <string>
#include <optional>

namespace BGE
{
	enum struct RendererImpl; // Forward declare

	//! Struct for containing engine options.
	struct EngineOptions
	{	
		// Optional is used so options can be specified at the command line.
		template <typename Type>
		using Opt = std::optional<Type>;

		Opt<RendererImpl> rendererImpl; //!< Choice of renderer implementation.
		Opt<bool> bRendererDebug; //!< Should the renderer open a debug layer.
		Opt<std::string> windowTitle; //!< Default window title.
		Opt<std::int32_t> windowWidth; //!< Default window width.
		Opt<std::int32_t> windowHeight; //!< Default window height.
		Opt<bool> bWindowResizable; //!< Enable window resizing?
		Opt<bool> bFullscreen; //!< Enable fullscreen?
		Opt<bool> bVSync; //!< Enable V-Sync?
		Opt<std::uint8_t> MSAA; //!< Multisampling 0=OFF up to 4.
		Opt<bool> bImGuiEnabled; //!< Enable ImGui?
		Opt<bool> bLimitFrames; //!< Should the frame rate be controlled?
		Opt<std::int32_t> maxFrames; //!< Maximum frame rate.
		Opt<std::int32_t> masterVolume; //!< Master audio volume [0, 100].
	};
} // End namespace (BGE)

#endif /* !_BGE_ENGINE_ENGINEOPTIONS_HPP_ */
