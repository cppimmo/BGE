/*******************************************************************************
 * @file   AudioSource.hpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioSource class declaration.
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
#ifndef _BGE_AL_AUDIOSOURCE_HPP_
#define _BGE_AL_AUDIOSOURCE_HPP_

// OpenAL headers:
#include <al.h>
#include <alc.h>

#include "Audio/Audio.hpp"

namespace BGE
{
	class ALAudioSource; // Forward declare
	BGE_DECLARE_PTR(ALAudioSource);

	/**
	 * @brief .
	 */
	class ALAudioSource final : public IAudioSource
	{
	private:
		ALuint m_sourceID; //!< OpenAL source identifier.
		StrongIAudioBufferPtr m_pBuffer; //!< Audio buffer managed by this source.
		glm::vec3 m_position;
		glm::vec3 m_velocity;
	public:
		ALAudioSource(void);
		~ALAudioSource(void) override;
		// IAudioSource's interface:
		virtual void VSetPosition(const glm::vec3 &kPosition) override;
		virtual const glm::vec3 &VGetPosition(void) const override;
		virtual void VSetVelocity(const glm::vec3 &kVelocity) override;
		virtual const glm::vec3 &VGetVelocity(void) const override;
		virtual void VSetVolume(float volume) override;
		virtual float VGetVolume(void) const override;

		virtual void VSetLooping(bool bLooping) override;
		virtual bool VIsLooping(void) const override;

		virtual void VPlay(void) override;
		virtual void VPause(void) override;
		virtual void VStop(void) override;
		virtual bool VIsPlaying(void) const override;
		virtual float VGetProgress(void) const override;

		virtual void VAttachBuffer(StrongIAudioBufferPtr pBuffer) override;
		virtual StrongIAudioBufferPtr VGetBuffer(void) const override;
		virtual void VDetachBuffer(void) override;
	};
} // End namespace (BGE)

#endif /* !_BGE_AL_AUDIOSOURCE_HPP_ */
