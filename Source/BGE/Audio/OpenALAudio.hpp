#ifndef _BGE_OPENALAUDIO_HPP_
#define _BGE_OPENALAUDIO_HPP_

namespace BGE
{
	class Audio : public IAudio
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
	};
	
	class AudioListener : public IAudioListener
	{
	public:
	};
	
	class AudioSource : public IAudioSource
	{
	public:
	};
	
	class AudioBuffer : public IAudioBuffer
	{
	public:
	};
} // End namespace (BGE)

#endif /* !_BGE_OPENALAUDIO_HPP_ */