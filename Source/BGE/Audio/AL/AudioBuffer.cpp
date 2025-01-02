/*******************************************************************************
 * @file   AudioBuffer.cpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioBuffer class implementation.
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
#include "Audio/AL/AudioBuffer.hpp"

#include "Audio/SoundResource.hpp"

namespace BGE
{
	ALAudioBuffer::ALAudioBuffer(void)
	{
		alGenBuffers(1, &m_bufferID);
	}

	ALAudioBuffer::~ALAudioBuffer(void)
	{
		alDeleteBuffers(1, &m_bufferID);
	}

	bool ALAudioBuffer::VLoadFromResource(StrongResourceHandlePtr pHandle)
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

	StrongResourceHandlePtr ALAudioBuffer::VGetResource(void) const
	{
		return nullptr;
	}

	void *ALAudioBuffer::VGet(void)
	{
		return reinterpret_cast<void *>(&m_bufferID);
	}

	int ALAudioBuffer::VGetFrequency(void) const
	{
		ALint frequency{};
		alGetBufferi(m_bufferID, AL_FREQUENCY, &frequency);
		return frequency;
	}

	int ALAudioBuffer::VGetBitDepth(void) const
	{
		ALint bitDepth{};
		alGetBufferi(m_bufferID, AL_BITS, &bitDepth);
		return bitDepth;
	}

	int ALAudioBuffer::VGetChannels(void) const
	{
		ALint channels{};
		alGetBufferi(m_bufferID, AL_CHANNELS, &channels);
		return channels;
	}

	int ALAudioBuffer::VGetSize(void) const
	{
		ALint size{};
		alGetBufferi(m_bufferID, AL_SIZE, &size);
		return size;
	}

	bool ALAudioBuffer::VIsLoaded(void) const
	{
		return m_bInitialized;
	}
} // End namespace (BEG)
