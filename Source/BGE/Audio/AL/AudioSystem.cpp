/*******************************************************************************
 * @file   AudioSystem.cpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioSystem class implementation.
 *
 * Copyright (c) 2024, Brian Hoffpauir All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 ******************************************************************************/
#include "Engine/EngineStd.hpp"
#include "Audio/AL/AudioSystem.hpp"

#include "Audio/AL/AudioBuffer.hpp"
#include "Audio/AL/AudioSource.hpp"
#include "Audio/AL/AudioListener.hpp"

namespace
{
	void OpenALDebugCallback(ALenum source, ALenum type, ALuint id, ALenum severity,
							 ALsizei length, const ALchar *pMessage, void *pUserParam);
} // End namespace

namespace BGE
{
	ALAudioSystem::ALAudioSystem(void)
		: m_pDevice(nullptr), m_pContext(nullptr), m_bAllPaused(false), m_bInitialized(false)
	{
	}

	ALAudioSystem::~ALAudioSystem(void)
	{
		VShutdown();
	}

	bool ALAudioSystem::VInit(void)
	{
		if (m_bInitialized)
		{
			return true;
		}

		m_pDevice = alcOpenDevice(nullptr);
		if (!m_pDevice)
		{
			BGE_ERROR("Couldn't open OpenAL device");
			return false;
		}

		constexpr std::array<ALCint, (5 * 2)> kATTRIBS =
		{
			ALC_FREQUENCY, 0,
			ALC_MONO_SOURCES, 0,
			ALC_REFRESH, 0,
			ALC_STEREO_SOURCES, 0,
			ALC_SYNC, 0
		};
		m_pContext = alcCreateContext(m_pDevice, nullptr);
		if (!m_pContext || alcMakeContextCurrent(m_pContext) == ALC_FALSE)
		{
			if (m_pContext)
				alcDestroyContext(m_pContext);

			alcCloseDevice(m_pDevice);
			BGE_ERROR("Couldn't set OpenAL context");
			return false;
		}

		// Display log messages
		BGE_LOG("Audio", "OpenAL audio system started successfully");
		std::string_view alVendor = alGetString(AL_VENDOR);
		std::string_view alVersion = alGetString(AL_VERSION);
		std::string_view alRenderer = alGetString(AL_RENDERER);
		std::string_view alExtensions = alGetString(AL_EXTENSIONS);
		BGE_LOG("Audio", "OpenAL vendor: %s", alVendor.data());
		BGE_LOG("Audio", "OpenAL version: %s", alVersion.data());
		BGE_LOG("Audio", "OpenAL renderer: %s", alRenderer.data());
		BGE_LOG("Audio", "OpenAL extensions: %s", alExtensions.data());

		std::string_view alcDefaultDeviceSpecifier = alcGetString(m_pDevice, ALC_DEFAULT_DEVICE_SPECIFIER);
		std::string_view alcCaptureDefaultDeviceSpecifier = alcGetString(m_pDevice, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
		//std::string_view alcDeviceSpecifier = alcGetString(m_pDevice, ALC_DEVICE_SPECIFIER);
		//std::string_view alcCaptureDeviceSpecifier = alcGetString(m_pDevice, ALC_CAPTURE_DEVICE_SPECIFIER);
		//std::span<std::string_view> alcExtensions = alcGetString(m_pDevice, ALC_EXTENSIONS);
		BGE_LOG("Audio", "OpenAL context default device specifier: %s", alcDefaultDeviceSpecifier.data());
		BGE_LOG("Audio", "OpenAL context capture default device specifier: %s", alcCaptureDefaultDeviceSpecifier.data());
		ListAudioDevices(alcGetString(nullptr, ALC_DEVICE_SPECIFIER));
		//BGE_LOG("Audio", "OpenAL context device specifier: %s", alcDeviceSpecifier.data());
		//BGE_LOG("Audio", "OpenAL context capture device specifier: %s", alcCaptureDeviceSpecifier.data());
		//BGE_LOG("Audio", "OpenAL context extensions: %s", alcExtensions.data());

		{
			ALCint majorVersion{}, minorVersion{};
			alcGetIntegerv(m_pDevice, ALC_MAJOR_VERSION, 1, &majorVersion);
			alcGetIntegerv(m_pDevice, ALC_MINOR_VERSION, 1, &minorVersion);
			BGE_LOG("Audio", "OpenAL context version: %d.%d", majorVersion, minorVersion);
		}
		// Attempt to setup OpenAL debug context
		if (alIsExtensionPresent("AL_EXT_debug"))
		{
			BGE_LOG("Audio", "AL_EXT_debug extension is present");
		}

		m_bInitialized = true;
		return true;
	}

	void ALAudioSystem::VShutdown(void)
	{
		if (!m_bInitialized)
		{
			return;
		}

		m_sources.clear(); // Clear the audio source list

		// Attempt to destroy the OpenAL context
		if (m_pContext)
		{
			ALCboolean bResult = alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_pContext);
		}

		// Attempt to close the OpenAL device
		if (m_pDevice)
		{
			ALCboolean bResult = alcCloseDevice(m_pDevice);
		}

		m_bInitialized = false;
	}

