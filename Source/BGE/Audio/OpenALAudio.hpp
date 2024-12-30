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
	public:
		// IAudioListener's interface:
		virtual void VSetPosition(const glm::vec3 &position) override;
		virtual void VSetVelocity(const glm::vec3 &velocity) override;
		virtual void VSetOrientation(const glm::vec3 &forward, const glm::vec3 &up) override;
	};

	/**
	 * @brief .
	 */
	class OpenALAudioSource final : public IAudioSource
	{
		ALuint m_sourceID;
		StrongIAudioBufferPtr m_pBuffer;
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

		virtual void VAttachBuffer(StrongIAudioBufferPtr pBuffer) override;
		virtual StrongIAudioBufferPtr VGetBuffer(void) const override;
		virtual void VDetachBuffer(void) override;
	};

	class OpenALAudioBuffer final : public IAudioBuffer
	{
		ALuint m_bufferID;
		bool m_bInitialized = false;
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
		ALCdevice *m_pDevice;
		ALCcontext *m_pContext;
		bool m_bAllPaused;
		bool m_bInitialized;
		StrongIAudioListenerPtr m_pActiveListener;
	public:
		OpenALAudioSystem(void);
		~OpenALAudioSystem(void) override;
		// IAudioSystem's interface:
		virtual bool VInitialize(void) override;
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
	};

	/*class OpenALAudioSystem : public IAudioSystem
	{
		ALCdevice *m_pDevice;
		ALCcontext *m_pContext;
		bool m_allPaused;
	public:
		Audio(void);
		~Audio(void);
	
		virtual bool VActive(void) { return false; }
	
		virtual IAudioListener *VInitAudioListener(void) { return nullptr; }
		virtual void VReleaseAudioListener(void) { }
	
		virtual IAudioSource *VInitAudioSource(void) { return nullptr; }
		virtual void VReleaseAudioSource(void) { }
	
		virtual IAudioBuffer *VInitAudioBuffer(void) { return nullptr; }
		virtual void VReleaseAudioBuffer(IAudioBuffer *pAudioBuffer) { }
	
		virtual void VStopAllSounds(void) { }
		virtual void VPauseAllSounds(void) { }
		virtual void VResumeAllSounds(void) { }
	
		virtual bool VInitialize(void) { return false; }
		virtual void VShutdown(void) { }
	
		ALCdevice *GetDevice(void) { return nullptr; }
		ALCcontext *GetContext(void) { return nullptr; }
		bool IsPaused(void) const { return false; }
	};*/
} // End namespace (BGE)

#endif /* !_BGE_OPENALAUDIO_HPP_ */
