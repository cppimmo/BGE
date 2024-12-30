#include "Engine/EngineStd.hpp"
#include "Audio/OpenALAudio.hpp"

#include "Audio/SoundResource.hpp"

namespace BGE
{
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
		auto pSoundExtraData = std::dynamic_pointer_cast<SoundExtraData>(pHandle->GetExtraData());
		const SoundExtraData::SoundData &soundData = pSoundExtraData->GetSoundData();

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

	bool OpenALAudioSystem::VInitialize(void)
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

		m_pContext = alcCreateContext(m_pDevice, nullptr);
		if (!m_pContext || alcMakeContextCurrent(m_pContext) == ALC_FALSE)
		{
			if (m_pContext)
				alcDestroyContext(m_pContext);

			alcCloseDevice(m_pDevice);
			BGE_ERROR("Couldn't set OpenAL context");
			return false;
		}
		BGE_LOG("Audio", "OpenAL audio system started successfully");
		m_bInitialized = true;
		return true;
	}

	void OpenALAudioSystem::VShutdown(void)
	{
		if (m_pContext)
		{
			alcMakeContextCurrent(nullptr);
			alcDestroyContext(m_pContext);
		}

		if (m_pDevice)
		{
			alcCloseDevice(m_pDevice);
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
		// TODO: Implement.
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
		// TODO: Implement.
	}

	void OpenALAudioSystem::VResumeAll(void)
	{
		// TODO: Implement.
	}

	void OpenALAudioSystem::VStopAll(void)
	{
		// TODO: Implement.
	}

	bool OpenALAudioSystem::VIsInitialized(void) const
	{
		return m_bInitialized;
	}
} // End namespace (BGE)
