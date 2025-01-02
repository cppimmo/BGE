/*******************************************************************************
 * @file   Audio.hpp
 * @author Brian Hoffpauir
 * @date   12.31.2024
 * @brief  Audio subsystem.
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
#ifndef _BGE_AUDIO_HPP_
#define _BGE_AUDIO_HPP_

#include <string_view>
#include <list>

#include "Resources/ResourceHandle.hpp"

namespace BGE
{
	class IAudioSystem; // Forward declare
	BGE_DECLARE_PTR(IAudioSystem);
	class IAudioBuffer; // Forward declare
	BGE_DECLARE_PTR(IAudioBuffer);
	class IAudioSource; // Forward declare
	BGE_DECLARE_PTR(IAudioSource);
	class IAudioListener; // Forward declare
	BGE_DECLARE_PTR(IAudioListener);

	//! List of strong IAudioBuffer pointers.
	using AudioBufferList = std::list<StrongIAudioBufferPtr>;
	//! List of strong IAudioSource pointers.
	using AudioSourceList = std::list<StrongIAudioSourcePtr>;
	//! List of strong IAudioListener pointers.
	using AudioListenerList = std::list<StrongIAudioListenerPtr>;

	//! Enum for representing the audio system used by an implementation.
	enum struct AudioImpl
	{
		kOpenAL /**< OpenAL audio implementation. */
	};
	//! Convert an AudioImpl to a string representation.
	constexpr std::string_view AudioImplToString(AudioImpl impl) noexcept;

	/**
	 * @brief .
	 */
	class IAudioBuffer
	{
	public:
		virtual ~IAudioBuffer(void) = default;
		// Interface:
		virtual bool VLoadFromResource(StrongResourceHandlePtr pHandle) = 0;
		virtual StrongResourceHandlePtr VGetResource(void) const = 0;
		virtual void *VGet(void) = 0;
		virtual int VGetFrequency(void) const = 0;
		virtual int VGetBitDepth(void) const = 0;
		virtual int VGetChannels(void) const = 0;
		virtual int VGetSize(void) const = 0;
		virtual bool VIsLoaded(void) const = 0;
	};
	
	/**
	 * @brief .
	 */
	class IAudioSource
	{
	public:
		virtual ~IAudioSource(void) = default;
		// Interface:
		virtual void VSetPosition(const glm::vec3 &kPosition) = 0;
		virtual const glm::vec3 &VGetPosition(void) const = 0;
		virtual void VSetVelocity(const glm::vec3 &kVelocity) = 0;
		virtual const glm::vec3 &VGetVelocity(void) const = 0;
		virtual void VSetVolume(float volume) = 0;
		virtual float VGetVolume(void) const = 0;

		virtual void VSetLooping(bool bLooping) = 0;
		virtual bool VIsLooping(void) const = 0;

		virtual void VPlay(void) = 0;
		virtual void VPause(void) = 0;
		virtual void VStop(void) = 0;
		virtual bool VIsPlaying(void) const = 0;
		virtual float VGetProgress(void) const = 0;

		virtual void VAttachBuffer(StrongIAudioBufferPtr pBuffer) = 0;
		virtual StrongIAudioBufferPtr VGetBuffer(void) const = 0;
		virtual void VDetachBuffer(void) = 0;
	};
	
	/**
	 * @brief .
	 */
	class IAudioListener
	{
	public:
		virtual ~IAudioListener(void) = default;
		// Interface:
		virtual void VSetVolume(float volume) = 0;
		virtual float VGetVolume(void) const = 0;
		virtual void VSetPosition(const glm::vec3 &kPosition) = 0;
		virtual void VSetVelocity(const glm::vec3 &kVelocity) = 0;
		virtual void VSetOrientation(const glm::vec3 &kForward, const glm::vec3 &kUp) = 0;
	};

	// TODO: Add audio capture API.
	/**
	 * @brief .
	 */
	class IAudioCapture
	{
	public:
		virtual ~IAudioCapture(void) = default;
		// Interface:
		virtual bool VStartCapture(void) = 0;
		virtual void VStopCapture(void) = 0;
		virtual std::vector<float> VGetCapturedData(void) const = 0;
	};

	//! .
	enum struct DistanceModel
	{
		kNone,              /**< No attenuation. */
		kInverse,           /**< OpenAL's AL_INVERSE_DISTANCE. */
		kInverseClamped,    /**< OpenAL's AL_INVERSE_DISTANCE_CLAMPED. */
		kLinear,            /**< OpenAL's AL_LINEAR_DISTANCE. */
		kLinearClamped,     /**< OpenAL's AL_LINEAR_DISTANCE_CLAMPED. */
		kExponential,       /**< OpenAL's AL_EXPONENT_DISTANCE. */
		kExponentialClamped /**< OpenAL's AL_EXPONENT_DISTANCE_CLAMPED. */
	};

	/**
	 * @brief .
	 */
	class IAudioSystem
	{
	public:
		virtual ~IAudioSystem(void) = default;
		// Interface:
		virtual bool VInit(void) = 0;
		virtual void VShutdown(void) = 0;
		virtual AudioImpl VGetImpl(void) const noexcept = 0;
		virtual std::size_t VGetActiveBufferCount(void) const = 0;
		virtual std::size_t VGetActiveSourceCount(void) const = 0;
		virtual DistanceModel VGetDistanceModel(void) const = 0;
		virtual void VSetDistanceModel(DistanceModel model) = 0;

		virtual StrongIAudioListenerPtr VCreateListener(void) = 0;
		virtual StrongIAudioSourcePtr VCreateSource(void) = 0;
		virtual StrongIAudioBufferPtr VCreateBuffer(void) = 0;

		virtual void VReleaseListener(StrongIAudioListenerPtr pListener) = 0;
		virtual void VReleaseSource(StrongIAudioSourcePtr pSource) = 0;
		virtual void VReleaseBuffer(StrongIAudioBufferPtr pBuffer) = 0;

		virtual void VSetListener(StrongIAudioListenerPtr pListener) = 0;
		virtual StrongIAudioListenerPtr VGetListener(void) const = 0;

		virtual void VPauseAll(void) = 0;
		virtual void VResumeAll(void) = 0;
		virtual void VStopAll(void) = 0;
		virtual bool VIsInitialized(void) const = 0;
	};

	inline constexpr std::string_view AudioImplToString(AudioImpl impl) noexcept
	{
		using namespace std::string_view_literals;
		switch (impl)
		{
		case AudioImpl::kOpenAL:
			return "OpenAL"sv;
		default:
			return "Unknown";
			break;
		}
	}
} // End namespace (BGE)

#endif /* !_BGE_AUDIO_HPP_ */
