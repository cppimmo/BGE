/*=============================================================================*
 * AudioSystem.hpp - Audio utility system.
 *
 * Copyright (c) 2023, Brian Hoffpauir All rights reserved.
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
 *============================================================================*/
#ifndef _BGE_AUDIOSYSTEM_HPP_
#define _BGE_AUDIOSYSTEM_HPP_

//#include "Engine/EngineStd.hpp"

#include "Audio/AudioBuffer.hpp"
#include "Audio/AudioSource.hpp"
#include "Audio/AudioListener.hpp"

#include <list>
// OpenAL headers:
#include <AL/al.h>
#include <AL/alc.h>

namespace BGE
{
	class IAudioSystem;
	BGE_DECLARE_PTR(IAudioSystem);
	class AudioSystem;
	BGE_DECLARE_PTR(AudioSystem);
	class IAudioListener;
	BGE_DECLARE_PTR(IAudioListener);
	class IAudioSource;
	BGE_DECLARE_PTR(IAudioSource);
	class IAudioBuffer;
	BGE_DECLARE_PTR(IAudioBuffer);

	class IAudioListener
	{
	public:
	};
	
	class IAudioSource
	{
	public:
	};
	
	class IAudioBuffer
	{
	public:
	};
	
	using AudioListenerList = std::list<IAudioListener *>;
	using AudioSourceList = std::list<IAudioSource *>;
	using AudioBufferList = std::list<IAudioBuffer *>;
	
	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem(void) = default;
		// Interface:
		virtual bool VActive(void) = 0;
		virtual StrongIAudioListenerPtr VInitAudioListener(void) = 0;
		virtual void VReleaseAudioListener(StrongIAudioListenerPtr pAudioListener) = 0;
		virtual StrongIAudioSourcePtr VInitAudioSource(void) = 0;
		virtual void VReleaseAudioSource(StrongIAudioSourcePtr pAudioSource) = 0;
		virtual StrongIAudioBufferPtr VInitAudioBuffer(StrongResourceHandlePtr pResourceHandle) = 0;
		virtual void VReleaseAudioBuffer(StrongIAudioBufferPtr pAudioBuffer) = 0;
		virtual void VStopAllSounds(void) = 0;
		virtual void VPauseAllSounds(void) = 0;
		virtual void VResumeAllSounds(void) = 0;
		virtual bool VInitialize(void) = 0;
		virtual void VShutdown(void) = 0;
	};

	class AudioSystem : public IAudioSystem, public INonCopyable, public INonMovable
	{
	protected:
		AudioBufferList m_buffers;
		bool m_bAllPaused;
		bool m_bInitialized;
	public:
		AudioSystem(void);
		virtual ~AudioSystem(void);
		// IAudioSystem's interface:
		virtual bool VActive(void) = 0;
		virtual StrongIAudioListenerPtr VInitAudioListener(void) = 0;
		virtual void VReleaseAudioListener(StrongIAudioListenerPtr pAudioListener) = 0;
		virtual StrongIAudioSourcePtr VInitAudioSource(void) = 0;
		virtual void VReleaseAudioSource(StrongIAudioSourcePtr pAudioSource) = 0;
		virtual StrongIAudioBufferPtr VInitAudioBuffer(StrongResourceHandlePtr pResourceHandle) = 0;
		virtual void VReleaseAudioBuffer(StrongIAudioBufferPtr pAudioBuffer) = 0;
		virtual void VStopAllSounds(void) = 0;
		virtual void VPauseAllSounds(void) = 0;
		virtual void VResumeAllSounds(void) = 0;
		virtual bool VInitialize(void) = 0;
		virtual void VShutdown(void) = 0;
		// Accessors:
		bool IsPaused(void);
		static bool HasSoundCard(void);
	};
} // End namespace (BGE)

#endif /* !_BGE_AUDIOSYSTEM_HPP_ */
