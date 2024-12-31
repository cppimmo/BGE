/*******************************************************************************
 * @file   OpenALAudio.hpp
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
#ifndef _BGE_OPENALAUDIO_HPP_
#define _BGE_OPENALAUDIO_HPP_

#include "Audio/Audio.hpp"

// OpenAL headers:
#include <al.h>
#include <alc.h>

namespace BGE
{
	class OpenALAudioListener; // Forward declare
	class OpenALAudioSource; // Forward declare
	class OpenALAudioBuffer; // Forward declare
	class OpenALAudioSystem; // Forward declare
	BGE_DECLARE_PTR(OpenALAudioListener);
	BGE_DECLARE_PTR(OpenALAudioSource);
	BGE_DECLARE_PTR(OpenALAudioBuffer);
	BGE_DECLARE_PTR(OpenALAudioSystem);

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

	/**
	 * @brief .
	 */
	class OpenALAudioSource final : public IAudioSource
	{
	private:
		ALuint m_sourceID; //!< OpenAL source identifier.
		StrongIAudioBufferPtr m_pBuffer; //!< Audio buffer managed by this source.
	public:
		OpenALAudioSource(void);
		~OpenALAudioSource(void) override;
		// IAudioSource's interface:
		virtual void VSetPosition(const glm::vec3 &position) override;
		virtual void VSetVelocity(const glm::vec3 &velocity) override;
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

	class OpenALAudioBuffer final : public IAudioBuffer
	{
	private:
		ALuint m_bufferID; //!< OpenAL buffer identifier.
		bool m_bInitialized = false; //!< Has the buffer been initialized?
	public:
		OpenALAudioBuffer(void);
		~OpenALAudioBuffer(void) override;
		// IAudioBuffer's interface:
		virtual bool VLoadFromResource(StrongResourceHandlePtr pHandle) override;
		virtual StrongResourceHandlePtr VGetResource(void) const override;
		virtual void *VGet(void) override;
		virtual bool VIsLoaded(void) const override;
	};

	/**
	 * @brief .
	 */
	class OpenALAudioSystem final : public IAudioSystem
	{
	private:
		ALCdevice *m_pDevice; //!< OpenAL device.
		ALCcontext *m_pContext; //!< OpenAL context.
		bool m_bAllPaused; //!< Are all sources paused?
		bool m_bInitialized; //!< Is the audio system initialized?
		StrongIAudioListenerPtr m_pActiveListener; //!< The active audio listener (of which there can only be one).
		AudioSourceList m_sources; //!< List of managed audio sources.
	public:
		OpenALAudioSystem(void);
		~OpenALAudioSystem(void) override;
		// IAudioSystem's interface:
		virtual bool VInit(void) override;
		virtual void VShutdown(void) override;

		virtual StrongIAudioListenerPtr VCreateListener(void) override;
		virtual StrongIAudioSourcePtr VCreateSource(void) override;
		virtual StrongIAudioBufferPtr VCreateBuffer(void) override;

		virtual void VReleaseListener(StrongIAudioListenerPtr pListener) override;
		virtual void VReleaseSource(StrongIAudioSourcePtr pSource) override;
		virtual void VReleaseBuffer(StrongIAudioBufferPtr pBuffer) override;

		virtual void VSetListener(StrongIAudioListenerPtr pListener) override;
		virtual StrongIAudioListenerPtr VGetListener(void) const override;

		virtual void VPauseAll(void) override;
		virtual void VResumeAll(void) override;
		virtual void VStopAll(void) override;
		virtual bool VIsInitialized(void) const override;
	private:
		static void ListAudioDevices(const ALchar *pDevices);
	};
} // End namespace (BGE)

#endif /* !_BGE_OPENALAUDIO_HPP_ */
