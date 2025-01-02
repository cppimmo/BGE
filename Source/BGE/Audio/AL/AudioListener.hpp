/*******************************************************************************
 * @file   AudioListener.hpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioListener class declaration.
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
#ifndef _BGE_AL_AUDIOLISTENER_HPP_
#define _BGE_AL_AUDIOLISTENER_HPP_

// OpenAL headers:
#include <al.h>
#include <alc.h>

#include "Audio/Audio.hpp"

namespace BGE
{
	class OpenALAudioListener; // Forward declare
	BGE_DECLARE_PTR(OpenALAudioListener);

	/**
	 * @brief .
	 */
	class OpenALAudioListener final : public IAudioListener
	{
	private:
		static constexpr float kDEFAULT_VOLUME = 1.0f; //!< Default listener volume.

		float m_volume; //!< Master volume of the listener.
		glm::vec3 m_position; //!< 3D position of the listener.
		glm::vec3 m_velocity; //!< Velocity of the listener.
	public:
		OpenALAudioListener(void);
		~OpenALAudioListener(void) override = default;
		// IAudioListener's interface:
		virtual void VSetVolume(float volume) override;
		virtual float VGetVolume(void) const override;
		virtual void VSetPosition(const glm::vec3 &position) override;
		virtual void VSetVelocity(const glm::vec3 &velocity) override;
		virtual void VSetOrientation(const glm::vec3 &forward, const glm::vec3 &up) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_AL_AUDIOLISTENER_HPP_ */