	AudioImpl ALAudioSystem::VGetImpl(void) const noexcept
	{
		return AudioImpl::kOpenAL;
	}

	std::size_t ALAudioSystem::VGetActiveBufferCount(void) const
	{
		std::size_t count = 0;
		for (const auto &pkSource : m_sources)
		{
			count += ((pkSource->VGetBuffer() != nullptr) ? 1 : 0);
		}
		return count;
	}

	std::size_t ALAudioSystem::VGetActiveSourceCount(void) const
	{
		return m_sources.size();
	}

	DistanceModel ALAudioSystem::VGetDistanceModel(void) const
	{
		return m_distModel;
	}

	void ALAudioSystem::VSetDistanceModel(DistanceModel model)
	{
		m_distModel = model;
		alDistanceModel(ConvertDistanceModel(model));
	}

	StrongIAudioListenerPtr ALAudioSystem::VCreateListener(void)
	{
		auto pAudioListener = std::make_shared<ALAudioListener>();
		return pAudioListener;
	}

	StrongIAudioSourcePtr ALAudioSystem::VCreateSource(void)
	{
		auto pAudioSource = std::make_shared<ALAudioSource>();
		return pAudioSource;
	}

	StrongIAudioBufferPtr ALAudioSystem::VCreateBuffer(void)
	{
		auto pAudioBuffer = std::make_shared<ALAudioBuffer>();
		return pAudioBuffer;
	}

	void ALAudioSystem::VReleaseListener(StrongIAudioListenerPtr pListener)
	{
		// TODO: Implement.
	}

	void ALAudioSystem::VReleaseSource(StrongIAudioSourcePtr pSource)
	{
		auto it = std::ranges::find(m_sources, pSource);
		// Ensure pSource exists in the list and then remove it.
		if (it != m_sources.end())
		{
			m_sources.remove(pSource);
		}
	}

	void ALAudioSystem::VReleaseBuffer(StrongIAudioBufferPtr pBuffer)
	{
		// TODO: Implement.
	}

	void ALAudioSystem::VSetListener(StrongIAudioListenerPtr pListener)
	{
		m_pActiveListener = pListener;
	}

	StrongIAudioListenerPtr ALAudioSystem::VGetListener(void) const
	{
		return m_pActiveListener;
	}

	void ALAudioSystem::VPauseAll(void)
	{
		// Pause all sources
		for (auto &pSource : m_sources)
		{
			pSource->VPause();
		}
	}

	void ALAudioSystem::VResumeAll(void)
	{
		// Resume all sources
		for (auto &pSource : m_sources)
		{
			pSource->VPlay();
		}
	}

	void ALAudioSystem::VStopAll(void)
	{
		// Stop all sources
		for (auto &pSource : m_sources)
		{
			pSource->VStop();
		}
	}

	bool ALAudioSystem::VIsInitialized(void) const
	{
		return m_bInitialized;
	}

	void ALAudioSystem::ListAudioDevices(const ALchar *pDevices)
	{
		const ALCchar *pDevice = pDevices, *pNext = pDevices + 1;
		size_t length = 0;

		std::ostringstream oss;
		oss << "\n----------\n";
		while (pDevice && *pDevice != '\0' && pNext && *pNext != '\0')
		{
			oss << pDevice << '\n';
			length = strlen(pDevice);
			pDevice += (length + 1);
			pNext += (length + 2);
        }
        oss << "----------";
		BGE_LOG("Audio", "OpenAL context device specifiers: %s", oss.str().c_str());
	}

	ALenum ALAudioSystem::ConvertDistanceModel(DistanceModel model)
	{
		switch (model)
		{
		case DistanceModel::kNone:
			return AL_NONE;
		case DistanceModel::kInverse:
			return AL_INVERSE_DISTANCE;
		case DistanceModel::kInverseClamped:
			return AL_INVERSE_DISTANCE_CLAMPED;
		case DistanceModel::kLinear:
			return AL_LINEAR_DISTANCE;
		case DistanceModel::kLinearClamped:
			return AL_LINEAR_DISTANCE_CLAMPED;
		case DistanceModel::kExponential:
			return AL_EXPONENT_DISTANCE;
		case DistanceModel::kExponentialClamped:
			return AL_EXPONENT_DISTANCE_CLAMPED;
		default:
			return AL_NONE; // Default fallback
		}
	}
} // End namespace (BGE)

namespace
{
#if 0
	void OpenALDebugCallback(ALenum source, ALenum type, ALuint id, ALenum severity,
							 ALsizei length, const ALchar *pMessage, void *pUserParam)
	{
		using namespace std::literals::string_view_literals;
		// Assign strings for enums
		std::string_view sourceName;
		std::string_view typeName;
		std::string_view severityName;
		// Decide source string
		switch (source)
		{
		case AL_DEBUG_SOURCE_API_EXT:
			sourceName = "API_EXT"sv;
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
	// Weird use of preprocessor, please forgive me
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
#endif
} // End namespace
