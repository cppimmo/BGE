/*******************************************************************************
 * @file   AudioSource.cpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioSource class implementation.
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
#include "Audio/AL/AudioSource.hpp"

namespace BGE
{
	ALAudioSource::ALAudioSource(void)
		: m_sourceID(0), m_pBuffer(nullptr), m_position(0), m_velocity(0)
	{
		alGenSources(1, &m_sourceID);
		VSetPosition(m_position);
		VSetVelocity(m_velocity);
	}

	ALAudioSource::~ALAudioSource(void)
	{
		alDeleteSources(1, &m_sourceID);
	}

	void ALAudioSource::VSetPosition(const glm::vec3 &kPosition)
	{
		m_position = kPosition;
		alSource3f(m_sourceID, AL_POSITION, kPosition.x, kPosition.y, kPosition.z);
	}

	const glm::vec3 &ALAudioSource::VGetPosition(void) const
	{
		return m_position;
	}

	void ALAudioSource::VSetVelocity(const glm::vec3 &kVelocity)
	{
		m_velocity = kVelocity;
		alSource3f(m_sourceID, AL_VELOCITY, kVelocity.x, kVelocity.y, kVelocity.z);
	}

	const glm::vec3 &ALAudioSource::VGetVelocity(void) const
	{
		return m_velocity;
	}

	void ALAudioSource::VSetVolume(float volume)
	{
		alSourcef(m_sourceID, AL_GAIN, volume);
	}

	float ALAudioSource::VGetVolume(void) const
	{
		float volume;
		alGetSourcef(m_sourceID, AL_GAIN, &volume);
		return volume;
	}

	void ALAudioSource::VSetLooping(bool bLooping)
	{
		alSourcei(m_sourceID, AL_LOOPING, (bLooping ? AL_TRUE : AL_FALSE));
	}

	bool ALAudioSource::VIsLooping(void) const
	{
		int looping;
		alGetSourcei(m_sourceID, AL_LOOPING, &looping);
		return (looping == AL_TRUE);
	}

	void ALAudioSource::VPlay(void)
	{
		if (!VIsPlaying())
		{
			alSourcePlay(m_sourceID);
		}
	}

	void ALAudioSource::VPause(void)
	{
		alSourcePause(m_sourceID);
	}

	void ALAudioSource::VStop(void)
	{
		alSourceStop(m_sourceID);
	}

	bool ALAudioSource::VIsPlaying(void) const
	{
		int state;
		alGetSourcei(m_sourceID, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	float ALAudioSource::VGetProgress(void) const
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

	void ALAudioSource::VAttachBuffer(StrongIAudioBufferPtr pBuffer)
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

	StrongIAudioBufferPtr ALAudioSource::VGetBuffer(void) const
	{
		return m_pBuffer;
	}

	void ALAudioSource::VDetachBuffer(void)
	{
		m_pBuffer = nullptr;
		alSourcei(m_sourceID, AL_BUFFER, 0);
	}
} // End namespace (BGE)
