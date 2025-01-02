/*******************************************************************************
 * @file   AudioListener.cpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioListener class implementation.
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
#include "Audio/AL/AudioListener.hpp"

namespace BGE
{
	ALAudioListener::ALAudioListener(void)
		: m_volume(kDEFAULT_VOLUME), m_position(0), m_velocity(0)
	{
	}

	void ALAudioListener::VSetVolume(float volume)
	{
		// TODO: Error check on bounds.
		BGE_ASSERT(volume >= 0.0f);
		m_volume = volume;
		alListenerf(AL_GAIN, volume);
	}

	float ALAudioListener::VGetVolume(void) const
	{
		float volume;
		alGetListenerf(AL_GAIN, &volume);
		return volume;
	}

	void ALAudioListener::VSetPosition(const glm::vec3 &position)
	{
		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void ALAudioListener::VSetVelocity(const glm::vec3 &velocity)
	{
		alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}

	void ALAudioListener::VSetOrientation(const glm::vec3 &forward, const glm::vec3 &up)
	{
		// Orientation is expressed as “at” and “up” vectors
		const std::array<float, 6> orientation = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation.data());
	}
} // End namespace (BGE)
