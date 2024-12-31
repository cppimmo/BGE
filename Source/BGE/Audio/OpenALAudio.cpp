/*******************************************************************************
 * @file   OpenALAudio.cpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  OpenAL audio subsystem implementation.
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
#include "Audio/OpenALAudio.hpp"

#include "Audio/SoundResource.hpp"

namespace
{
	void OpenALDebugCallback(ALenum source, ALenum type, ALuint id, ALenum severity,
							 ALsizei length, const ALchar *pMessage, void *pUserParam);
} // End namespace

namespace BGE
{
	OpenALAudioListener::OpenALAudioListener(void)
		: m_volume(kDEFAULT_VOLUME), m_position(0), m_velocity(0)
	{
	}

	void OpenALAudioListener::VSetVolume(float volume)
	{
		// TODO: Error check on bounds.
		BGE_ASSERT(volume >= 0.0f);
		m_volume = volume;
		alListenerf(AL_GAIN, volume);
	}

	float OpenALAudioListener::VGetVolume(void) const
	{
		float volume;
		alGetListenerf(AL_GAIN, &volume);
		return volume;
	}

	void OpenALAudioListener::VSetPosition(const glm::vec3 &position)
	{
		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void OpenALAudioListener::VSetVelocity(const glm::vec3 &velocity)
	{
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void OpenALAudioListener::VSetOrientation(const glm::vec3 &forward, const glm::vec3 &up)
	{
		// Orientation is expressed as “at” and “up” vectors
		const std::array<float, 6> orientation = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation.data());
	}

	OpenALAudioSource::OpenALAudioSource(void)
	{
		alGenSources(1, &m_sourceID);
	}

	OpenALAudioSource::~OpenALAudioSource(void)
	{
		alDeleteSources(1, &m_sourceID);
	}

	void OpenALAudioSource::VSetPosition(const glm::vec3 &position)
	{
		alSource3f(m_sourceID, AL_POSITION, position.x, position.y, position.z);
	}

	void OpenALAudioSource::VSetVelocity(const glm::vec3 &velocity)
	{
		alSource3f(m_sourceID, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void OpenALAudioSource::VSetVolume(float volume)
	{
		alSourcef(m_sourceID, AL_GAIN, volume);
	}

	float OpenALAudioSource::VGetVolume(void) const
	{
		float volume;
		alGetSourcef(m_sourceID, AL_GAIN, &volume);
		return volume;
	}

	void OpenALAudioSource::VSetLooping(bool bLooping)
	{
		alSourcei(m_sourceID, AL_LOOPING, (bLooping ? AL_TRUE : AL_FALSE));
	}

	bool OpenALAudioSource::VIsLooping(void) const
	{
		int looping;
		alGetSourcei(m_sourceID, AL_LOOPING, &looping);
		return (looping == AL_TRUE);
	}

	void OpenALAudioSource::VPlay(void)
	{
		if (!VIsPlaying())
		{
			alSourcePlay(m_sourceID);
		}
	}

	void OpenALAudioSource::VPause(void)
	{
		alSourcePause(m_sourceID);
	}

	void OpenALAudioSource::VStop(void)
	{
		alSourceStop(m_sourceID);
	}

	bool OpenALAudioSource::VIsPlaying(void) const
	{
		int state;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	float OpenALAudioSource::VGetProgress(void) const
	{
		ALint byteOffset = 0;
		ALint bufferID = 0;

		// TODO: Use IAudioBuffer::VGet() instead.
		// Get the currently bound buffer
		alGetSourcei(m_sourceID, AL_BUFFER, &bufferID);
		if (bufferID == 0)
		{
			return 0.0f; // No buffer is attached
		}

		// Query the byte offset of the source
		alGetSourcei(m_sourceID, AL_BYTE_OFFSET, &byteOffset);

		// Query buffer properties
		ALint size = 0, freq = 0, bits = 0, channels = 0;
		alGetBufferi(bufferID, AL_SIZE, &size);
		alGetBufferi(bufferID, AL_FREQUENCY, &freq);
		alGetBufferi(bufferID, AL_BITS, &bits);
		alGetBufferi(bufferID, AL_CHANNELS, &channels);

		if (freq == 0 || bits == 0 || channels == 0)
		{
			return 0.0f; // Invalid buffer properties
		}

		// Calculate the total duration of the buffer in seconds
		float totalDuration = static_cast<float>(size) / (freq * channels * (bits / 8.0f));

		// Calculate the current playback position in seconds
		float currentPosition = static_cast<float>(byteOffset) / (freq * channels * (bits / 8.0f));

		// Calculate normalized progress in range [0.0, 1.0]
		if (totalDuration > 0.0f)
		{
			return currentPosition / totalDuration;
		}

		return 0.0f; // Avoid division by zero
	}

	void OpenALAudioSource::VAttachBuffer(StrongIAudioBufferPtr pBuffer)
	{
		if (!pBuffer)
		{
			BGE_ERROR("Attempting to attach a null buffer");
			return;
		}

		m_pBuffer = pBuffer;
		// Safely obtain the buffer ID via VGet().
		void *pRawBufferID = pBuffer->VGet();
		if (!pRawBufferID)
		{
			BGE_ERROR("Buffer VGet() returned nullptr");
			return;
		}
		// OpenAL buffer ID is accessible via `VGet()` in `OpenALAudioBuffer`.
		alSourcei(m_sourceID, AL_BUFFER, *static_cast<ALuint *>(pRawBufferID));
	}

	StrongIAudioBufferPtr OpenALAudioSource::VGetBuffer(void) const
	{
		return m_pBuffer;
	}

	void OpenALAudioSource::VDetachBuffer(void)
	{
		m_pBuffer = nullptr;
		alSourcei(m_sourceID, AL_BUFFER, 0);
	}

	OpenALAudioBuffer::OpenALAudioBuffer(void)
	{
		alGenBuffers(1, &m_bufferID);
	}

	OpenALAudioBuffer::~OpenALAudioBuffer(void)
	{
		alDeleteBuffers(1, &m_bufferID);
	}

	bool OpenALAudioBuffer::VLoadFromResource(StrongResourceHandlePtr pHandle)
	{
		auto pExtraData = std::dynamic_pointer_cast<SoundResourceExtraData>(pHandle->GetExtraData());
		const SoundData &soundData = pExtraData->GetSoundData();

		ALenum format;
		const bool bSingleChannel = soundData.channels == 1;
		const bool bDualChannel = soundData.channels == 2;
		if (bSingleChannel && soundData.bitsPerSample == 8)
		{
			format = AL_FORMAT_MONO8;
		}
		else if (bSingleChannel && soundData.bitsPerSample == 16)
		{
			format = AL_FORMAT_MONO16;
		}
		else if (bDualChannel && soundData.bitsPerSample == 8)
		{
			format = AL_FORMAT_STEREO8;
		}
		else if (bDualChannel && soundData.bitsPerSample == 16)
		{
			format = AL_FORMAT_STEREO16;
		}
		else
		{
			BGE_ERROR("Unsupported channels/bitrate!");
			return false;
		}

		alBufferData(m_bufferID, format, soundData.soundData.data(), soundData.soundData.size(), soundData.sampleRate);
		m_bInitialized = true;
		return true;
	}

	StrongResourceHandlePtr OpenALAudioBuffer::VGetResource(void) const
	{
		return nullptr;
	}

	void *OpenALAudioBuffer::VGet(void)
	{
		return reinterpret_cast<void *>(&m_bufferID);
	}

	bool OpenALAudioBuffer::VIsLoaded(void) const
	{
		return m_bInitialized;
	}

	OpenALAudioSystem::OpenALAudioSystem(void)
		: m_pDevice(nullptr), m_pContext(nullptr), m_bAllPaused(false), m_bInitialized(false)
	{
	}

	OpenALAudioSystem::~OpenALAudioSystem(void)
	{
		if (m_bInitialized)
		{
			VShutdown();
		}
	}

	bool OpenALAudioSystem::VInit(void)
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

	void OpenALAudioSystem::VShutdown(void)
	{
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

	StrongIAudioListenerPtr OpenALAudioSystem::VCreateListener(void)
	{
		auto pAudioListener = std::make_shared<OpenALAudioListener>();
		return pAudioListener;
	}

	StrongIAudioSourcePtr OpenALAudioSystem::VCreateSource(void)
	{
		auto pAudioSource = std::make_shared<OpenALAudioSource>();
		return pAudioSource;
	}

	StrongIAudioBufferPtr OpenALAudioSystem::VCreateBuffer(void)
	{
		auto pAudioBuffer = std::make_shared<OpenALAudioBuffer>();
		return pAudioBuffer;
	}

	void OpenALAudioSystem::VReleaseListener(StrongIAudioListenerPtr pListener)
	{
		// TODO: Implement.
	}

	void OpenALAudioSystem::VReleaseSource(StrongIAudioSourcePtr pSource)
	{
		auto it = std::ranges::find(m_sources, pSource);
		// Ensure pSource exists in the list and then remove it.
		if (it != m_sources.end())
		{
			m_sources.remove(pSource);
		}
	}

	void OpenALAudioSystem::VReleaseBuffer(StrongIAudioBufferPtr pBuffer)
	{
		// TODO: Implement.
	}

	void OpenALAudioSystem::VSetListener(StrongIAudioListenerPtr pListener)
	{
		m_pActiveListener = pListener;
	}

	StrongIAudioListenerPtr OpenALAudioSystem::VGetListener(void) const
	{
		return m_pActiveListener;
	}

	void OpenALAudioSystem::VPauseAll(void)
	{
		// Pause all sources
		for (auto &pSource : m_sources)
		{
			pSource->VPause();
		}
	}

	void OpenALAudioSystem::VResumeAll(void)
	{
		// Resume all sources
		for (auto &pSource : m_sources)
		{
			pSource->VPlay();
		}
	}

	void OpenALAudioSystem::VStopAll(void)
	{
		// Stop all sources
		for (auto &pSource : m_sources)
		{
			pSource->VStop();
		}
	}

	bool OpenALAudioSystem::VIsInitialized(void) const
	{
		return m_bInitialized;
	}

	void OpenALAudioSystem::ListAudioDevices(const ALchar *pDevices)
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
