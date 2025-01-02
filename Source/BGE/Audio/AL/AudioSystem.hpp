/*******************************************************************************
 * @file   AudioSystem.hpp
 * @author Brian Hoffpauir
 * @date   01.01.2025
 * @brief  OpenAL AudioSystem class declaration.
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
#ifndef _BGE_AL_AUDIOSYSTEM_HPP_
#define _BGE_AL_AUDIOSYSTEM_HPP_

// OpenAL headers:
#include <al.h>
#include <alc.h>

#include "Audio/Audio.hpp"

namespace BGE
{
	class OpenALAudioSystem; // Forward declare
	BGE_DECLARE_PTR(OpenALAudioSystem);

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
		DistanceModel m_distModel = DistanceModel::kInverseClamped; //!< Distance model.
	public:
		OpenALAudioSystem(void);
		~OpenALAudioSystem(void) override;
		// IAudioSystem's interface:
		virtual bool VInit(void) override;
		virtual void VShutdown(void) override;
		virtual AudioImpl VGetImpl(void) const noexcept override;
		virtual std::size_t VGetActiveBufferCount(void) const override;
		virtual std::size_t VGetActiveSourceCount(void) const override;
		virtual DistanceModel VGetDistanceModel(void) const override;
		virtual void VSetDistanceModel(DistanceModel model) override;

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
		static ALenum ConvertDistanceModel(DistanceModel model);
	};
} // End namespace (BGE)

#endif /* !_BGE_AL_AUDIOSYSTEM_HPP_ */
